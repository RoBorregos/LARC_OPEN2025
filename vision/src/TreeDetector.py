import cv2
import numpy as np

class TreeDetector:
    def __init__(self):
        # HSV bounds for brown detection
        self.color = ([0, 0, 0], [179, 80, 51])

        # Real-world distance (cm) and camera focal length (px)
        self.TRUNK_WIDTH_CM = 21.0
        self.BASE_WIDTH_CM = 45.0
        self.APPROX_FOCAL_PX = 620.0 # Lower REGO

    def _calculate_distance(self, width_px, is_trunk):
        # Calculate distance using real-world width and focal length
        real_width_cm = self.TRUNK_WIDTH_CM if is_trunk else self.BASE_WIDTH_CM
        return (real_width_cm * self.APPROX_FOCAL_PX) / width_px

    def _detect_largest_object(self, frame):
        # Convert to HSV and find largest brown contour
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        lower_color = np.array(self.color[0])
        upper_color = np.array(self.color[1])
        mask = cv2.inRange(hsv, lower_color, upper_color)
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        contours = [c for c in contours if cv2.contourArea(c) > 500]

        if not contours:
            return None

        largest_contour = max(contours, key=cv2.contourArea)
        x, y, w, h = cv2.boundingRect(largest_contour)
        is_trunk = h > w

        return (x, y, w, h), is_trunk, mask

    def run(self, frame, show_on_screen=False):
        # Detect and measure the tree object
        if frame is None:
            return False, None, False, (0.0, 0.0)

        detection = self._detect_largest_object(frame)
        if detection is None:
            return False, None, False, (0.0, 0.0)

        (x, y, w, h), is_trunk, mask = detection
        distance_cm = self._calculate_distance(w, is_trunk)

        frame_height, frame_width = frame.shape[:2]
        too_close = (w / frame_width) >= 0.5

        # Calculate normalized offset (-1 to 1)
        obj_center_x = x + w / 2
        obj_center_y = y + h / 2
        offset_x = ((obj_center_x - frame_width / 2) / (frame_width / 2))
        offset_y = ((obj_center_y - frame_height / 2) / (frame_height / 2))
        # Clip to range just in case
        offset_x = np.clip(offset_x, -1.0, 1.0)
        offset_y = np.clip(offset_y, -1.0, 1.0)

        if show_on_screen:
            # Display bounding box and distance info
            display_frame = frame.copy()
            cv2.rectangle(display_frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            cv2.putText(display_frame, f"W: {w}px", (x, y - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            cv2.putText(display_frame, f"D: {distance_cm:.1f}cm", (x, y - 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            
            status = "TOO CLOSE" if too_close else "OK"
            cv2.putText(display_frame, f"Status: {status}", (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            
            cv2.putText(display_frame, f"Offset X: {offset_x:.2f}", (10, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)
            cv2.putText(display_frame, f"Offset Y: {offset_y:.2f}", (10, 90),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)

            # Show images
            cv2.imshow("Tree Detection", display_frame)
            cv2.imshow("Brown Mask", mask)
            cv2.waitKey(1)

        return True, distance_cm, too_close, (offset_x, offset_y)

'''
# for testing
if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    detector = TreeDetector()

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            detected, distance, too_close, offset  = detector.run(frame, show_on_screen=True)
            distance_str = f"{distance:.1f} cm" if distance is not None else "N/A"
            print(f"Detected: {detected}, Distance: {distance_str}, Too close: {too_close}, Offset: ({offset[0]:.2f}, {offset[1]:.2f})")
    except KeyboardInterrupt:
        pass
    finally:
        cap.release()
        cv2.destroyAllWindows()
'''