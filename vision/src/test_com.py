import serial
import time

ser = serial.Serial('/dev/ttyTHS1', 9600, timeout=1)

time.sleep(2)

try:
    while True:
        # Envía un mensaje
        ser.write(b'Hola desde Jetson Nano\n')
        print("Mensaje enviado.")
        
        response = ser.readline().decode().strip()
        if response:
            print("Respuesta de Teensy:", response)
        
        time.sleep(1)

except KeyboardInterrupt:
    print("Terminando comunicación.")
    ser.close()