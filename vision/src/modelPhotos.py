import math
import time
from ultralytics import YOLO
import cv2

model = YOLO("vision/model/mediumModel.pt")
class_names = model.names

cap = cv2.VideoCapture(2)
if not cap.isOpened():
    print("Can't open camera.")
    exit()
print("Succesfully started")

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

        target_labels = ["mature", "overmature", "inmature"]
        coffes = sum(class_counts.get(label, 0) for label in target_labels)
        if(coffes == 16):
            print("Tree is fully detected")
        else:
            print("Tree is not fully detected")

        matrix = [[0 for _ in range(3)] for _ in range(10)]

        for box in boxes:
            x1, y1, x2, y2 = box.xyxy[0]
            center_x = int((x1 + x2) / 2)
            center_y = int((y1 + y2) / 2)

            height, width, _ = frame.shape

            col = min(9, max(0, int(center_x / width * 10)))
            row = min(2, max(0, int(center_y / height * 3)))

            cls = int(box.cls[0])
            label = class_names[cls]

            if label in ["mature", "overmature"]:
                matrix[col][row] = 1
            elif label == "inmature":
                matrix[col][row] = 0

        print("Matriz de conteo:")
        for row in matrix:
            print(row)

        # Save the image in case of
        # timestamp = int(time.time())
        # cv2.imwrite(f"photo_{timestamp}.jpg", frame)

        # Espera 10 minutos (600 segundos)
        time.sleep(5)

except KeyboardInterrupt:
    print("Proceso terminado por el usuario.")

cap.release()
cv2.destroyAllWindows()