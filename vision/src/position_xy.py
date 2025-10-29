import cv2
from ultralytics import YOLO

def main():
    model = YOLO("model/nanoModel.pt")  # Cambia la ruta a tu modelo .pt
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("No se pudo abrir la cámara")
        return

    print("Presiona ESC para salir")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Inferencia
        results = model(frame, verbose=False)
        annotated = frame.copy()

        for r in results:
            boxes = r.boxes  # Contiene todas las detecciones
            for box in boxes:
                x1, y1, x2, y2 = box.xyxy[0]  # Coordenadas esquina bbox
                cx = int((x1 + x2) / 2)       # Centro X
                cy = int((y1 + y2) / 2)       # Centro Y

                # Dibujar caja y centro
                cv2.rectangle(annotated, (int(x1), int(y1)), (int(x2), int(y2)), (0,255,0), 2)
                cv2.circle(annotated, (cx, cy), 4, (0,0,255), -1)

                print(f"Centro XY: ({cx}, {cy})")

        cv2.imshow("Detección", annotated)
        if cv2.waitKey(1) & 0xFF == 27:  # ESC para salir
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
