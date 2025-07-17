import serial
import time

# Abre el UART principal del Pi
ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=1)
time.sleep(0.1)  # Espera breve

# Envía mensaje
ser.write(b'Hola Teensy\n')
time.sleep(0.2)

# Lee respuesta
data = ser.readline().decode(errors='ignore').strip()
print("Respuesta desde Teensy:", repr(data))

ser.close()

