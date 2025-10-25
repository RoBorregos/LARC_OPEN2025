# Quick Start - Sistema de Comunicación

## 🚀 Inicio Rápido

### 1️⃣ Setup Python (Raspberry Pi / PC)

```bash
cd vision
pip install -r requirements.txt
```

### 2️⃣ Ejecutar Sistema de Visión

```bash
python src/main_vision.py
```

**Controles:**
- `s` → START comunicación
- `e` → END comunicación  
- `ESC` → Salir

### 3️⃣ Setup Arduino/Teensy

En tu sketch de Arduino:

```cpp
#include "subsystem/Communication/Communication.hpp"

Communication comm;

void setup() {
    comm.begin();
    comm.startDetection();  // Inicia detección
}

void loop() {
    comm.update();  // Lee datos de visión
    
    // Obtener datos
    String top = comm.getTopBean().c_str();
    String bottom = comm.getBottomBean().c_str();
    
    // Enviar a MQTT
    comm.sendDataToMQTT("{\"sensor\": 123}");
}
```

## 📊 Flujo de Datos

```
┌──────────────┐                    ┌────────────┐
│   VISIÓN     │  {"top":"red",     │  ARDUINO   │
│  (Python)    │   "bottom":"white"}│  (C++)     │
│              │ ─────────────────► │            │
│ parse_data.py│                    │ Comm.cpp   │
│ + comm.py    │ ◄───────────────── │            │
└──────────────┘  START_DETECTION   └────────────┘
       │
       │ MQTT
       ▼
┌──────────────┐
│   CELULAR    │
│ MQTT Client  │
└──────────────┘
```

## 🔧 Funciones Principales

### Python (`communication.py`)

| Función | Descripción |
|---------|-------------|
| `start_communication()` | Activa envío de matrices |
| `end_communication()` | Pausa envío de matrices |
| `sendDataToTeensy(matrix)` | Envía detección a Arduino |
| `sendDataFromTeensy()` | Lee de Arduino y publica a MQTT |
| `listen_for_commands()` | Escucha comandos START/STOP |

### C++ (`Communication.hpp`)

| Función | Descripción |
|---------|-------------|
| `startDetection()` | Envía START_DETECTION a Python |
| `stopDetection()` | Envía STOP_DETECTION a Python |
| `getTopBean()` | Obtiene frijol superior detectado |
| `getBottomBean()` | Obtiene frijol inferior detectado |
| `isVisionActive()` | Verifica si visión está activa |
| `sendDataToMQTT(data)` | Envía datos a MQTT vía Python |

## 📝 Protocolo Serial

### Python → Arduino (Detecciones)
```json
{"top": "red bean", "bottom": "white bean"}
```

### Arduino → Python (Comandos)
```
START_DETECTION
STOP_DETECTION
```

### Arduino → Python → MQTT
```
MQTT:{"distance": 25.5, "angle": 90}
```

## 🧪 Testing

### Test Python
```bash
# Test automático
python src/test_communication.py

# Test manual interactivo
python src/test_communication.py manual
```

### Test Arduino
Cargar: `examples/communication_test.ino`

## ⚙️ Configuración

### Puerto Serial
```python
# En main_vision.py
comm = TeensyCommunication(
    serial_port='COM3',  # Windows
    # serial_port='/dev/ttyACM0',  # Linux
    baudrate=115200
)
```

### MQTT Broker
```python
comm = TeensyCommunication(
    mqtt_broker='192.168.1.100',  # IP del broker
    mqtt_port=1883
)
```

## 📱 Ver Datos en Celular

1. Instalar app MQTT (ej: IoT MQTT Panel)
2. Conectar a broker: `192.168.1.100:1883`
3. Suscribirse: `robot/teensy/sensor_data`
4. ✅ Listo! Verás datos en tiempo real

## ⚠️ Troubleshooting

**Puerto no encontrado:**
```bash
# Ver puertos disponibles
python -m serial.tools.list_ports
```

**MQTT no conecta:**
- Verificar firewall
- Ping al broker: `ping 192.168.1.100`

**No detecta frijoles:**
- Verificar iluminación
- Revisar que modelo esté en `vision/model/`

## 📚 Más Información

Ver: `COMMUNICATION_README.md` para documentación completa
