from ultralytics import YOLO
import cv2
from collections import Counter
import torch

model = YOLO("model/nanoModel.pt")  # PyTorch model

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 480)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

while True:
    ret, frame = cap.read()
    if not ret:
        continue

    results = model(frame, verbose=False)  # <-- imgsz correcto

    counts = Counter()
    for r in results:
        for box in r.boxes:
            cls = model.names[int(box.cls)]
            counts[cls] += 1

    print(dict(counts))
    torch.cuda.empty_cache()

    if cv2.waitKey(1) == 27:
        break

cap.release()