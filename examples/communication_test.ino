/*
 * Example Arduino sketch demonstrating Communication subsystem usage
 * 
 * This shows how to:
 * 1. Initialize communication with vision system
 * 2. Start/stop detection
 * 3. Read bean detection data
 * 4. Send sensor data to MQTT via vision system
 */

#include "subsystem/Communication/Communication.hpp"

Communication comm;

// Example sensor values
float distance = 0.0;
int angle = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000); // Wait for Serial with timeout
    
    Serial.println("=== Communication Test Starting ===");
    
    // Initialize communication subsystem
    comm.begin();
    
    Serial.println("Communication initialized");
    delay(2000); // Give vision system time to start
    
    // Start vision detection
    Serial.println("Starting vision detection...");
    comm.startDetection();
    
    delay(1000);
}

void loop() {
    // Update communication (reads data from vision system)
    comm.update();
    
    // Check if vision system is active
    if (comm.isVisionActive()) {
        // Get detected beans
        String topBean = comm.getTopBean().c_str();
        String bottomBean = comm.getBottomBean().c_str();
        
        // Only print if we have actual detections (not NULL)
        if (topBean != "NULL" || bottomBean != "NULL") {
            Serial.println("--- Vision Detection ---");
            Serial.print("Top Bean: ");
            Serial.println(topBean);
            Serial.print("Bottom Bean: ");
            Serial.println(bottomBean);
            
            // Example: Make decisions based on detection
            if (topBean == "red bean") {
                Serial.println("Action: Detected red bean on top!");
                // Add your logic here
            }
            
            if (bottomBean == "white bean") {
                Serial.println("Action: Detected white bean on bottom!");
                // Add your logic here
            }
        }
    } else {
        Serial.println("Vision system is not active");
    }
    
    // Simulate sensor readings
    distance += 0.5;
    angle = (angle + 5) % 360;
    
    // Send sensor data to MQTT (via vision system)
    String sensorData = "{\"distance\":" + String(distance, 1) + 
                       ",\"angle\":" + String(angle) + 
                       ",\"millis\":" + String(millis()) + "}";
    
    comm.sendDataToMQTT(sensorData);
    Serial.print("Sent to MQTT: ");
    Serial.println(sensorData);
    
    // Print separator
    Serial.println("------------------------");
    
    delay(1000); // Update every second
}

/*
 * Example output:
 * 
 * === Communication Test Starting ===
 * Communication initialized
 * Starting vision detection...
 * Vision system activated
 * --- Vision Detection ---
 * Top Bean: red bean
 * Bottom Bean: white bean
 * Action: Detected red bean on top!
 * Action: Detected white bean on bottom!
 * Sent to MQTT: {"distance":25.5,"angle":90,"millis":12345}
 * ------------------------
 */
