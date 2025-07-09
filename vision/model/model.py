from ultralytics import YOLO
import cv2

# model = YOLO("yolov8m.pt")

# model.train(
#     data="vision/model/config.yaml",
#     epochs=150,
#     patience=15,
#     save_period=20,
#     name="larc-model",
# )


model = YOLO('runs/detect/larc-model6/weights/best.pt')
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

annotated_img_resized = resize_image(annotated_img)

while True:
    cv2.imshow("YOLO inference", annotated_img_resized)
    if cv2.waitKey(1) & 0xFF == 27:  # Press ESC to exit
        break
cv2.destroyAllWindows()