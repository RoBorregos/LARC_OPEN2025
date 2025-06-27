import serial
import time
from enum import Enum, auto
from .BlueObstacleDetector import BlueObstacleDetector
from .TreeDetector import TreeDetector
from .StorageDetector import StorageDetector
import cv2

class CameraState(Enum):
    AVOID_OBSTACLES = auto()
    FETCH_TREES = auto()
    FETCH_STOREHOUSE_MADURO = auto()
    FETCH_STOREHOUSE_SOBREMADURO = auto()

    @staticmethod
    def from_string(state_str):
        try:
            return CameraState[state_str.upper()]
        except KeyError:
            return None

class CameraLower:
    def __init__(self, camera_index, port='/dev/ttyACM0', baudrate=9600, timeout=1):
        self.state = CameraState.AVOID_OBSTACLES
        print(f"[INIT] Camera initialized in state: {self.state.name}")

        # Initialize serial
        try:
            self.serial = serial.Serial(port, baudrate=baudrate, timeout=timeout)
            print(f"[SERIAL] Connected to {port} at {baudrate} baud.")
        except serial.SerialException as e:
            print(f"[ERROR] Serial connection failed: {e}")
            self.serial = None

        # Initialize detectors
        try:
            self.obstacle_detector = BlueObstacleDetector()
            print("[DETECTOR] BlueObstacleDetector initialized successfully.")
        except Exception as e:
            print(f"[ERROR] BlueObstacleDetector init failed: {e}")
            self.obstacle_detector = None

        try:
            self.tree_detector = TreeDetector()
            print("[DETECTOR] TreeDetector initialized successfully.")
        except Exception as e:
            print(f"[ERROR] TreeDetector init failed: {e}")
            self.tree_detector = None

        try:
            self.storage_detector = StorageDetector()
            print("[DETECTOR] StorageDetector initialized successfully.")
        except Exception as e:
            print(f"[ERROR] StorageDetector init failed: {e}")
            self.storage_detector = None

        # Initialize video capture
        self.cap = cv2.VideoCapture(camera_index)
        if not self.cap.isOpened():
            raise RuntimeError("Could not open camera.")

    def set_state(self, new_state: CameraState):
        print(f"[STATE CHANGE] Changing state from {self.state.name} to {new_state.name}")
        self.state = new_state
    
    def receive_state(self):
        if not self.serial or not self.serial.is_open:
            print("[ERROR] Serial port not available.")
            return

        # Receive new state from serial
        try:
            incoming_data = self.serial.readline().decode().strip()
            print(f"[RECEIVE] Received: {incoming_data}")
            new_state = CameraState.from_string(incoming_data)
            if new_state:
                self.set_state(new_state)
            else:
                print(f"[WARNING] Invalid state received: {incoming_data}")
        except Exception as e:
            print(f"[ERROR] Failed to read or parse serial input: {e}")
    
    def send_data(self, data):
        print(f"[RESULT] {data}")
        if self.serial and self.serial.is_open:
            self.serial.write((data + "\n").encode())

    def read_frame(self):
        # Capture and rotate frame
        ret, frame = self.cap.read()
        if not ret:
            return None
        return frame

    def run(self):
        frame = self.read_frame()
        if frame is None:
            return False, None, False, "", (0.0, 0.0)
        
        print(f"[RUN] Current state: {self.state.name}")
        if self.state == CameraState.AVOID_OBSTACLES:
            self._handle_avoid_obstacles(frame)
        elif self.state == CameraState.FETCH_TREES:
            self._handle_fetch_trees(frame)
        elif self.state == CameraState.FETCH_STOREHOUSE_MADURO:
            self._handle_fetch_storehouse(frame, 'blue')
        elif self.state == CameraState.FETCH_STOREHOUSE_SOBREMADURO:
            self._handle_fetch_storehouse(frame, 'red')



    def _handle_avoid_obstacles(self, frame):
        print("[ACTION] Avoiding obstacles...")
        if not self.obstacle_detector:
            print("[ERROR] Obstacle detector not initialized.")
            return

        detected, distance, too_close, warning, offset = self.obstacle_detector.run(frame, show_on_screen=False) #!change show_on_screen to True for testing

        # Compose result string
        result = f"OBSTACLE: {'True' if detected else 'False'}"
        if detected:
            result += f", DISTANCE: {f'{distance:.1f}' if distance is not None else ''}"
            result += ", TOO_CLOSE: True" if too_close else ", TOO_CLOSE: False"
            result += f", WARNING: {warning}, OFFSET_X: {offset[0]:.2f}, OFFSET_Y: {offset[1]:.2f}"

        self.send_data(result)




    def _handle_fetch_trees(self, frame):
        print("[ACTION] Fetching tree data...")
        if not self.tree_detector:
            print("[ERROR] Tree detector not initialized.")
            return

        detected, distance, too_close, offset = self.tree_detector.run(frame, show_on_screen=False) #!change show_on_screen to True for testing

        result = f"TREE: {'True' if detected else 'False'}"
        if detected:
            result += f", DISTANCE: {f'{distance:.1f}' if distance is not None else ''}"
            result += ", TOO_CLOSE: True" if too_close else ", TOO_CLOSE: False"
            result += f", OFFSET_X:{offset[0]:.2f}, OFFSET_Y:{offset[1]:.2f}"

        self.send_data(result)




    def _handle_fetch_storehouse(self, frame, color):
        print("[ACTION] Running StorageDetector...")
        only_blue = (color == 'blue') or (color != 'red')
        only_red = (color == 'red') or (color != 'blue')
        detected, distance, too_close, warning, offset, box_type = self.storage_detector.run(frame, only_blue=only_blue, only_red=only_red, show_on_screen=False) #!change show_on_screen to True for testing

        # Format the result as a string to send
        result = f"STORAGE: {'True' if detected else 'False'}"
        if detected:
            result += ", TYPE: MADURO" if box_type == 'blue' else ", TYPE: SOBREMADURO"
            result += f", DISTANCE: {f'{distance:.1f}' if distance is not None else ''}"
            result += f", OFFSET_X:{offset[0]:.2f}, OFFSET_Y:{offset[1]:.2f}, WARNING: {warning},"
            result += ", TOO_CLOSE: True" if too_close else ", TOO_CLOSE: False"
        self.send_data(result)




'''
# Example usage
if __name__ == "__main__":
    camera = CameraLower(port='/dev/ttyACM0', camera_index=2)
    while True:
        camera.run()
        camera.receive_state()
        time.sleep(1)
'''

'''
# for testing
if __name__ == "__main__":
    camera = CameraLower(port='/dev/ttyACM0', camera_index=0)
    while True:
        phase_duration = 10
        start_time = time.time()

        camera.set_state(CameraState.AVOID_OBSTACLES) # avoid_obstacles is the DEFAULT state too
        while time.time() - start_time < phase_duration:
            camera.run()
            continue

        camera.set_state(CameraState.FETCH_TREES)
        start_time = time.time()
        while time.time() - start_time < phase_duration:
            camera.run()
            continue

        camera.set_state(CameraState.FETCH_STOREHOUSE_MADURO)
        start_time = time.time()
        while time.time() - start_time < phase_duration:
            camera.run()
            continue

        camera.set_state(CameraState.FETCH_STOREHOUSE_SOBREMADURO)
        start_time = time.time()
        while time.time() - start_time < phase_duration:
            camera.run()
            continue
'''
