#include "Communication.hpp"
#include <string.h>
using namespace std;

Communication::Communication() : new_obstacle_data(false), new_bean_data(false) {
    Serial.begin(115200);       // USB serial (para debuggear)

    // UARTs on Teensy 4.1
    Serial1.begin(115200);      // UART on pins 0 (RX) and 1 (TX), UART1 - Raspberry Pi Vision
    Serial2.begin(115200);      // UART on pins 7 (RX) and 8 (TX), UART2
    Serial3.begin(115200);      // UART on pins 15 (RX) and 14 (TX), UART3
    
    // Initialize data structures
    latest_obstacle_data = {false, 0.0, 0.0, 0};
}

void Communication::begin() {
    // Send initialization message to vision system
    Serial1.println("TEENSY_READY");
}

void Communication::update() {
    // Check for new vision messages
    string msg = readCommunication();
    if (!msg.empty()) {
        parseVisionMessage(msg);
    }
}

string Communication::getCommand() {
    string msg = readCommunication();

    if (msg == "OBSTACLE") {
        // TODO: Implement obstacle logic
    }

    return msg;
}

string Communication::readCommunication() {
    String msg;
    
    // Check Serial1 (Vision system - Raspberry Pi)
    if (Serial1.available()) {
        msg = Serial1.readStringUntil('\n');
        Serial.print("Vision data: ");
        Serial.println(msg);
        return msg.c_str();
    }
    
    // Check Serial2 for other communications
    if (Serial2.available()) {
        msg = Serial2.readStringUntil('\n');
        Serial2.print("Teensy receive: ");
        Serial2.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    
    // Check Serial3 for other communications
    if (Serial3.available()) {
        msg = Serial3.readStringUntil('\n');
        Serial3.print("Teensy receive: ");
        Serial3.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    
    return "";
}

void Communication::parseVisionMessage(const string& message) {
    if (message.find("OBSTACLE_DETECTED") != string::npos) {
        parseObstacleMessage(message);
    } else if (message.find("NO_OBSTACLE") != string::npos) {
        latest_obstacle_data = {false, 0.0, 0.0, millis()};
        new_obstacle_data = true;
    }
}

void Communication::parseObstacleMessage(const string& message) {
    // Format: "OBSTACLE_DETECTED,distance,angle"
    size_t comma1 = message.find(',');
    size_t comma2 = message.find(',', comma1 + 1);
    
    if (comma1 != string::npos && comma2 != string::npos) {
        float distance = atof(message.substr(comma1 + 1, comma2 - comma1 - 1).c_str());
        float angle = atof(message.substr(comma2 + 1).c_str());
        
        latest_obstacle_data = {true, distance, angle, millis()};
        new_obstacle_data = true;
        
        Serial.print("Obstacle detected - Distance: ");
        Serial.print(distance);
        Serial.print(", Angle: ");
        Serial.println(angle);
    }
}

ObstacleData Communication::getObstacleData() {
    new_obstacle_data = false;  // Mark as read
    return latest_obstacle_data;
}

bool Communication::hasNewObstacleData() {
    return new_obstacle_data;
}


void Communication::requestObstacleDetection() {
    Serial1.println("DETECT_OBJECT");
    Serial.println("Requested obstacle detection");
}


void Communication::stopDetection() {
    Serial1.println("STOP_DETECTION");
    Serial.println("Stopped detection");
}

void Communication::setState(int state) {
    // Handle state changes if needed
    (void)state;
}