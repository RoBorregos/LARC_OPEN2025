import math
from ultralytics import YOLO
import cv2

# Load model once
model = YOLO("model/nanoModel.pt")
class_names = model.names  

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()

# Set camera properties for better performance
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 30)

print("ESC to exit.")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Run inference
        results = model(frame, verbose=False)
        
        # Count classes
        boxes = results[0].boxes
        labels = [class_names[int(b.cls[0])] for b in boxes]
        inmature = sum(1 for l in labels if l == "inmature")
        mature = sum(1 for l in labels if l == "mature")
        overmature = sum(1 for l in labels if l == "overmature")

        print(f"Inmature beans: {inmature}")
        print(f"Mature beans: {mature}")
        print(f"Overmature beans: {overmature}")

        # Display results
        annotated_img = results[0].plot()
        cv2.imshow('Detection', annotated_img)
        
        # Exit on ESC key
        if cv2.waitKey(1) & 0xFF == 27:  # ESC key
            break

finally:
    cap.release()
    cv2.destroyAllWindows()
