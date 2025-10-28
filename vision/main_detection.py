from src.complete_parse_data import Camera
import serial
import cv2
from ultralytics import YOLO
import subprocess

# ------------ Configuration ------------
MODEL_PATH   = "model/nanoModel.pt"
SOURCE       = 0  # Use 0 for xavier
TIMEOUT_SEC  = 0.5
SERIAL_PORT  = "/dev/ttyACM0"
BAUDRATE     = 115200

def handle_serial_command(cmd, cam: Camera):
    cmd = cmd.strip()
    command_actions = {
        "DETECT_BEANS": lambda: cam.set_state(Camera.State.DETECT_BEANS),
        "DETECT_RED_STORAGE": lambda: cam.set_state(Camera.State.DETECT_RED_STORAGE),
        "DETECT_BLUE_STORAGE": lambda: cam.set_state(Camera.State.DETECT_BLUE_STORAGE),
        "STOP_DETECTING": lambda: cam.set_state(Camera.State.STOP),
        "SHUTDOWN": lambda: (print("[INFO] Powering off Xavier..."), subprocess.run("sudo shutdown now", shell=True)),
    }
    action = command_actions.get(cmd)
    if action:
        action()
    else:
        print(f"[WARN] Unknown command: {cmd}")

def mapping(label) -> int:
    mapping_dict = {
        # beans
        "inmature": 0,
        "mature": 1,
        "overmature": 2,

        # storage
        "blue_benefit": 0,
        "red_benefit": 1,

        # None
        None: -1
        
        # Float, send number
    }
    return mapping_dict.get(label, -1)

# ------------ Main loop ------------
def run(verbose: bool):
    cam = Camera(model=YOLO(MODEL_PATH, task="detect"), timeout=TIMEOUT_SEC)

    cap = cv2.VideoCapture(SOURCE)
    if not cap.isOpened():
        raise RuntimeError(f"Could not open video source: {SOURCE}")
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    cap.set(cv2.CAP_PROP_FPS, 30)
    verbose = True

    # Open serial port
    ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=0.1)
    print(f"[INFO] Listening for Teensy commands on {SERIAL_PORT}...")

    while ser.readline().decode(errors="ignore").strip() != "START":
        continue
    ser.write(b"STARTED\n")
    print("[INFO] Starting detection loop...")
    
    try:
        while True:
            # --- Handle Teensy commands ---
            if ser.in_waiting:
                line = ser.readline().decode(errors="ignore").strip()
                if line:
                    handle_serial_command(line)

            # --- YOLO detection ---
            ok, frame = cap.read()
            if not ok:
                break

            cam.run(frame, verbose=verbose)
            ser.write(f"{mapping(cam.detection_matrix[0])},{mapping(cam.detection_matrix[1])}\n".encode())

            if cv2.waitKey(1) & 0xFF == 27:
                break

    finally:
        cap.release()
        ser.close()
        if verbose:
            cv2.destroyAllWindows()

if __name__ == "__main__":
    run(verbose=True)