#!/bin/bash
# Script para iniciar el flujo de visión en Jetson Xavier AGX

USER="xavier"
VISION_DIR="/home/$USER/Desktop/LARC_OPEN2025/vision"
TTY_DEVICE="/dev/ttyACM0"
DOCKER_DIR="/home/$USER/docker_pytorch"
YAML_FILE="pytorch.yaml"
CONTAINER_NAME="pytorch"

echo "=== Iniciando entorno LARC (docker compose) ==="
cd "$DOCKER_DIR" || { echo "No se encontró $DOCKER_DIR"; exit 1; }

docker compose -f "$YAML_FILE" start

docker compose -f "$YAML_FILE" exec "$CONTAINER_NAME" bash -c "
  echo '=== Entrando al entorno de visión ===';
  cd /workspace/Desktop/LARC_OPEN2025/vision || exit 1;
  sudo chmod 777 /dev/ttyACM0;
  echo '=== Ejecutando main_detection.py ===';
  python3 src/main_detection.py;
"