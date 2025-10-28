import cv2
from ultralytics import YOLO
import enum
import time
from typing import List, Optional
from dataclasses import dataclass

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
                 timeout: float = 0.5):
        
        self.MODEL = model
        self.TIMEOUT = timeout
        
        self._frame: Optional[cv2.Mat] = None
        self._state = CameraState.STOP
        self._infer_ms = 0.0

        #* output matrix, always 1x2
        # DETECT_BEANS:    ["<top>", "<bottom>"]
        # DETECT_STORAGE:  ["<benefit_type>", <offset_x>]
        # STOP  ["none", "none"]
        self.detection_matrix = [None, None]

    #* --------- for SystemController ---------
    def set_state(self, new_state: CameraState):
        if not isinstance(new_state, CameraState):
            raise ValueError("Invalid state")
        self._state = new_state

    def get_state(self) -> CameraState:
        return self._state
    
    #* --------- Helpers ---------
    def _print_data(self) -> None:
        state_name = getattr(self._state, "name", "UNKNOWN")
        print(f"[Camera] {state_name} data → {self.detection_matrix} | infer_ms → {self._infer_ms:.1f} ms")

    def _show(self, detections: List[DetectOutput] | None) -> None:
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

        cv2.imshow("Detections", annotated)
    
    def _infer(self) -> any:
        if self._frame is None:
            self._infer_ms = 0.0
            return None 
        t0 = time.time()
        results = self.MODEL.predict(self._frame, verbose=False)
        self._infer_ms = (time.time() - t0) * 1000.0
        return results[0] if results else None

    def detect(self) -> List[DetectOutput]:
        if self._state == CameraState.STOP:
            self.detection_matrix = [None, None]
            self._infer_ms = 0.0
            return []
        
        elif self._state == CameraState.DETECT_BEANS:
            return self._detect_beans()
        
        elif self._state == CameraState.DETECT_RED_STORAGE:
            return [self._detect_storage(benefit_type="red_benefit")]
        
        else: # CameraState.DETECT_BLUE_STORAGE
            return [self._detect_storage(benefit_type="blue_benefit")]
        
    #* --------- State Handlers ---------
    def _detect_beans(self) -> List[DetectOutput]:
        targets = {"inmature", "mature", "overmature"}

        # Initialize or maintain sticky state
        if not hasattr(self, '_last_label'):
            self._last_label = {"bottom": None, "top": None}
            self._last_seen_ts = {"bottom": None, "top": None}

        # Perform inference
        results = self._infer()
        now = time.time()

        # Process detection results
        h, w = self._frame.shape[:2]
        mid_x = w // 2
        best_for = {
            "top": {"label": None, "conf": -1.0, "bbox": None}, # LEFT
            "bottom": {"label": None, "conf": -1.0, "bbox": None}, # RIGHT
        }

        if results.boxes is not None and len(results.boxes) > 0:
            for box in results.boxes:
                cls_id = int(box.cls[0])
                label = self.MODEL.names.get(cls_id, str(cls_id))
                if label not in targets:
                    continue

                conf = float(box.conf[0]) if box.conf is not None else 0.0
                x1, y1, x2, y2 = map(float, box.xyxy[0])
                cx = 0.5 * (x1 + x2)
                side = "top" if cx < mid_x else "bottom"

                if conf > best_for[side]["conf"]:
                    best_for[side] = {"label": label, "conf": conf, "bbox": (x1, y1, x2, y2)}

        # Update sticky state
        for side in ("top", "bottom"):
            candidate_label = best_for[side]["label"]
            if candidate_label is not None:
                self._last_label[side] = candidate_label
                self._last_seen_ts[side] = now
            else:
                last_time = self._last_seen_ts[side]
                if last_time is not None and (now - last_time) > self.TIMEOUT:
                    self._last_label[side] = None

        # Build matrix
        self.detection_matrix = [self._last_label["top"], self._last_label["bottom"]]

        # Return detection outputs
        return [
        DetectOutput(label=best_for["top"]["label"], confidence=best_for["top"]["conf"], bbox=best_for["top"]["bbox"]) if best_for["top"]["label"] else None,
        DetectOutput(
            label=best_for["bottom"]["label"],
            confidence=best_for["bottom"]["conf"],
            bbox=best_for["bottom"]["bbox"]) if best_for["bottom"]["label"] else None,
    ]

    def _detect_storage(self, benefit_type: str) -> DetectOutput:
        # NOTE: camera is rotated 90 degrees, so we use vertical offset
        results = self._infer()
        h, w = self._frame.shape[:2]
        midline = h//2 # horizontal midline / center of frame
        
        storage = DetectOutput(label=benefit_type, confidence = 0.0, bbox = [])
        offset: float = 0.0  # normalized offset (-1.0 to 1.0)
        
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
                    
        self.detection_matrix = [benefit_type, offset]
        return storage
    
    #* --------- Main ---------
    def run(self, frame, verbose=False) -> None:
        self._frame = frame
        self.get_state()

        det = self.detect()

        self._print_data()

        if verbose:
            self._show(det)




    



### testing main
if __name__ == "__main__":
    cap = None
    try:
        model = YOLO("model/nanoModel.pt", task="detect")
        cam = Camera(model=model, timeout=0.5)

        cap = cv2.VideoCapture(2)  # or a file path
        if not cap.isOpened():
            raise RuntimeError("Failed to open camera/video source.")

        cam.set_state(CameraState.DETECT_RED_STORAGE)

        while True:
            ret, frame = cap.read()
            if not ret:
                print("Failed to read frame.")
                break

            cam.run(frame, verbose=True)

            if cv2.waitKey(1) & 0xFF == 27:
                break

    except KeyboardInterrupt:
        print("\nInterrupted by user.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if cap is not None:
            cap.release()
        cv2.destroyAllWindows()