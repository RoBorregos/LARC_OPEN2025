#!/bin/bash
from ultralytics import YOLO

model = YOLO("yolov8m.pt")

results = model.train(
    data=r"vision\model\data.yaml",
    epochs=150,
    imgsz=640,
    batch=8,
    perspective=0.0005,
    scale=0.4,
    translate=0.04,
    degrees=5,
    shear=1,
    hsv_s=0.5,
    hsv_v=0.3,
    flipud=0.2,
    patience=15,
    save_period=20,
    name="larc-model",
)

# model.train(resume=True)

model.val()

"""
model = YOLO('/path/to/best.pt')
results = model('/path/to/test/image.jpg')
annotated_img = results[0].plot()

import cv2
cv2.imshow("YOLO inference", annotated_img)
cv2.waitKey(5000)
cv2.destroyAllWindows()
"""