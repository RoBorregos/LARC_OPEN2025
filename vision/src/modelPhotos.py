import time
from ultralytics import YOLO
import cv2
import math

model = YOLO("vision/model/mediumModel.pt")
class_names = model.names

cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()

tree_detection_tolerance = 0.8 
coffee_labels = ["mature", "overmature", "inmature"]

image = cv2.imread("vision/imagesTest/IMG_2503.JPG")
image = cv2.resize(image, (640, 480)) if image is not None else None
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

            results = model(frame)
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

            # for label in coffee_labels:
            #     print(f"Cantidad de '{label}': {labels.count(label)}")
        
        print(f"Number of coffees: {coffees}")
        print(f"Inmatures beans: {inmature}" )
        print(f"Mature beans: {mature}")
        print(f"Overmature beans: {overmature}" )

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
