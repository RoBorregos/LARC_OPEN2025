import cv2
from ultralytics import YOLO

def main():
    model = YOLO("model/nanoModel.pt")  # Cambia la ruta a tu modelo .pt
    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("No se pudo abrir la cámara")
        return

    cv2.namedWindow("Detección", cv2.WINDOW_NORMAL)
    print("Presiona ESC para salir")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        results = model(frame, verbose=False)
        annotated = frame.copy()

        for r in results:
            for box in r.boxes:
                x1, y1, x2, y2 = box.xyxy[0]
                cx = int((x1 + x2) / 2)
                cy = int((y1 + y2) / 2)

                # Dibujar
                cv2.rectangle(annotated, (int(x1), int(y1)), (int(x2), int(y2)), (0,255,0), 2)
                cv2.circle(annotated, (cx, cy), 4, (0,0,255), -1)

                print(f"Centro XY: ({cx}, {cy})")

        # Mostrar SIEMPRE en la misma ventana
        cv2.imshow("Detección", annotated)

        if cv2.waitKey(1) & 0xFF == 27:  # tecla ESC
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()