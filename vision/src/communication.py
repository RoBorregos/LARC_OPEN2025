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