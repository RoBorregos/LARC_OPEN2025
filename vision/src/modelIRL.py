import numpy as np
np.bool = bool  # Parche para evitar error con TensorRT y NumPy recientes

from ultralytics import YOLO
import cv2
from collections import Counter
import torch

# ------------------------------
# Configuración del modelo
# ------------------------------
model = YOLO("model/nanoModel.engine", task='detect')  # Modelo TensorRT

# Lista externa de clases (modifica según tu modelo)
class_names = ["inmature", "mature", "overmature", "red_benefit", "blue_benefit"]

# ------------------------------
# Configuración de la cámara
# ------------------------------
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 480)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

# ------------------------------
# Loop principal
# ------------------------------
while True:
    ret, frame = cap.read()
    if not ret:
        continue

    results = model(frame, verbose=False)

    counts = Counter()

    for r in results:
        for box in r.boxes:
            cls_idx = int(box.cls)
            cls_name = class_names[cls_idx]
            counts[cls_name] += 1

    # Imprimir conteos por clase
    print(dict(counts))
    torch.cuda.empty_cache()

    # Salir con ESC
    if cv2.waitKey(1) == 27:
        break

cap.release()
cv2.destroyAllWindows()