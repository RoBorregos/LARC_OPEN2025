from ultralytics import YOLO
import cv2

model = YOLO("yolov8m.pt")

# model.train(
#     data="vision/model/config.yaml",
#     epochs=150,
#     patience=15,
#     save_period=20,
#     name="larc-model",
# )

model = YOLO('runs/detect/larc-model/weights/best.pt')
results = model('vision/model/dataset/val')
accurate = model.val()
annotated_img = results[0].plot()

def resize_image(img, max_width=800, max_height=600):
    height, width = img.shape[:2]
    scale = min(max_width / width, max_height / height)
    if scale < 1:
        new_size = (int(width * scale), int(height * scale))
        return cv2.resize(img, new_size, interpolation=cv2.INTER_AREA)
    return img

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    results = model(frame)
    annotated_img = results[0].plot()
    annotated_img_resized = resize_image(annotated_img)

    cv2.imshow("YOLO inference", annotated_img_resized)
    if cv2.waitKey(1) & 0xFF == 27:  # Presiona ESC para salir
        break

cap.release()
cv2.destroyAllWindows()