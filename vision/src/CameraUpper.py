import cv2
import numpy as np
import serial
import time

class CameraUpper:
    def __init__(self, port=None, camera_index=0, min_area=3800, max_area=50000, max_diameter=250):
        self.colors = {
            "Red": ([0, 92, 98], [12, 255, 255]),
            "Blue": ([90, 88, 159], [146, 255, 255]),
            "Orange": ([0, 56, 102], [42, 255, 255]),
            "Yellow": ([25, 52, 67], [40, 255, 255]),
            "Black": ([0, 0, 0], [180, 65, 107]),
        }
        self.color_bgr_values = {
            "Red": (0, 0, 255),
            "Blue": (255, 0, 0),
            "Orange": (0, 165, 255),
            "Yellow": (0, 255, 255),
            "Black": (0, 0, 0),
        }
        self.color_to_type = {
            "Red": "MADURO",
            "Orange": "MADURO",
            "Yellow": "MADURO",
            "Blue": "SOBREMADURO",
            "Black": "SOBREMADURO"
        }
        self.kernel = np.ones((5, 5), "uint8")
        self.min_area = min_area
        self.max_area = max_area
        self.max_diameter = max_diameter
        self.center_frame = (0, 0)

        # Initialize serial port if provided
        self.serial = None
        if port:
            try:
                self.serial = serial.Serial(port, 9600, timeout=1)
                print(f"✅ Serial connected to {port}")
            except serial.SerialException as e:
                print(f"❌ Serial connection failed: {e}")

        # Initialize camera
        self.cap = cv2.VideoCapture(camera_index)
        if not self.cap.isOpened():
            print(f"❌ Could not open webcam at index {camera_index}")
        else:
            print(f"✅ Camera opened at index {camera_index}")

    def send_data(self, data):
        print(f"[RESULT] {data}")
        if self.serial and self.serial.is_open:
            self.serial.write((data + "\n").encode())

    def normalize(self, val, max_val):
        return (2 * (val - (max_val / 2)) / max_val)

    def process_frame(self, frame):
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        height, width, _ = frame.shape
        self.center_frame = (width // 2, height // 2)

        detected_objects = []

        for color_name, (lower, upper) in self.colors.items():
            lower_np = np.array(lower, np.uint8)
            upper_np = np.array(upper, np.uint8)
            mask = cv2.inRange(hsv_frame, lower_np, upper_np)
            mask = cv2.dilate(mask, self.kernel)

            contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                area = cv2.contourArea(cnt)
                x, y, w, h = cv2.boundingRect(cnt)
                diameter = w

                if area < self.min_area or area > self.max_area or diameter > self.max_diameter:
                    continue

                center = self.get_object_center(cnt)

                detected_objects.append({
                    'contour': cnt,
                    'color': color_name,
                    'center': center,
                    'area': area,
                    'diameter': diameter
                })

        return detected_objects

    def get_object_center(self, contour):
        hull = cv2.convexHull(contour)
        M = cv2.moments(hull)
        if M["m00"] != 0:
            return (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        else:
            return (0, 0)

    def get_detection_data(self, frame):
        objects = self.process_frame(frame)

        if not objects:
            return None, []

        selected_object = min(objects, key=lambda obj: obj['center'][0])

        object_center = selected_object['center']
        offset_x = object_center[0] - self.center_frame[0]
        offset_y = object_center[1] - self.center_frame[1]
        area = selected_object['area']
        color_name = selected_object['color']
        diameter = selected_object['diameter']
        type_name = self.color_to_type.get(color_name, "UNKNOWN")

        return (object_center, offset_x, offset_y, area, type_name, diameter), objects

    def run(self, show_on_screen=False):
        ret, frame = self.cap.read()
        if not ret:
            print("❌ Failed to read frame")
            return

        selected_data, all_objects = self.get_detection_data(frame)

        if show_on_screen:
            for obj in all_objects:
                color = self.color_bgr_values.get(obj['color'], (255, 255, 255))
                center = obj['center']
                area = obj['area']
                diameter = obj['diameter']
                x, y, w, h = cv2.boundingRect(obj['contour'])

                cv2.drawContours(frame, [obj['contour']], -1, color, 2)
                cv2.circle(frame, center, 5, color, -1)
                cv2.putText(frame, f"Area: {int(area)}", (center[0] + 10, center[1] - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                cv2.putText(frame, f"Diameter: {diameter} px", (center[0] + 10, center[1] + 20),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                cv2.line(frame, (x, y + h // 2), (x + w, y + h // 2), (0, 255, 0), 2)

        if selected_data:
            object_center, offset_x, offset_y, area, type_name, diameter = selected_data
            result_str = f"Type: {type_name}, Area: {area:.0f}, OffsetX: {offset_x}, OffsetY: {offset_y}, Diameter: {diameter}"
            self.send_data(result_str)

            if show_on_screen:
                cv2.circle(frame, object_center, 7, (255, 255, 255), -1)
                cv2.putText(frame, result_str, (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)

        if show_on_screen:
            cv2.circle(frame, self.center_frame, 5, (0, 255, 0), -1)
            cv2.imshow("CamUpper View", frame)
            if cv2.waitKey(1) == 27:  # ESC
                self.cap.release()
                cv2.destroyAllWindows()

'''
# Example usage
if __name__ == "__main__":
    camera = CamUpper(port='/dev/ttyACM0', camera_index=0)
    while True:
        camera.run(show_on_screen=True)
        time.sleep(1)
'''

# for testing
if __name__ == "__main__":
    camera = CameraUpper(port='/dev/ttyACM0', camera_index=0)
    while True:
        camera.run(show_on_screen=True)
