import cv2
from ultralytics import YOLO
import enum
import time
from typing import List, Optional
from dataclasses import dataclass
import serial

class CameraState(enum.Enum):
    DETECT_BEANS = enum.auto()
    DETECT_RED_STORAGE = enum.auto()
    DETECT_BLUE_STORAGE = enum.auto()
    STOP = enum.auto()

@dataclass
class DetectOutput:
    label: str
    confidence: float
    bbox: list  # [x1, y1, x2, y2]

class Camera:
    def __init__(self,
                 model: YOLO,
                 timeout: float = 2.0):
        
        self.MODEL = model
        self.TIMEOUT = timeout
        self.DEBUG = False
        
        self._frame: Optional[cv2.Mat] = None
        self._state = CameraState.STOP
        self._infer_ms = 0.0
        self._last_read_s = { "top": 0.0, "bottom": 0.0 }  # for top and bottom halves

        #* output matrix, always 1x2
        # DETECT_BEANS:    ["<top>", "<bottom>"]
        # DETECT_STORAGE:  ["<benefit_type>", <offset_x>]
        # STOP  ["none", "none"]
        self.detection_matrix = [None, None]

    #* --------- for SystemController ---------
    def set_state(self, new_state: CameraState) -> None:
        if not isinstance(new_state, CameraState):
            raise ValueError("Invalid state")
        self._state = new_state

    def get_state(self) -> CameraState:
        return self._state
    
    #* --------- Helpers ---------
    def _print_data(self) -> None:
        state_name = getattr(self._state, "name", "UNKNOWN")
        print(f"[Camera] {state_name} data → {self.detection_matrix} | infer_ms → {self._infer_ms:.1f} ms")

    def _show(self, detections) -> None:
        if self._frame is None:
            return 
        annotated = self._frame.copy()
        h, w = annotated.shape[:2]
        mid_x = w // 2
        mid_y = h // 2

        def draw_text_with_background(image, text, position, font, font_scale, text_color, bg_color, thickness):
            text_size = cv2.getTextSize(text, font, font_scale, thickness)[0]
            text_x, text_y = position
            box_coords = ((text_x, text_y - text_size[1] - 5), (text_x + text_size[0] + 5, text_y + 5))
            cv2.rectangle(image, box_coords[0], box_coords[1], bg_color, cv2.FILLED)
            cv2.putText(image, text, (text_x, text_y), font, font_scale, text_color, thickness)

        if self._state == CameraState.STOP:
            draw_text_with_background(annotated, "STATE: STOP", (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), (0, 0, 0), 2)

        elif self._state == CameraState.DETECT_BEANS:
            cv2.line(annotated, (mid_x, 0), (mid_x, h), (255, 255, 255), 1)
            draw_text_with_background(annotated, "STATE: DETECT_BEANS", (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), (0, 0, 0), 2)

        else:  # CameraState.DETECT_STORAGE (red or blue)
            cv2.line(annotated, (0, mid_y), (w, mid_y), (255, 255, 255), 1)
            draw_text_with_background(annotated, "STATE: DETECT_STORAGE", (10, 25), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), (0, 0, 0), 2)

        draw_text_with_background(annotated, f"{self.detection_matrix}", (10, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), (0, 0, 0), 2)
        draw_text_with_background(annotated, f"{self._infer_ms:.2f} ms", (10, 75), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), (0, 0, 0), 2)

        for det in detections:
            if det is None or not det.bbox or len(det.bbox) != 4: # Skip invalid or empty bboxess
                continue
            x1, y1, x2, y2 = map(int, det.bbox)

            cv2.rectangle(annotated, (x1, y1), (x2, y2), (0, 255, 0), 2)
            label_text = f"{det.label} ({det.confidence:.2f})"
            cv2.putText(annotated, label_text, (x1, y1 - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

        # cv2.imshow("Detections", annotated)
    
    def _read_frame(self) -> List[DetectOutput]:
        if self._frame is None:
            self._infer_ms = 0.0
            return []

        t0 = time.time()
        results = self.MODEL.predict(self._frame, verbose=False)
        self._infer_ms = (time.time() - t0) * 1000.0

        outputs: List[DetectOutput] = []
        if not results:
            return outputs

        # Assume first batch element (frame)
        result = results[0]
        if getattr(result, "boxes", None) is None or len(result.boxes) == 0:
            return outputs

        for box in result.boxes:
            cls_id = int(box.cls[0])
            label = self.MODEL.names.get(cls_id, str(cls_id))
            conf = float(box.conf[0]) if getattr(box, "conf", None) is not None else 0.0
            bbox = list(map(float, box.xyxy[0]))
            outputs.append(DetectOutput(label=label, confidence=conf, bbox=bbox))

        return outputs


    def detect(self) -> List[Optional[DetectOutput]]:
        '''State manager'''
        if self._state == CameraState.STOP:
            self.detection_matrix = [None, None]
            self._infer_ms = 0.0
            return []
        
        elif self._state == CameraState.DETECT_BEANS:
            return self._detect_beans()
        
        elif self._state == CameraState.DETECT_RED_STORAGE:
            return self._detect_storage(benefit_type="red_benefit")
        
        else: # CameraState.DETECT_BLUE_STORAGE
            return self._detect_storage(benefit_type="blue_benefit")
        
    #* --------- State Handlers ---------
    def _detect_beans(self, ser: serial.Serial = None) -> List[Optional[DetectOutput]]:
        '''Returns list of 2 DetectOutput for top and bottom halves and optionally sends to Teensy'''
        targets = {"inmature", "mature", "overmature"}
        label_to_num = {"inmature": 0, "mature": 1, "overmature": 2}

        # THRESHOLD
        THRESHOLD_TOP_MIN = 50
        THRESHOLD_TOP_MAX = 160
        THRESHOLD_BOTTOM_MIN = 70
        THRESHOLD_BOTTOM_MAX = 170

        results = self._read_frame()
        now = time.time()

        h, w = self._frame.shape[:2]
        mid_x = w * 0.5

        default_output = DetectOutput(label=None, confidence=-1.0, bbox=[])
        best_for = {"top": default_output, "bottom": default_output}

        # Timeout: limpiar detections viejas
        for side in ["top", "bottom"]:
            idx = 0 if side == "top" else 1
            if self.detection_matrix[idx] is not None and self._last_read_s[side] + self.TIMEOUT < now:
                self.detection_matrix[idx] = None

        # Seleccionar la mejor detección por lado
        for det in results:
            if det.label not in targets:
                continue
            x1, y1, x2, y2 = det.bbox
            cx = 0.5 * (x1 + x2)
            cy = 0.5 * (y1 + y2)
            side = "top" if cx > mid_x else "bottom"
            idx = 0 if side == "top" else 1
            if self.detection_matrix[idx] is None or det.confidence > best_for[side].confidence:
                best_for[side] = det

        # Construir matriz de salida considerando umbrales
        matrix_to_send = [0, 0]  # None o fuera de rango → 0
        final_detections = [None, None]
        cy_positions = [None, None]

        for idx, side in enumerate(["top", "bottom"]):
            best_det = best_for[side]
            if best_det.label is not None:
                x1, y1, x2, y2 = best_det.bbox
                cy = 0.5 * (y1 + y2)
                cy_positions[idx] = cy

                # Validación con min/max thresholds
                if side == "top" and THRESHOLD_TOP_MIN <= cy <= THRESHOLD_TOP_MAX:
                    matrix_to_send[idx] = label_to_num[best_det.label]
                    final_detections[idx] = best_det
                    self._last_read_s[side] = now
                elif side == "bottom" and THRESHOLD_BOTTOM_MIN <= cy <= THRESHOLD_BOTTOM_MAX:
                    matrix_to_send[idx] = label_to_num[best_det.label]
                    final_detections[idx] = best_det
                    self._last_read_s[side] = now

        self.detection_matrix = matrix_to_send

        # Imprimir resultados
        # print(f"[DETECT] matrix={matrix_to_send} → cy_top={cy_positions[0]}, cy_bottom={cy_positions[1]}")

        # Enviar a Teensy si se pasó el serial
        if ser is not None:
            try:
                msg = f"{matrix_to_send[0]},{matrix_to_send[1]}\n".encode()
                ser.write(msg)
                ser.flush()
            except Exception as e:
                print(f"[WARN] Failed to send to Teensy: {e}")

        return final_detections

    def _detect_storage(self, benefit_type: str) -> List[Optional[DetectOutput]]:
        '''Returns single DetectOutput for specified benefit_type and offset'''
        # NOTE: camera is rotated 90 degrees, so we use vertical offset
        results = self._read_frame()
        h, w = self._frame.shape[:2]
        midline = h//2 # horizontal midline / center of frame
        
        storage = DetectOutput(label=benefit_type, confidence = 0.0, bbox = [])
        offset: float = 0.0  # normalized offset (-1.0 to 1.0)
        found: bool = False
        
        if getattr(results, "boxes", None) is not None and len(results.boxes) > 0:
            for box in results.boxes:
                cls_id = int(box.cls[0])
                label = self.MODEL.names.get(cls_id, str(cls_id))
                if label != benefit_type:
                    continue

                conf = float(box.conf[0]) if box.conf is not None else 0.0 # get only info of benefit with highest confidence
                
                if conf > storage.confidence:
                    x1, y1, x2, y2 = map(float, box.xyxy[0])
                    vertical_center = 0.5 * (y1 + y2)
                    norm_offset = (vertical_center - midline) / (h / 2)
                    norm_offset = max(-1.0, min(1.0, norm_offset))

                    storage = DetectOutput(label=label, confidence=conf, bbox=[x1, y1, x2, y2])
                    offset = round(norm_offset, 2)
                    found = True
                    
        self.detection_matrix = [benefit_type, offset]
        return storage if found else []

    #* --------- Main ---------
    def run(self, frame, verbose=False) -> None:
        self._frame = frame
        self.get_state()

        det = self.detect()
        self._print_data()

        if verbose:
            self._show(det)

if __name__ == "__main__":
    cap = None
    ser = None
    try:
        # Cargar modelo
        model = YOLO("model/nanoModel.pt", task="detect")
        cam = Camera(model=model, timeout=1.0)

        # Abrir cámara
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            raise RuntimeError("Failed to open camera/video source.")

        # Abrir puerto serial para Teensy
        try:
            ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
            print("[INFO] Serial port opened for Teensy")
        except serial.SerialException as e:
            print(f"[WARN] Could not open serial port: {e}")
            ser = None

        cam.set_state(CameraState.DETECT_BEANS)

        while True:
            ret, frame = cap.read()
            if not ret:
                print("Failed to read frame.")
                break

            # Pasar serial a _detect_beans mediante run
            cam._frame = frame
            det = cam.detect()  # Esto llamará internamente a _detect_beans
            if cam._state == CameraState.DETECT_BEANS:
                cam._detect_beans(ser=ser)  # Enviar datos a Teensy aquí

            cam._print_data()

            if True:  # verbose
                cam._show(det)

            if cv2.waitKey(1) & 0xFF == 27:
                break

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if cap is not None:
            cap.release()
        if ser is not None:
            ser.close()
            print("[INFO] Serial port closed")
        cv2.destroyAllWindows()