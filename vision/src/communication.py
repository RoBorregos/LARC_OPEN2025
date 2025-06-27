from enum import Enum
import smbus
import time

import communication as comm

class Function(Enum):
    NONE = 0
    GETCOLOR = 1
    GETCENTEROFFSETX = 2
    GETCENTEROFFSETY = 3
    GETOBJECTAREA = 4

# I2C bus (0 for older models, 1 for newer models like Pi 2/3/4)
bus = smbus.SMBus(1)

# I2C address of the Arduino
ARDUINO_ADDRESS = 8

def send_message(message):
    # Convert the message string into a byte array
    bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in message])
    print(f"Sent message: {message}")

def receive_response():
    # Read the response from the Arduino
    time.sleep(0.1)  # Wait a bit for the Arduino to process
    response = bus.read_i2c_block_data(ARDUINO_ADDRESS, 0, 32)
    return ''.join([chr(b) for b in response]).strip()

while True:
    send_message("GIVE_INSTRUCTION")  # Send the command to Arduino
    response = receive_response()

    if (response is not None):
        response = int(response)
        match response:
            case Function.NONE:
                break
            case Function.GETCOLOR:
                send_message(comm.getColor())
                break
            case Function.GETCENTEROFFSETX:
                send_message(comm.getCenterOffsetX())
                break
            case Function.GETCENTEROFFSETY:
                send_message(comm.getCenterOffsetY())
                break
            case Function.GETOBJECTAREA:
                send_message(comm.getObjectArea())
                break


    print(f"Arduino says: {response}")
    time.sleep(1)  # Adjust the time interval as needed
