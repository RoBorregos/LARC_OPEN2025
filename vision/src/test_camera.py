import cv2

cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("❌ No se pudo abrir la cámara /dev/video1")
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("⚠️ No se pudo leer frame")
        break
    cv2.imshow("Camera", frame)
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()
cv2.destroyAllWindows()
