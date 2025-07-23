import math
import cv2
import numpy as np
from typing import Tuple, Optional, List, Dict

class Camera:
    def __init__(self, camera1: int, camera2: int):
        self.camera1 = camera1
        self.camera2 = camera2
        self.cap1 = None
        self.cap2 = None
        self._initialize_cameras()
    
    #* Color detection implementation
    BEAN_COLORS: Dict[str, Tuple[np.ndarray, np.ndarray]] = {
        'red':    (np.array([0, 70, 50]), np.array([10, 255, 255])),
        'red2':   (np.array([170, 70, 50]), np.array([180, 255, 255])),
        'blue':   (np.array([100, 150, 0]), np.array([140, 255, 255])),
        'yellow': (np.array([20, 100, 100]), np.array([30, 255, 255])),
        'orange': (np.array([10, 100, 20]), np.array([25, 255, 255])),
        'black':  (np.array([0, 0, 0]), np.array([179, 255, 44]))
    }

    OBSTACLE_COLORS: Dict[str, Tuple[np.ndarray, np.ndarray]] = {
        'pool':   (np.array([100, 150, 0]), np.array([140, 255, 255])),
        'red_container': (np.array([0, 70, 50]), np.array([10, 255, 255])),
        'blue_container': (np.array([100, 150, 0]), np.array([140, 255, 255])),
        'tree': (np.array([34, 54, 0]), np.array([100, 255, 255]))
    }

    COLOR_BGR: Dict[str, Tuple[int, int, int]] = {
        'red':    (0, 0, 255),
        'blue':   (255, 0, 0),
        'black':  (0, 0, 0),
        'yellow': (0, 255, 255),
        'orange': (0, 165, 255)
    }

    BEAN_TYPES = {
        'RIPE': ['red', 'yellow', 'orange', 'blue', 'red2'],
        'OVER_RIPE': ['black', 'brown']
    }

    GREEN_HSV = (np.array([34, 54, 0]), np.array([100, 255, 255]))

    def _initialize_cameras(self) -> None:
        '''Initialize camera connections.'''
        try:
            self.cap1 = cv2.VideoCapture(self.camera1)
            self.cap2 = cv2.VideoCapture(self.camera2)
            #if not self.cap1.isOpened() or not self.cap2.isOpened():
            if not self.cap1.isOpened():
                raise RuntimeError("Could not open one or both cameras")
        except Exception as e:
            print(f"Camera initialization error: {e}")
            self.release()
            raise

    def release(self) -> None:
        '''Release camera resources.'''
        if self.cap1 and self.cap1.isOpened():
            self.cap1.release()
        if self.cap2 and self.cap2.isOpened():
            self.cap2.release()
        cv2.destroyAllWindows()

    def get_frame(self, camera_num: int = 1) -> Optional[np.ndarray]:
        '''Get frame from specified camera.'''
        cap = self.cap1 if camera_num == 1 else self.cap2
        if cap and cap.isOpened():
            ret, frame = cap.read()
            return frame if ret else None
        return None

    ###   Camera 1 Logic   ###
    def _get_color_mask(self, img_hsv: np.ndarray, color_name: str, color_dict: Dict) -> Optional[np.ndarray]:
        '''Get mask for specific color.'''
        if color_name == 'red':
            mask1 = cv2.inRange(img_hsv, color_dict['red'][0], color_dict['red'][1])
            mask2 = cv2.inRange(img_hsv, color_dict['red2'][0], color_dict['red2'][1])
            return cv2.bitwise_or(mask1, mask2)
        return cv2.inRange(img_hsv, color_dict[color_name][0], color_dict[color_name][1])

    def _get_largest_green_area(self, img_hsv: np.ndarray) -> np.ndarray:
        '''Get mask for the largest green area in the image.'''
        mask_green = cv2.inRange(img_hsv, self.GREEN_HSV[0], self.GREEN_HSV[1])
        contours, _ = cv2.findContours(mask_green, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        
        if not contours:
            return np.zeros(img_hsv.shape[:2], dtype=np.uint8)
        
        # Find the largest contour
        largest_contour = max(contours, key=cv2.contourArea)
        mask = np.zeros(img_hsv.shape[:2], dtype=np.uint8)
        cv2.drawContours(mask, [largest_contour], -1, 255, -1)
        return mask

    def _detect_color_objects(self, img_hsv: np.ndarray, mask: np.ndarray, img_draw: np.ndarray, color_dict: Dict) -> List[Tuple[float, float, float, str]]:
        '''Detect colored objects in the frame using specified color dictionary.'''
        centroids = []

        for color_name in color_dict:
            if color_name == 'red2':
                continue

            mask_color = self._get_color_mask(img_hsv, color_name, color_dict)
            if mask_color is None:
                continue

            mask_color = cv2.bitwise_and(mask_color, mask)
            contours, _ = cv2.findContours(mask_color, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            for cnt in contours:
                area = cv2.contourArea(cnt)
                if area > 200:
                    (cx, cy), radius = cv2.minEnclosingCircle(cnt)

                    # Check for nearby objects
                    too_close = any(
                        math.hypot(cx - x_prev, cy - y_prev) < 1.1 * 2 * math.pi * r_prev
                        for (x_prev, y_prev, r_prev, _) in centroids
                    )

                    if not too_close:
                        centroids.append((cx, cy, radius, color_name))
                        margin_radius = 0.1 * 2 * math.pi * radius
                        self._draw_dotted_circle(
                            img_draw, (int(cx), int(cy)), margin_radius, 
                            (255, 255, 255), thickness=1, gap=8
                        )

        return centroids

    def _process_frame(self, img: np.ndarray, use_green_mask: bool = False) -> Tuple[np.ndarray, float, float]:
        '''Process a single frame and return annotated image with offsets.'''
        img_draw = img.copy()
        img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        
        if use_green_mask:
            # Get the largest green area mask
            mask = self._get_largest_green_area(img_hsv)
            
            # Find contours of the green area
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            
            # Draw the green area contour on the image
            if contours:
                # Draw the largest contour (green area boundary)
                cv2.drawContours(img_draw, contours, -1, (0, 255, 0), 2)
                
                # Optional: Fill the green area with semi-transparent green
                green_visual = np.zeros_like(img)
                cv2.drawContours(green_visual, contours, -1, (0, 255, 0), -1)
                img_draw = cv2.addWeighted(img_draw, 1, green_visual, 0.2, 0)
        else:
            mask = np.ones(img_hsv.shape[:2], dtype=np.uint8) * 255

        centroids = self._detect_color_objects(img_hsv, mask, img_draw, 
                                            self.BEAN_COLORS if use_green_mask else self.OBSTACLE_COLORS)

        # Draw detected objects
        for (cx, cy, radius, color_name) in centroids:
            color = self.COLOR_BGR.get(color_name, (255, 255, 255))
            cv2.circle(img_draw, (int(cx), int(cy)), int(radius), color, 2)
            cv2.putText(img_draw, color_name, (int(cx), int(cy) - 10), 
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

        offset_x, offset_y = self._draw_annotations(img_draw, centroids)
        return img_draw, offset_x, offset_y

    def detect_bean(self) -> Tuple[bool, str, float, float]:
        '''Detect beans using camera1 only within the largest green area.'''
        frame = self.get_frame(1)
        if frame is None:
            return False, "NONE", 0.0, 0.0

        img_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        green_mask = self._get_largest_green_area(img_hsv)
        
        # Detect all color objects within green area
        centroids = self._detect_color_objects(
            img_hsv, 
            green_mask,
            frame.copy(),
            self.BEAN_COLORS
        )
        
        if not centroids:
            return False, "NONE", 0.0, 0.0

        # Extract just the color names from centroids
        detected_colors = {color for (_, _, _, color) in centroids}
        
        # Classify based on detected colors
        bean_type = ("OVER_RIPE" if any(color in self.BEAN_TYPES["OVER_RIPE"] 
                    for color in detected_colors) else
                    "RIPE" if any(color in self.BEAN_TYPES["RIPE"]
                    for color in detected_colors) else
                    "UNKNOWN")

        # Calculate offsets from the first bean
        cx, cy, _, _ = centroids[0]
        h, w = frame.shape[:2]
        offset_x = (cx - w // 2) / (w // 2)
        offset_y = (cy - h // 2) / (h // 2)

        return True, bean_type, offset_x, offset_y

    ###  Camera 2 Logic   ###
    def detect_obstacle(self) -> Tuple[bool, str, float, float]:
        '''Detect obstacles using camera2 in the full frame and return the obstacle type.'''
        frame = self.get_frame(2)  # camera2 only
        if frame is None:
            return False, "NONE", 0.0, 0.0

        img_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        full_mask = np.ones(frame.shape[:2], dtype=np.uint8) * 255
        
        # Detect all obstacle objects in full frame
        centroids = self._detect_color_objects(
            img_hsv,
            full_mask,
            frame.copy(),
            self.OBSTACLE_COLORS
        )
        
        if not centroids:
            return False, "NONE", 0.0, 0.0

        # Get the closest obstacle (lowest y-coordinate)
        closest_obstacle = max(centroids, key=lambda c: -c[1])  # Max because y increases downward
        _, _, _, obstacle_type = closest_obstacle

        # Calculate offsets from center
        cx, cy, _, _ = closest_obstacle
        h, w = frame.shape[:2]
        offset_x = (cx - w // 2) / (w // 2)
        offset_y = (cy - h // 2) / (h // 2)

        return True, obstacle_type.upper(), offset_x, offset_y

    ### Camera Display Logic ###
    def show_camera(self, camera1: bool = True, camera2: bool = False) -> None:
        '''Display camera feed with appropriate processing.'''
        if camera1:
            frame = self.get_frame(1)
            if frame is not None:
                # Show bean detection view (with green mask)
                processed_frame, _, _ = self._process_frame(frame, use_green_mask=True)
                cv2.imshow("Camera 1 - Bean Detection", processed_frame)
        
        if camera2:
            frame = self.get_frame(2)
            if frame is not None:
                # Show obstacle detection view
                processed_frame, _, _ = self._process_frame(frame, use_green_mask=False)
                cv2.imshow("Camera 2 - Obstacle Detection", processed_frame)
        
        cv2.waitKey(1)

    def _process_and_show(self, camera_num: int, window_name: str) -> None:
        '''Internal method to process and show camera feed.'''
        frame = self.get_frame(camera_num)
        if frame is not None:
            # For display purposes, we'll show the full frame with obstacles
            processed_frame, _, _ = self._process_frame(frame, use_green_mask=False)
            cv2.imshow(window_name, processed_frame)
            cv2.waitKey(1)
    
    @staticmethod
    def _draw_dotted_circle(img: np.ndarray, center: Tuple[int, int], radius: int, 
                          color: Tuple[int, int, int], thickness: int = 1, gap: int = 10) -> None:
        '''Draw a dotted circle on the image.'''
        for angle in range(0, 360, gap * 2):
            x1 = int(center[0] + radius * math.cos(math.radians(angle)))
            y1 = int(center[1] + radius * math.sin(math.radians(angle)))
            x2 = int(center[0] + radius * math.cos(math.radians(angle + gap)))
            y2 = int(center[1] + radius * math.sin(math.radians(angle + gap)))
            cv2.line(img, (x1, y1), (x2, y2), color, thickness)
    
    def _draw_annotations(self, img: np.ndarray, centroids: List[Tuple[float, float, float, str]]) -> Tuple[float, float]:
        '''Draw annotations and calculate offsets.'''
        if not centroids:
            return 0.0, 0.0

        # Find bottom-left centroid
        target = sorted(centroids, key=lambda c: (-c[1], c[0]))[0]
        cx, cy, _, _ = target
        cv2.circle(img, (int(cx), int(cy)), 4, (255, 255, 255), -1)

        h, w = img.shape[:2]
        offset_x = (cx - w // 2) / (w // 2)
        offset_y = (cy - h // 2) / (h // 2)
        
        # Draw offset text
        offset_text = f"Offset X: {offset_x:.2f}  Y: {offset_y:.2f}"
        text_size, _ = cv2.getTextSize(offset_text, cv2.FONT_HERSHEY_SIMPLEX, 0.7, 2)
        text_x = (w - text_size[0]) // 2
        cv2.putText(img, offset_text, (text_x, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)

        # Draw connections between objects at similar heights
        thres_y = 10
        for (ix, iy, _, _) in centroids:
            if abs(iy - cy) <= thres_y and (ix, iy) != (cx, cy):
                cv2.circle(img, (int(ix), int(iy)), 4, (255, 255, 0), -1)
                cv2.line(img, (int(cx), int(cy)), (int(ix), int(iy)), (200, 200, 200), 1)

        return offset_x, offset_y

if __name__ == '__main__':
    cam = Camera(camera1=2, camera2=0)  # Assuming camera1 is device 2, camera2 is device 0
    
    try:
        while True:
            # Detect beans from camera1
            bean_detected, bean_type, bx, by = cam.detect_bean()
            # Detect obstacles from camera2
            #obstacle_detected, obstacle_type, ox, oy = cam.detect_obstacle()
            
            print(f"Bean: {bean_detected}, Type: {bean_type}, Offsets: X: {bx:.2f}, Y: {by:.2f}")
            #print(f"Obstacle: {obstacle_detected}, Type: {obstacle_type}, Offsets: X: {ox:.2f}, Y: {oy:.2f}")
            
            # Show both camera feeds
            cam.show_camera(camera1=True, camera2=False)
            
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
                
    except KeyboardInterrupt:
        print("Program interrupted by user")
    finally:
        cam.release()
        cv2.destroyAllWindows()