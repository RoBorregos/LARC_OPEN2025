# src/modelIRL_trt_fast.py
from ultralytics import YOLO
import cv2
from collections import Counter
import torch
import time

# CONFIG
ENGINE_PATH = "model/nanoModel.engine"   # ruta al .engine
CAP_INDEX = 0                            # cámara USB index
IMG_SIZE = 416                           # debe coincidir con la exportación
CONF = 0.45
IOU = 0.45
FRAME_SKIP = 0                           # 0 = no skip, 1 = process every 2nd frame, etc.
PRINT_FPS = True

# Load model (task explicit avoids guess warning)
print("Loading", ENGINE_PATH, "for TensorRT inference...")
model = YOLO(ENGINE_PATH, task="detect")
class_names = model.names or {}

# warmup (pre-allocate buffers)
try:
    model.warmup(imgsz=(1, 3, IMG_SIZE, IMG_SIZE))
except Exception:
    pass

# Open camera
cap = cv2.VideoCapture(CAP_INDEX)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, IMG_SIZE)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, IMG_SIZE)
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

print("Running detection... Press ESC to exit.")

frame_id = 0
t0 = time.time()
proc_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        continue

    frame_id += 1
    if FRAME_SKIP and (frame_id % (FRAME_SKIP + 1) != 0):
        continue

    # ensure frame matches expected size (OpenCV can return other sizes)
    if frame.shape[1] != IMG_SIZE or frame.shape[0] != IMG_SIZE:
        frame = cv2.resize(frame, (IMG_SIZE, IMG_SIZE))

    # inference: force imgsz same as engine
    results = None
    try:
        results = model(frame, conf=CONF, iou=IOU, imgsz=IMG_SIZE, verbose=False)
    except AssertionError as e:
        # fallback: try predict() which is a tiny bit different
        try:
            results = model.predict(source=frame, conf=CONF, iou=IOU, imgsz=IMG_SIZE, verbose=False)
        except Exception as ex:
            print("Inference error:", ex)
            results = None

    counts = Counter()
    if results and len(results) > 0:
        res = results[0]
        # some backends might set boxes to None
        if getattr(res, "boxes", None):
            for box in res.boxes:
                # check cls exists
                if getattr(box, "cls", None) is None:
                    continue
                cls_id = int(box.cls)
                # guard against missing class names
                cls_name = class_names.get(cls_id, f"class_{cls_id}")
                counts[cls_name] += 1

    # print result: classes + counts
    print(dict(counts))

    # fps printing
    proc_count += 1
    if PRINT_FPS and proc_count % 10 == 0:
        dt = time.time() - t0
        fps = proc_count / dt if dt > 0 else 0
        print(f"[INFO] avg FPS (last {proc_count} frames): {fps:.2f}")

    # free some GPU memory (harmless)
    torch.cuda.empty_cache()

    # exit check
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
