import serial
import paho.mqtt.client as mqtt
import json
import time
from typing import List

class TeensyCommunication:
    """
    Handles bidirectional communication with Teensy microcontroller and MQTT broker.
    - Sends detection matrix data TO Teensy via Serial
    - Receives data FROM Teensy and publishes to MQTT
    """
    
    def __init__(self, serial_port: str = 'COM3', baudrate: int = 115200, 
                 mqtt_broker: str = 'localhost', mqtt_port: int = 1883):
        """
        Initialize serial connection to Teensy and MQTT client.
        
        Args:
            serial_port: Serial port where Teensy is connected (e.g., 'COM3' on Windows)
            baudrate: Serial communication baudrate
            mqtt_broker: MQTT broker address
            mqtt_port: MQTT broker port
        """
        self.is_sending_enabled = False  # Flag to control matrix sending
        
        # Serial connection to Teensy
        try:
            self.serial_conn = serial.Serial(serial_port, baudrate, timeout=0.1)
            time.sleep(2)  # Wait for connection to establish
            print(f"Serial connection established on {serial_port}")
        except serial.SerialException as e:
            print(f"Error opening serial port: {e}")
            self.serial_conn = None
        
        # MQTT client setup
        self.mqtt_client = mqtt.Client()
        self.mqtt_client.on_connect = self._on_mqtt_connect
        self.mqtt_client.on_disconnect = self._on_mqtt_disconnect
        
        try:
            self.mqtt_client.connect(mqtt_broker, mqtt_port, 60)
            self.mqtt_client.loop_start()
            print(f"MQTT client connected to {mqtt_broker}:{mqtt_port}")
        except Exception as e:
            print(f"Error connecting to MQTT broker: {e}")
    
    def _on_mqtt_connect(self, client, userdata, flags, rc):
        """Callback when connected to MQTT broker"""
        if rc == 0:
            print("Successfully connected to MQTT broker")
        else:
            print(f"Failed to connect to MQTT broker. Return code: {rc}")
    
    def _on_mqtt_disconnect(self, client, userdata, rc):
        """Callback when disconnected from MQTT broker"""
        print(f"Disconnected from MQTT broker. Return code: {rc}")
    
    def sendDataToTeensy(self, bean_matrix: List[str]):
        """
        Sends the bean detection matrix to the Teensy via Serial.
        Only sends if communication is enabled (after start_communication() is called).
        
        Args:
            bean_matrix: List of 2 strings representing [top_bean, bottom_bean]
                        e.g., ["red bean", "white bean"] or ["NULL", "red bean"]
        
        Returns:
            bool: True if data was sent successfully, False otherwise
        """
        if not self.is_sending_enabled:
            return False
            
        if not self.serial_conn or not self.serial_conn.is_open:
            print("Serial connection not available")
            return False
        
        try:
            # Format data as JSON for clear communication
            data = {
                "top": bean_matrix[0] if len(bean_matrix) > 0 else "NULL",
                "bottom": bean_matrix[1] if len(bean_matrix) > 1 else "NULL"
            }
            
            # Convert to JSON string and add newline as delimiter
            message = json.dumps(data) + '\n'
            
            # Send to Teensy
            self.serial_conn.write(message.encode('utf-8'))
            self.serial_conn.flush()
            
            print(f"Sent to Teensy: {data}")
            return True
            
        except Exception as e:
            print(f"Error sending data to Teensy: {e}")
            return False
    
    def sendDataFromTeensy(self, mqtt_topic: str = 'robot/teensy/data'):
        """
        Reads data FROM Teensy via Serial and publishes it to MQTT broker.
        This allows reading Teensy data from a mobile device.
        
        Args:
            mqtt_topic: MQTT topic to publish Teensy data
        
        Returns:
            dict: Data received from Teensy, or None if no data available
        """
        if not self.serial_conn or not self.serial_conn.is_open:
            print("Serial connection not available")
            return None
        
        try:
            # Check if data is available
            if self.serial_conn.in_waiting > 0:
                # Read line from Teensy (assuming newline-delimited messages)
                raw_data = self.serial_conn.readline().decode('utf-8').strip()
                
                if raw_data:
                    try:
                        # Try to parse as JSON
                        teensy_data = json.loads(raw_data)
                    except json.JSONDecodeError:
                        # If not JSON, send as plain text
                        teensy_data = {"message": raw_data}
                    
                    # Add timestamp
                    teensy_data["timestamp"] = time.time()
                    
                    # Publish to MQTT
                    self.mqtt_client.publish(
                        mqtt_topic, 
                        json.dumps(teensy_data),
                        qos=1
                    )
                    
                    print(f"Published to MQTT ({mqtt_topic}): {teensy_data}")
                    return teensy_data
            
            return None
            
        except Exception as e:
            print(f"Error reading data from Teensy: {e}")
            return None
    
    def start_communication(self):
        """
        Enables sending of detection matrices to Teensy.
        Call this when you want to start transmitting vision data.
        
        Returns:
            bool: True if communication was started successfully
        """
        if not self.serial_conn or not self.serial_conn.is_open:
            print("Cannot start communication: Serial connection not available")
            return False
        
        self.is_sending_enabled = True
        print("✓ Communication STARTED - Now sending detection matrices to Teensy")
        
        # Send start signal to Teensy
        try:
            self.serial_conn.write(b"VISION_START\n")
            self.serial_conn.flush()
        except Exception as e:
            print(f"Error sending start signal: {e}")
            
        return True
    
    def end_communication(self):
        """
        Disables sending of detection matrices to Teensy.
        Call this when you want to stop transmitting vision data.
        
        Returns:
            bool: True if communication was stopped successfully
        """
        self.is_sending_enabled = False
        print("✗ Communication ENDED - Stopped sending detection matrices to Teensy")
        
        # Send stop signal to Teensy
        if self.serial_conn and self.serial_conn.is_open:
            try:
                self.serial_conn.write(b"VISION_STOP\n")
                self.serial_conn.flush()
            except Exception as e:
                print(f"Error sending stop signal: {e}")
        
        return True
    
    def is_communication_active(self):
        """
        Check if communication is currently enabled.
        
        Returns:
            bool: True if sending is enabled, False otherwise
        """
        return self.is_sending_enabled
    
    def listen_for_commands(self):
        """
        Listen for commands from Teensy (like START_DETECTION, STOP_DETECTION).
        This method should be called in the main loop to respond to Teensy commands.
        
        Returns:
            str: Command received from Teensy, or None if no command
        """
        if not self.serial_conn or not self.serial_conn.is_open:
            return None
        
        try:
            if self.serial_conn.in_waiting > 0:
                command = self.serial_conn.readline().decode('utf-8').strip()
                
                if command:
                    print(f"Received command from Teensy: {command}")
                    
                    # Auto-handle start/stop commands
                    if command == "START_DETECTION":
                        self.start_communication()
                    elif command == "STOP_DETECTION":
                        self.end_communication()
                    
                    return command
        except Exception as e:
            print(f"Error listening for commands: {e}")
        
        return None
    
    def close(self):
        """Close serial connection and MQTT client"""
        if self.serial_conn and self.serial_conn.is_open:
            self.serial_conn.close()
            print("Serial connection closed")
        
        self.mqtt_client.loop_stop()
        self.mqtt_client.disconnect()
        print("MQTT client disconnected")


# Example usage
if __name__ == "__main__":
    # Initialize communication
    comm = TeensyCommunication(
        serial_port='COM3',  # Adjust to your Teensy's port
        baudrate=115200,
        mqtt_broker='localhost',  # Change to your MQTT broker address
        mqtt_port=1883
    )
    
    try:
        while True:
            # Example: Send detection matrix to Teensy
            example_matrix = [0, 1]
            comm.sendDataToTeensy(example_matrix)
            
            # Read data from Teensy and send to MQTT
            comm.sendDataFromTeensy()
            
            time.sleep(0.1)  # 100ms delay
            
    except KeyboardInterrupt:
        print("\nStopping communication...")
    finally:
        comm.close()
