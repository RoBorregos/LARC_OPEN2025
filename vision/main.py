from src.complete_parse_data import *  # provides Camera, CameraState, etc.
import serial
import cv2
from ultralytics import YOLO
import subprocess
import sys
import time
from typing import Union

# ------------ Configuration ------------
MODEL_PATH   = "model/nanoModel.pt"
SOURCE       = 0            # Use 0 for Xavier camera
TIMEOUT_SEC  = 1.0
SERIAL_PORT  = "/dev/ttyACM0"
BAUDRATE     = 115200

def handle_serial_command(cmd: str, cam: Camera):
    """Handle a single command coming from Teensy."""
    cmd = cmd.strip()
    command_actions = {
        "PICKUP STATE":        lambda: cam.set_state(CameraState.DETECT_BEANS),
        "DETECT_RED_STORAGE":  lambda: cam.set_state(CameraState.DETECT_RED_STORAGE),
        "DETECT_BLUE_STORAGE": lambda: cam.set_state(CameraState.DETECT_BLUE_STORAGE),
        "STOP_DETECTING":      lambda: cam.set_state(CameraState.STOP),
        "SHUTDOWN":            lambda: (
            print("[INFO] Powering off Xavier..."),
            subprocess.run("sudo shutdown now", shell=True)
        ),
    }
    action = command_actions.get(cmd)
    if action:
        action()
        print(f"[INFO] Command handled: {cmd}")
    else:
        print(f"[WARN] Unknown command from serial: {cmd}")

def map_value(value: Union[str, int, float, None]) -> Union[int, float]:
    if isinstance(value, (int, float)):
        return value
    if value is None:
        return -1

    key = str(value).strip().lower()
    mapping_dict = {
        # beans
        "inmature": 0,
        "mature": 1,
        "overmature": 2,
        # storage
        "blue_benefit": 0,
        "red_benefit": 1,

        #Float : number
    }
    print(f"[DEBUG] Mapping value: {value} -> {mapping_dict.get(key, -1)}")
    return mapping_dict.get(key, -1)

# ------------ Main loop ------------
def run():
    # Load model
    model = YOLO(MODEL_PATH, task="detect")
    cam = Camera(model=model, timeout=TIMEOUT_SEC)

    # Open camera
    cap = cv2.VideoCapture(SOURCE)
    if not cap.isOpened():
        raise RuntimeError(f"Could not open video source: {SOURCE}")

    # Try to keep latency low
    try:
        cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    except Exception:
        pass
    try:
        cap.set(cv2.CAP_PROP_FPS, 30)
    except Exception:
        pass

    # Open serial port
    ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=0.05)
    print(f"[INFO] Listening for Teensy commands on {SERIAL_PORT} WAITING ON THE PICKUP STATE...")

    while ser.readline().decode(errors="ignore").strip() != "PICKUP STATE":
        continue
    ser.write(b"XAVIER READY\n")

    # Default initial state
    cam.set_state(CameraState.DETECT_BEANS)

    try:
        while True:
            # --- Handle Teensy commands (non-blocking) ---
            try:
                if ser.in_waiting:
                    line = ser.readline().decode(errors="ignore").strip()
                    if line:
                        handle_serial_command(line, cam)  # <-- FIX: pass cam
            except Exception as e:
                print(f"[WARN] Serial read error: {e}")

            # --- Capture frame ---
            ok, frame = cap.read()
            if not ok or frame is None:
                print("[WARN] Failed to read frame. Retrying...")
                time.sleep(0.01)
                continue

            # --- Run detection state machine ---
            try:
                cam.run(frame, verbose=False)
            except Exception as e:
                # Keep loop alive even if a single inference fails
                print(f"[WARN] cam.run failed: {e}")
                continue

            # Ensure detection_matrix is 2-length
            out0 = cam.detection_matrix[0] if len(cam.detection_matrix) > 0 else None
            out1 = cam.detection_matrix[1] if len(cam.detection_matrix) > 1 else None

            # Convert to protocol and send over serial
            v0 = map_value(out0)
            v1 = map_value(out1)
            sending_msg = f"{v0},{v1}\n".encode()

            print(sending_msg)

            try:
                ser.write(sending_msg)
                # Optional: flush to minimize latency
                ser.flush()
            except Exception as e:
                print(f"[WARN] Serial write error: {e}")

            # Escape on ESC (only useful if you display a window elsewhere)
            if cv2.waitKey(1) & 0xFF == 27:
                break

    except KeyboardInterrupt:
        print("\n[INFO] Interrupted by user. Exiting...")
    finally:
        try:
            cap.release()
        except Exception:
            pass
        try:
            ser.close()
        except Exception:
            pass
        cv2.destroyAllWindows()

if __name__ == "__main__":
    run()
