from src.communication import Function, send_message, receive_response, process_command
from vision.src.modelIRL import Camera

class SystemController:
    def __init__(self):
        self.started = False
        self.camera_state = Function.NONE
        self.camera = Camera(camera1=0, camera2=1)  # Initialize camera system

    def initialize_system(self) -> bool:
        '''Initialize all system components.'''
        if self.camera.initialize():
            if send_message("START"):
                return receive_response() == "STARTED"
        return False

    def process_camera_data(self, function: Function) -> None:
        '''Process camera data based on current function.'''
        if function == Function.NONE:
            return

        # Determine which detection function to use
        detection_func = {
            Function.DETECT_BEAN: self.camera.detect_bean,
            Function.DETECT_OBJECT: self.camera.detect_obstacle
        }.get(function)

        if not detection_func:
            return

        # Get detection results
        detected, obj_type, offset_x, offset_y = detection_func()
        
        # Format message using f-strings for better performance
        prefix = "BEAN" if function == Function.DETECT_BEAN else "OBJECT"
        msg = f"{prefix}: {detected} Type: {obj_type} x: {offset_x} y: {offset_y}"
        
        send_message(msg)

    def main_loop(self):
        '''Main system control loop.'''
        while True:
            if not self.started:
                self.started = self.initialize_system()
                continue

            # Process commands
            command = receive_response()
            if command:
                current_function = process_command(command)
                if current_function != Function.NONE:
                    send_message("DONE")
                    self.process_camera_data(current_function)

if __name__ == "__main__":
    controller = SystemController()
    controller.main_loop()