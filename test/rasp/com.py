import serial
import time

# Abrir el puerto serial (ajusta baudrate si es diferente)
ser = serial.Serial('/dev/serial0', baudrate=115200, timeout=1)

print("Esperando datos desde la Teensy...\n")

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8', errors='ignore').strip()
            if data:
                print("Recibido:", data)
        time.sleep(0.1)

except KeyboardInterrupt:
    print("\nPrograma detenido por el usuario.")
    ser.close()
