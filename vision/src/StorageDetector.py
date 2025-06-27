import cv2
import numpy as np

class StorageDetector:
    def __init__(self):
        # Default HSV color bounds for red and blue detection
        self.red_color = ([156, 92, 98], [179, 255, 255])
        self.blue_color = ([0, 0,0], [179, 65, 107])
        
        # Real-world distance (cm) and camera focal length (px)
        self.VERTICAL_DISTANCE_CM = 9.0
        self.APPROX_FOCAL_PX = 620.0 # Lower REGO
        
        # Detection thresholds
        self.X_THRESHOLD = 50
        self.MIN_CONTOUR_AREA = 500
        self.MAX_RELIABLE_AREA_RATIO = 0.4
        self.MIN_DISTANCE_CM = 10.0
        self.BORDER_THRESHOLD = 10

    def _check_object_position(self, frame, contour):
        # Check if object touches top/bottom borders or is too large
        height, width = frame.shape[:2]
        frame_area = width * height
        contour_area = cv2.contourArea(contour)

        is_touching = False
        for point in contour[:, 0, :]:
            x, y = point
            if y <= self.BORDER_THRESHOLD or y >= height - self.BORDER_THRESHOLD:
                is_touching = True
                break

        area_ratio = contour_area / frame_area
        is_too_large = area_ratio > self.MAX_RELIABLE_AREA_RATIO

        reason = ""
        if is_touching:
            reason = "Object touches top or bottom border"
        elif is_too_large:
            reason = f"Object too large ({area_ratio:.0%} of frame)"

        return is_touching, is_too_large, reason

    def _calculate_distance(self, height_px):
        # Calculate distance using real-world width and focal length
        return (self.VERTICAL_DISTANCE_CM * self.APPROX_FOCAL_PX) / height_px

    def _detect_largest_box(self, frame, color_range):
        # Convert to HSV and find largest box contour based on color range
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        lower_color = np.array(color_range[0])
        upper_color = np.array(color_range[1])
        mask = cv2.inRange(hsv, lower_color, upper_color)
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if not contours:
            return None, mask

        largest_contour = max(contours, key=cv2.contourArea)
        if cv2.contourArea(largest_contour) < self.MIN_CONTOUR_AREA:
            return None, mask

        epsilon = 0.02 * cv2.arcLength(largest_contour, True)
        vertices = cv2.approxPolyDP(largest_contour, epsilon, True)

        if len(vertices) > 6:
            vertices = vertices[:6]

        return (largest_contour, vertices), mask

    def _find_vertical_dimension(self, vertices):
        # Find the longest vertical line segment among vertices
        max_vertical = 0
        best_line = None

        for i in range(len(vertices)):
            for j in range(i + 1, len(vertices)):
                x1, y1 = vertices[i][0]
                x2, y2 = vertices[j][0]
                if abs(x1 - x2) < self.X_THRESHOLD:
                    vertical_length = abs(y1 - y2)
                    if vertical_length > max_vertical:
                        max_vertical = vertical_length
                        best_line = ((x1, y1), (x2, y2))

        return (best_line, max_vertical) if max_vertical > 0 else None

    def _calculate_normalized_offset(self, frame, contour):
        M = cv2.moments(contour)
        if M['m00'] == 0:
            return (0.0, 0.0), (0, 0)
        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])
        height, width = frame.shape[:2]
        offset_x = (cx - width / 2) / (width / 2)
        offset_y = (cy - height / 2) / (height / 2)
        offset_x = max(min(offset_x, 1.0), -1.0)
        offset_y = max(min(offset_y, 1.0), -1.0)
        return (offset_x, offset_y), (cx, cy)

    def analyze_frame(self, frame, color_range, box_type):
        # Detect box and analyze its size and position based on color
        detection, mask = self._detect_largest_box(frame, color_range)
        if not detection:
            return None, mask

        contour, vertices = detection
        touches_border, is_too_large, reason = self._check_object_position(frame, contour)
        vertical_data = self._find_vertical_dimension(vertices)
        offset_norm, center_point = self._calculate_normalized_offset(frame, contour)

        result = {
            'contour': contour,
            'vertices': vertices,
            'touches_border': touches_border,
            'is_too_large': is_too_large,
            'warning': reason,
            'line_points': None,
            'pixel_height': None,
            'distance_cm': None,
            'show_distance': False,
            'normalized_offset': offset_norm,
            'center_point': center_point,
            'box_type': box_type
        }

        if not touches_border and vertical_data:
            line_points, pixel_height = vertical_data
            distance_cm = self._calculate_distance(pixel_height)
            result.update({
                'line_points': line_points,
                'pixel_height': pixel_height,
                'distance_cm': distance_cm,
                'show_distance': not is_too_large
            })

            if distance_cm < self.MIN_DISTANCE_CM:
                result['warning'] = f"Too close ({distance_cm:.1f}cm)"

        return result, mask

    def run(self, frame, only_red=False, only_blue=False, show_on_screen=False):
        if frame is None:
            print("No frame provided")
            return False, None, False, "", (0.0, 0.0), ""

        red_result = None
        blue_result = None
        masks = []

        if not only_blue:
            red_result, red_mask = self.analyze_frame(frame, self.red_color, 'SOBREMADURO')
            masks.append(red_mask)

        if not only_red:
            blue_result, blue_mask = self.analyze_frame(frame, self.blue_color, 'MADURO')
            masks.append(blue_mask)

        combined_mask = cv2.bitwise_or(masks[0], masks[1]) if len(masks) > 1 else (masks[0] if masks else None)

        # Select biggest object
        selected_result = None
        if red_result and blue_result:
            area_red = cv2.contourArea(red_result['contour'])
            area_blue = cv2.contourArea(blue_result['contour'])
            selected_result = red_result if area_red >= area_blue else blue_result
        else:
            selected_result = red_result or blue_result

        if show_on_screen and selected_result:
            display_frame = frame.copy()
            res = selected_result
            for point in res['vertices']:
                x, y = point[0]
                color = (0, 0, 255) if res['box_type'] == 'SOBREMADURO' else (255, 0, 0)
                cv2.circle(display_frame, (x, y), 5, color, -1)

            contour_color = (0, 255, 0) if res['box_type'] == 'SOBREMADURO' else (255, 255, 0)
            cv2.drawContours(display_frame, [res['vertices']], -1, contour_color, 2)

            if res['line_points'] and res['distance_cm'] is not None:
                p1, p2 = res['line_points']
                cv2.line(display_frame, p1, p2, (0, 0, 255), 2)
                mid_x = (p1[0] + p2[0]) // 2
                mid_y = (p1[1] + p2[1]) // 2
                text_color = (0, 0, 255) if res['box_type'] == 'SOBREMADURO' else (255, 255, 255)
                cv2.putText(display_frame, f"{res['distance_cm']:.1f} cm", (mid_x, mid_y),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, text_color, 2)

            if res.get('center_point'):
                cv2.circle(display_frame, res['center_point'], 5, (255, 255, 0), -1)

            if res.get('warning'):
                cv2.putText(display_frame, f"WARNING: {res['warning']}", (10, 30),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

            cv2.putText(display_frame, f"Box Type: {res['box_type']}", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.7,
                        (0, 0, 255) if res['box_type'] == 'SOBREMADURO' else (255, 255, 255), 2)

            cv2.imshow("Storage Detection", display_frame)
            if combined_mask is not None:
                cv2.imshow("Red and/or Blue Mask", combined_mask)
            cv2.waitKey(1)

        if not selected_result:
            return False, None, False, "", (0.0, 0.0), ""

        return (
            True,
            selected_result['distance_cm'],
            selected_result['warning'].startswith("Too close"),
            selected_result.get('warning', ""),
            selected_result['normalized_offset'],
            selected_result['box_type']
        )

'''
# for testing
if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    detector = StorageDetector()

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break

            frame = cv2.rotate(frame, cv2.ROTATE_180)
            detected, distance, too_close, warning, offset, box_type = detector.run(frame, only_blue=True, only_red=True, show_on_screen=True)
            print(f"Detected: {detected}, Distance: {distance}, Too close: {too_close}, Offset: {offset}, Warning: {warning}, Box Type: {box_type}")
    except KeyboardInterrupt:
        print("Interrupted by user.")
    finally:
        cap.release()
        detector.cleanup()
'''
