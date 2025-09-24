import math
from ultralytics import YOLO
import cv2

model = YOLO("vision/model/mediumModel.pt")
IGNORE_CLASS = "green bean" 
class_names = model.names  

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()
print("ESC to exit.")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame)
    annotated_img = results[0].plot()

    centers = []
    for box in results[0].boxes:
        cls = int(box.cls[0])       
        label = class_names[cls]

        if label == IGNORE_CLASS:
            continue  

        x1, y1, x2, y2 = box.xyxy[0]
        cx = int((x1 + x2) / 2)
        cy = int((y1 + y2) / 2)
        centers.append((cx, cy))

        cv2.circle(annotated_img, (cx, cy), 5, (0, 0, 255), -1)
    tolerance = 8
    centers.sort(key=lambda c: c[1]) 

    for i, (cx, cy) in enumerate(centers):
        for j, (cx2, cy2) in enumerate(centers):
            if j > i and abs(cx - cx2) < tolerance:
                cv2.line(annotated_img, (cx, cy), (cx2, cy2), (0, 255, 0), 2)

    cv2.imshow("YOLO inference", annotated_img)

    if cv2.waitKey(1) & 0xFF == 27: 
        break

cap.release()
cv2.destroyAllWindows()
