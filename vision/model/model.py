from ultralytics import YOLO
import cv2

model = YOLO("vision/model/runs/detect/larc-model8/weights/best.pt")

class_names = model.names  

IGNORE_CLASS = "green bean" 

def resize_image(img, max_width=800, max_height=600):
    height, width = img.shape[:2]
    scale = min(max_width / width, max_height / height)
    if scale < 1:
        new_size = (int(width * scale), int(height * scale))
        return cv2.resize(img, new_size, interpolation=cv2.INTER_AREA)
    return img

cap = cv2.VideoCapture(2)
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

    annotated_img_resized = resize_image(annotated_img)
    cv2.imshow("YOLO inference", annotated_img_resized)

    if cv2.waitKey(1) & 0xFF == 27: 
        break

cap.release()
cv2.destroyAllWindows()
