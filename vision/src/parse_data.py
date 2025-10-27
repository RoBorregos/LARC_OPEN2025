import time
import cv2
from ultralytics import YOLO
import serial

# ------------ Configuration ------------
MODEL_PATH   = "model/nanoModel.engine"
IGNORE_CLASS = {"blue_benefit", "red_benefit"}
SOURCE       = 0 # Use 0 for xavier
NONE_TEXT    = "none"
TIMEOUT_SEC  = 0.5  # reset to none if > TIMEOUT_SEC since last detection

def detect(model: YOLO, frame, ignore_set):
    h, w = frame.shape[:2]
    mid_x = w // 2

    # NOTE: "top" means LEFT side, "bottom" means RIGHT side, because of camera arrangement
    best_for = {
        "bottom": {"label": None, "conf": -1.0, "bbox": None},   # RIGHT
        "top":    {"label": None, "conf": -1.0, "bbox": None},   # LEFT
    }

    t0 = time.time()
    results = model.predict(frame, verbose=False)
    infer_ms = (time.time() - t0) * 1000.0

    r = results[0]
    if r.boxes is not None and len(r.boxes) > 0:
        for box in r.boxes:
            cls_id = int(box.cls[0])
            label = model.names.get(cls_id, str(cls_id))
            if label in ignore_set:
                continue

            conf = float(box.conf[0]) if box.conf is not None else 0.0
            x1, y1, x2, y2 = map(float, box.xyxy[0])
            cx = 0.5 * (x1 + x2)
            side = "bottom" if cx < mid_x else "top"

            if conf > best_for[side]["conf"]:
                best_for[side] = {"label": label, "conf": conf, "bbox": (x1, y1, x2, y2)}

    return r, best_for, mid_x, infer_ms

def update_last_state(best_for, last_label, last_seen_ts, now, timeout_sec):
    for side in ("bottom", "top"):
        candidate_label = best_for[side]["label"]
        if candidate_label is not None:
            last_label[side] = candidate_label
            last_seen_ts[side] = now
        else:
            last_time = last_seen_ts[side]
            if last_time is not None and (now - last_time) > timeout_sec:
                last_label[side] = None

def build_matrix(last_label, none_text):
    # Order: [top (left), bottom (right)]
    return [
        last_label["top"] if last_label["top"] is not None else none_text,
        last_label["bottom"] if last_label["bottom"] is not None else none_text,
    ]

def print_status(matrix, last_seen_ts, now, infer_ms):
    def format_time(side):
        ts = last_seen_ts[side]
        if ts is None:
            return "N/A"
        return f"{now - ts:.1f}s"

    print(
        f"{matrix} | "
        f"last seen → top: {format_time('top')}, bottom: {format_time('bottom')} | "
        f"inference: {infer_ms:.1f} ms"
    )

def annotate_and_show(r, frame, matrix, mid_x, best_for):
    annotated = r.plot(boxes=False, labels=True, conf=False)
    h, w = frame.shape[:2]

    # Vertical midline
    cv2.line(annotated, (mid_x, 0), (mid_x, h), (255, 255, 255), 1)

    # Draw chosen bbox per side (thicker), with class & conf
    for side, color in (("top", (0, 255, 0)), ("bottom", (0, 200, 255))):
        info = best_for[side]
        if info["bbox"] is None or info["label"] is None:
            continue
        x1, y1, x2, y2 = map(int, info["bbox"])
        cv2.rectangle(annotated, (x1, y1), (x2, y2), color, 3)
        label_text = f"{info['label']} {info['conf']:.2f}"
        # Put text just above the bbox
        y_txt = max(0, y1 - 8)
        cv2.putText(
            annotated, label_text, (x1, y_txt),
            cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2, cv2.LINE_AA
        )

    hud_lines = [f"[{matrix[0]}, {matrix[1]}]"]
    x0, y0, dy = 10, 24, 24
    pad = 8
    box_w = max(cv2.getTextSize(s, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2)[0][0] for s in hud_lines) + 2 * pad
    box_h = len(hud_lines) * dy + 2 * pad - (dy - 20)
    cv2.rectangle(annotated, (5, 5), (5 + box_w, 5 + box_h), (0, 0, 0), -1)
    for i, text in enumerate(hud_lines):
        cv2.putText(
            annotated, text, (x0, y0 + i * dy),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2, cv2.LINE_AA
        )

    cv2.imshow("Detections (with matrix)", annotated)


def send_matrix_to_esp32(matrix, port='/dev/ttyUSB0', baudrate=115200):
    """
    Sends the matrix data to the ESP32 via serial communication.
    """
    try:
        ser = serial.Serial(port, baudrate, timeout=1)

        # inmature -> 0, mature -> 1, overmature -> 2
        mapping = {
            "inmature": "0",
            "mature": "1",
            "overmature": "2",
            NONE_TEXT: "-1",
        }

        numeric_items = []
        for item in matrix:
            if isinstance(item, str):
                key = item.strip().lower()
                numeric = mapping.get(key)
                if numeric is None:
                    try:
                        int(key)
                        numeric = key
                    except ValueError:
                        numeric = "-1"
                numeric_items.append(numeric)
            else:
                numeric_items.append(str(item))

        matrix_str = ','.join(numeric_items) + '\n'
        ser.write(matrix_str.encode('utf-8'))

    except serial.SerialException as e:
        print(f"Error in serial communication: {e}")


# ------------ Main loop ------------
def run(verbose: bool):
    last_label   = {"bottom": None, "top": None} #init in None, None
    last_seen_ts = {"bottom": None, "top": None}

    model = YOLO(MODEL_PATH, task="detect")
    cap = cv2.VideoCapture(SOURCE)
    if not cap.isOpened():
        raise RuntimeError(f"Could not open video source: {SOURCE}")
    
    # Tuning (best-effort; may be ignored by some cameras/backends)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    cap.set(cv2.CAP_PROP_FPS, 30)

    try:
        while True:
            ok, frame = cap.read()
            if not ok:
                break

            now = time.time()

            r, best_for_half, mid_x, infer_ms = detect(model, frame, IGNORE_CLASS)
            update_last_state(best_for_half, last_label, last_seen_ts, now, TIMEOUT_SEC)
            matrix = build_matrix(last_label, NONE_TEXT)

            # Always print to terminal
            print_status(matrix, last_seen_ts, now, infer_ms)
            send_matrix_to_esp32(matrix)

            # Only show annotated window if verbose
            if verbose:
                annotate_and_show(r, frame, matrix, mid_x, best_for_half)
                # Exit on ESC key
                if cv2.waitKey(1) & 0xFF == 27:
                    break

    finally:
        cap.release()
        if verbose:
            cv2.destroyAllWindows()


if __name__ == "__main__":
    run(verbose=False)