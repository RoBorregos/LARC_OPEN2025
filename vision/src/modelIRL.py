import math
from ultralytics import YOLO
import cv2

# Load model once
model = YOLO("model/nanoModel.engine")
IGNORE_CLASS = "green bean" 
class_names = model.names  

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()

# Set camera properties for better performance
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 60)

print("ESC to exit.")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Run inference
        results = model(frame, verbose=False)
        
        # Display results
        annotated_img = results[0].plot()
        cv2.imshow('Detection', annotated_img)
        
        # Exit on ESC key
        if cv2.waitKey(1) & 0xFF == 27:  # ESC key
            break

finally:
    cap.release()
    cv2.destroyAllWindows()
