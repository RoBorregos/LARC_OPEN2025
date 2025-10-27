import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
print("Jetson lista comunicando por USB con ESP32...")

while True:
    if ser.in_waiting > 0:
        msg = ser.readline().decode('utf-8', errors='ignore').strip()
        print("📩 ESP32 dijo:", msg)

    ser.write(b"Mensaje desde Jetson\n")
    time.sleep(1)