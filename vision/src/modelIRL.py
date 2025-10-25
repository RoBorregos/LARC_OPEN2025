from ultralytics import YOLO
import cv2
from collections import Counter
import torch

# Load TensorRT engine
model = YOLO("model/nanoModel.engine", task="detect")
class_names = model.names

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 416)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 416)
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

print("Running detection... Press ESC to exit.")

while True:
    ret, frame = cap.read()
    if not ret:
        continue

    # Inference optimized
    results = model(frame, conf=0.6, iou=0.45, imgsz=416, verbose=False)

    counts = Counter()

    # Prevent NoneType errors
    if results and results[0].boxes is not None:
        for box in results[0].boxes:
            cls_id = int(box.cls)
            cls_name = class_names[cls_id]
            counts[cls_name] += 1

    print(dict(counts))

    # clear GPU memory
    torch.cuda.empty_cache()

    # ESC to exit
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
