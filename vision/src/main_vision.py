"""
Main vision system with integrated communication to Teensy and MQTT.
This integrates parse_data.py detection with communication.py for full system operation.
"""

import cv2
import time
from communication import TeensyCommunication
from parse_data import find_bean_matrix, model

def main():
    """
    Main loop that:
    1. Captures video frames
    2. Detects bean matrix using YOLO
    3. Sends matrix to Teensy (when enabled)
    4. Receives data from Teensy and publishes to MQTT
    5. Listens for start/stop commands from Teensy
    """
    
    # Initialize communication
    comm = TeensyCommunication(
        serial_port='COM3',  # Adjust to your Teensy's serial port
        baudrate=115200,
        mqtt_broker='localhost',  # Change to your MQTT broker IP
        mqtt_port=1883
    )
    
    # Initialize camera
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Can't open camera.")
        return
    
    # Camera settings
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    cap.set(cv2.CAP_PROP_FPS, 60)
    
    print("=" * 60)
    print("Vision System Started")
    print("=" * 60)
    print("Controls:")
    print("  's' - Start sending data to Teensy")
    print("  'e' - End/Stop sending data to Teensy")
    print("  ESC - Exit program")
    print("=" * 60)
    
    try:
        while True:
            # Read frame from camera
            ret, frame = cap.read()
            if not ret:
                print("Warning: Failed to read frame")
                continue
            
            # Listen for commands from Teensy (START_DETECTION, STOP_DETECTION)
            comm.listen_for_commands()
            
            # Detect beans using YOLO
            results = model(frame, verbose=False)
            vertical_matrix = find_bean_matrix(results, width, height)
            
            # Send matrix to Teensy (only if communication is enabled)
            if comm.is_communication_active():
                comm.sendDataToTeensy(vertical_matrix)
            
            # Check for and forward any data from Teensy to MQTT
            comm.sendDataFromTeensy(mqtt_topic='robot/teensy/sensor_data')
            
            # Display detection results on frame
            if vertical_matrix:
                status_text = "ACTIVE" if comm.is_communication_active() else "PAUSED"
                status_color = (0, 255, 0) if comm.is_communication_active() else (0, 165, 255)
                
                cv2.putText(
                    frame, 
                    f"Status: {status_text}", 
                    (10, 30), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, status_color, 2
                )
                cv2.putText(
                    frame, 
                    f"Top: {vertical_matrix[0]}", 
                    (10, 60), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2
                )
                cv2.putText(
                    frame, 
                    f"Bottom: {vertical_matrix[1]}", 
                    (10, 90), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2
                )
            
            # Display annotated frame
            annotated_img = results[0].plot(
                boxes=True,
                labels=True,
                conf=False
            )
            cv2.imshow('Vision System - Bean Detection', annotated_img)
            
            # Handle keyboard input
            key = cv2.waitKey(1) & 0xFF
            
            if key == 27:  # ESC key - exit
                print("\nExiting...")
                break
            elif key == ord('s'):  # 's' key - start communication
                comm.start_communication()
            elif key == ord('e'):  # 'e' key - end communication
                comm.end_communication()
            
            # Small delay to prevent overwhelming the serial port
            time.sleep(0.05)  # 50ms = ~20 fps for communication
    
    except KeyboardInterrupt:
        print("\nInterrupted by user")
    
    finally:
        # Cleanup
        print("Cleaning up...")
        comm.end_communication()
        comm.close()
        cap.release()
        cv2.destroyAllWindows()
        print("Vision system stopped")


if __name__ == "__main__":
    main()
