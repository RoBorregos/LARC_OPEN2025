import serial
import time

# Configura UART en Jetson (cambia el puerto si es necesario)
# UART1 normalmente es: /dev/ttyTHS1
ser = serial.Serial('/dev/ttyTHS1', 9600, timeout=1)

print("Jetson lista para comunicarse con NodeMCU...")

while True:
    # Leer si hay datos desde NodeMCU
    if ser.in_waiting > 0:
        received = ser.readline().decode('utf-8').strip()
        print("Mensaje recibido:", received)

        # Responder al NodeMCU
        reply = "ACK desde Jetson\n"
        ser.write(reply.encode('utf-8'))
        print("Respuesta enviada al NodeMCU")

    time.sleep(0.1)
