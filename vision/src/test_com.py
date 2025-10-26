import serial
import time

ser = serial.Serial(
    port='/dev/ttyUSB0',  # Serial port device file
    baudrate=115200,      # Baud rate (must match the connected device)
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1            
)

try:
    print(f"Opening serial port {ser.port}...")
    ser.isOpen()
    print("Serial port opened successfully.")

    # Send data
    message_to_send = "Hello from Jetson AGX Xavier!\n"
    ser.write(message_to_send.encode())  # Encode string to bytes
    print(f"Sent: {message_to_send.strip()}")

    time.sleep(0.1) # Give some time for the data to be sent

    # Read data
    print("Waiting for data from the connected device...")
    while True:
        if ser.in_waiting > 0:
            received_data = ser.readline().decode().strip() # Read a line and decode from bytes
            print(f"Received: {received_data}")
            break # Exit loop after receiving one line

except serial.SerialException as e:
    print(f"Serial port error: {e}")
except Exception as e:
    print(f"An error occurred: {e}")
finally:
    if ser.isOpen():
        ser.close()
        print("Serial port closed.")