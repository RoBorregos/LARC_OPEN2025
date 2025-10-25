"""
Test script for communication.py
Tests basic functionality without camera/vision requirements.
"""

import time
from communication import TeensyCommunication

def test_basic_communication():
    """Test basic start/stop and data sending"""
    print("=" * 60)
    print("Testing TeensyCommunication")
    print("=" * 60)
    
    # Initialize communication
    comm = TeensyCommunication(
        serial_port='COM3',  # Adjust to your port
        baudrate=115200,
        mqtt_broker='localhost',
        mqtt_port=1883
    )
    
    print("\nWaiting 2 seconds for connection to stabilize...")
    time.sleep(2)
    
    # Test 1: Start communication
    print("\n[TEST 1] Starting communication...")
    result = comm.start_communication()
    print(f"Result: {'SUCCESS' if result else 'FAILED'}")
    time.sleep(1)
    
    # Test 2: Send detection matrix
    print("\n[TEST 2] Sending bean matrix...")
    test_matrices = [
        ["red bean", "white bean"],
        ["NULL", "black bean"],
        ["white bean", "NULL"],
        ["NULL", "NULL"]
    ]
    
    for matrix in test_matrices:
        print(f"  Sending: {matrix}")
        comm.sendDataToTeensy(matrix)
        time.sleep(0.5)
    
    # Test 3: Check if communication is active
    print("\n[TEST 3] Checking communication status...")
    is_active = comm.is_communication_active()
    print(f"Communication active: {is_active}")
    
    # Test 4: Listen for commands (non-blocking test)
    print("\n[TEST 4] Listening for commands from Arduino/Teensy...")
    print("(Send START_DETECTION or STOP_DETECTION from Arduino)")
    for i in range(5):
        command = comm.listen_for_commands()
        if command:
            print(f"  Received: {command}")
        time.sleep(1)
    
    # Test 5: Read data from Arduino/Teensy
    print("\n[TEST 5] Reading data from Arduino/Teensy...")
    print("(Send JSON data from Arduino)")
    for i in range(5):
        data = comm.sendDataFromTeensy()
        if data:
            print(f"  Received: {data}")
        time.sleep(1)
    
    # Test 6: End communication
    print("\n[TEST 6] Ending communication...")
    comm.end_communication()
    is_active = comm.is_communication_active()
    print(f"Communication active after end: {is_active}")
    time.sleep(1)
    
    # Test 7: Try sending when disabled
    print("\n[TEST 7] Attempting to send while disabled...")
    result = comm.sendDataToTeensy(["test", "test"])
    print(f"Send result (should be False): {result}")
    
    # Cleanup
    print("\n[CLEANUP] Closing communication...")
    comm.close()
    
    print("\n" + "=" * 60)
    print("Tests completed!")
    print("=" * 60)

def test_manual_control():
    """Interactive test for manual control"""
    print("=" * 60)
    print("Manual Communication Test")
    print("=" * 60)
    print("Commands:")
    print("  's' - Start communication")
    print("  'e' - End communication")
    print("  '1' - Send test matrix [red bean, white bean]")
    print("  '2' - Send test matrix [NULL, black bean]")
    print("  'l' - Listen for incoming data")
    print("  'q' - Quit")
    print("=" * 60)
    
    comm = TeensyCommunication(
        serial_port='COM3',
        baudrate=115200,
        mqtt_broker='localhost',
        mqtt_port=1883
    )
    
    time.sleep(2)
    
    try:
        while True:
            cmd = input("\nEnter command: ").strip().lower()
            
            if cmd == 's':
                comm.start_communication()
            elif cmd == 'e':
                comm.end_communication()
            elif cmd == '1':
                comm.sendDataToTeensy(["red bean", "white bean"])
            elif cmd == '2':
                comm.sendDataToTeensy(["NULL", "black bean"])
            elif cmd == 'l':
                print("Listening for 5 seconds...")
                for _ in range(10):
                    command = comm.listen_for_commands()
                    data = comm.sendDataFromTeensy()
                    if command:
                        print(f"  Command: {command}")
                    if data:
                        print(f"  Data: {data}")
                    time.sleep(0.5)
            elif cmd == 'q':
                break
            else:
                print("Unknown command")
    
    except KeyboardInterrupt:
        print("\nInterrupted")
    finally:
        comm.close()

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) > 1 and sys.argv[1] == "manual":
        test_manual_control()
    else:
        test_basic_communication()
