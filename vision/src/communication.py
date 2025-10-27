from enum import Enum, IntEnum
import smbus
import time

class Function(IntEnum):
    NONE = 0
    DETECT_BEAN = 1
    DETECT_OBJECT = 2

# Constants
I2C_BUS = 1 # I2C bus (0 for older models, 1 for newer models like Pi 2/3/4)
ARDUINO_ADDRESS = 8 # I2C address of the Arduino
RESPONSE_BUFFER_SIZE = 32
RESPONSE_DELAY = 0.1

bus = smbus.SMBus(I2C_BUS)

def send_message(message: str) -> None:
    '''Send a message to Arduino via I2C.'''
    try:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in message]) # Convert the message string into a byte array
        print(f"Sent message: {message}")
    except Exception as e:
        print(f"Error sending message: {e}")

def receive_response() -> str:
    '''Receive response from Arduino via I2C.'''
    try:
        time.sleep(RESPONSE_DELAY)
        response = bus.read_i2c_block_data(ARDUINO_ADDRESS, 0, RESPONSE_BUFFER_SIZE)
        return ''.join([chr(b) for b in response if b != 0]).strip()
    except Exception as e:
        print(f"Error receiving response: {e}")
        return ""

def process_command(command: str) -> Function:
    '''Process the command received from Arduino and return the corresponding Function.'''
    if command == "DETECT_BEAN":
        return Function.DETECT_BEAN
    elif command == "DETECT_OBJECT":
        return Function.DETECT_OBJECT
    else:
        return Function.NONE


def start_model_once(camera):
    """A small helper to run one pass of the model/detection when START is received.
    This calls camera.detect_bean() as an example and prints results. In a
    production setup you would start the continuous loop or the model server.
    """
    try:
        bean_detected, bean_type, bx, by = camera.detect_bean()
        print(f"[VISION] Bean: {bean_detected}, Type: {bean_type}, Offsets: X:{bx:.2f}, Y:{by:.2f}")
        # Example: send a small matrix or data back to the Teensy as a debug reply
        # Here we will send a tiny textual matrix-like structure in brackets
        matrix_str = "[1,0,0;0,1,0;0,0,1]"
        send_message(f"MATRIX:{matrix_str}")
    except Exception as e:
        print(f"Error running model: {e}")


def listen_and_serve(camera, poll_interval: float = 0.2):
    """Poll the Arduino/Teensy device for messages and react.
    - If we receive 'START' => run the model (or start the model loop).
    - If we receive 'DATA:' prefixed payload => parse and optionally respond.
    This function is intentionally simple and uses the existing I2C read path.
    """
    print("[VISION] Starting listen loop. Polling for messages...")
    try:
        while True:
            cmd = receive_response()
            if not cmd:
                time.sleep(poll_interval)
                continue

            cmd = cmd.strip()
            print(f"[VISION] Received: '{cmd}'")

            if cmd == "START":
                print("[VISION] START command received. Running model once.")
                start_model_once(camera)
            elif cmd.startswith("DATA:"):
                payload = cmd[len("DATA:"):]
                print(f"[VISION] Received DATA payload: {payload}")
                # For debug, reply acknowledging and optionally send matrix/data
                send_message("ACK_DATA")
            elif cmd.startswith("MATRIX:"):
                # if the microcontroller sends matrix data to Vision
                matrix_payload = cmd[len("MATRIX:"):]
                print(f"[VISION] Received MATRIX payload: {matrix_payload}")
            else:
                # Map simple commands into Function enum if needed
                func = process_command(cmd)
                if func != Function.NONE:
                    print(f"[VISION] Mapped command to function: {func}")

            time.sleep(poll_interval)
    except KeyboardInterrupt:
        print("[VISION] Listener interrupted by user")