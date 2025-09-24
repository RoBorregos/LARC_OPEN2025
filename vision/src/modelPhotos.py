import math
import time
from ultralytics import YOLO
import cv2

model = YOLO("vision/model/mediumModel.pt")
class_names = model.names

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()
print("Taking photos each 10 seconds. Press Ctrl + C to exit")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("No se pudo leer el frame.")
            break

        results = model(frame)
        boxes = results[0].boxes

        class_counts = {}
        for box in boxes:
            cls = int(box.cls[0])
            label = class_names[cls]
            class_counts[label] = class_counts.get(label, 0) + 1

        print("Conteo de clases detectadas:")
        for label, count in class_counts.items():
            print(f"{label}: {count}")

        # Save the image in case of
        # timestamp = int(time.time())
        # cv2.imwrite(f"photo_{timestamp}.jpg", frame)

        # Espera 10 minutos (600 segundos)
        time.sleep(10)

except KeyboardInterrupt:
    print("Proceso terminado por el usuario.")

cap.release()
cv2.destroyAllWindows()