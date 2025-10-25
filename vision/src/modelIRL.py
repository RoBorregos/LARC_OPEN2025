import math
import time
from ultralytics import YOLO
import cv2

# Load model
model = YOLO("model/nanoModel.pt")  # Usa YOLO n
model.fuse()  # Acelera inferencia
IGNORE_CLASS = "green bean"
class_names = model.names

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 20)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

print("ESC to exit...")

while True:
    ret, frame = cap.read()
    if not ret:
        print("No frame from camera")
        break

    # Run YOLO inference (small resolution faster on Jetson)
    results = model.predict(frame, imgsz=640, conf=0.4, verbose=False)

    # EXTRAER DETECCIONES
    detections = []
    for r in results:
        for box in r.boxes:
            class_id = int(box.cls[0])
            cls_name = class_names[class_id]
            conf = float(box.conf[0])
            x1, y1, x2, y2 = box.xyxy[0].tolist()

            if cls_name != IGNORE_CLASS:
                detections.append({
                    "class": cls_name,
                    "confidence": conf,
                    "bbox": [x1, y1, x2, y2]
                })

    print("Detections:", detections)  # <- AQUÍ YA RECIBES DETECCIONES

    if cv2.waitKey(1) & 0xFF == 27:  # ESC
        break

cap.release()
cv2.destroyAllWindows()