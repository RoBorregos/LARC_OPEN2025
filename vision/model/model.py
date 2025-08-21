from ultralytics import YOLO
import cv2

# TODO: TRAIN TO STOP DETECTING GREEN BEANS

model = YOLO('vision/model/best.pt')

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
    annotated_img_resized = resize_image(annotated_img)

    cv2.imshow("YOLO inference", annotated_img_resized)
    if cv2.waitKey(1) & 0xFF == 27:  # ESC to exit
        break

cap.release()
cv2.destroyAllWindows()