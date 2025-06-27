import cv2

for i in range(10):  # try ports 0-9
    cap = cv2.VideoCapture(i)
    if cap.isOpened():
        ret, frame = cap.read()
        if ret and frame is not None and frame.size > 0:
            print(f"Camera {i} has signal.")
        else:
            print(f"Camera {i} opened but no signal.")
        cap.release()
    else:
        print(f"Camera {i} not available.")