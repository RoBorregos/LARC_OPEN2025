import numpy as np
import time
from ultralytics import YOLO
import cv2

model = YOLO("vision/model/mediumModel.pt")
class_names = model.names
zoom_factor = 4

cap = cv2.VideoCapture(2)
if not cap.isOpened():
    print("Can't open camera.")
    exit()

tree_detection_tolerance = 0.8 
coffee_labels = ["mature", "overmature", "inmature"]

# image = cv2.imread("vision/imagesTest/IMG_2503.JPG") It has a tree and the coffes
image = cv2.imread("vision/imagesTest/IMG_2526.JPG")
image = cv2.resize(image, (640, 480)) if image is not None else None

def zoom_frame(frame, zoom_factor):
    height, width = frame.shape[:2]
    new_height = int(height * zoom_factor)
    new_width = int(width * zoom_factor)
    
    # Resize the frame
    resized_frame = cv2.resize(frame, (new_width, new_height))
    
    # Calculate the region of interest (ROI)
    roi_x = int((new_width - width) / 2)
    roi_y = int((new_height - height) / 2)
    roi_width = width
    roi_height = height
    
    # Crop the frame to the ROI
    zoomed_frame = resized_frame[roi_y:roi_y+roi_height, roi_x:roi_x+roi_width]
    return zoomed_frame

# Option 1: Use camera
# source = cap

# Option 2: Use image
source = image

if source is cap:
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                print("No se pudo leer el frame.")
                break
            
            zoomed_frame = zoom_frame(frame, zoom_factor)
            
            # Display the zoomed frame
            # cv2.imshow('Zoomed Video', zoomed_frame)

            # if cv2.waitKey(1) & 0xFF == ord('q'):
            #     break     
              
            results = model(zoomed_frame)
            boxes = results[0].boxes

            trees = []
            coffees = []

            for box in boxes:
                cls = int(box.cls[0])
                conf = float(box.conf[0])
                label = class_names[cls]

                if conf < tree_detection_tolerance:
                    continue

                xyxy = box.xyxy[0].tolist()

                if label == "tree":
                    trees.append((xyxy, conf))
                elif label in coffee_labels:
                    coffees.append((xyxy, label, conf))
                elif len(trees) > 3:
                    print(f"\nHay {len(trees)} árboles, pero solo se procesa el caso exacto de 3.")
                else:
                    print("\nNo se detectaron al menos 3 árboles válidos.")

            time.sleep(2)
        cap.release()
        cv2.destroyAllWindows()
    except KeyboardInterrupt:
        print("Proceso terminado por el usuario.")

    cap.release()

else:
    if image is not None:
        results = model(image)
        boxes = results[0].boxes

        trees = []
        coffees = []
        class_counts = []
        for box in boxes:
            cls = int(box.cls[0])
            conf = float(box.conf[0])

            label = class_names[cls]
            labels = [class_names[int(b.cls[0])] for b in boxes]

            # Counters for each class
            coffees = sum(1 for l in labels if l in coffee_labels)
            inmature = sum(1 for l in labels if l == "inmature")
            mature = sum(1 for l in labels if l =="mature")
            overmature = sum(1 for l in labels if l =="overmature")
            pools = sum(1 for l in labels if l == "pool")
        
        print(f"Number of coffees: {coffees}")
        print(f"Inmatures beans: {inmature}" )
        print(f"Mature beans: {mature}")
        print(f"Overmature beans: {overmature}" )
        print(f"Number of pools: {pools}")
        
        if coffees == 16:
            print("Tree is fully detected!")
        else:
            print("Tree is not fully detected")

        annotated_img = results[0].plot()
        cv2.imshow("YOLO inference", annotated_img)
        cv2.waitKey(0) 
        cv2.destroyAllWindows()  

  
    else:
        print("No se pudo cargar la imagen.")
