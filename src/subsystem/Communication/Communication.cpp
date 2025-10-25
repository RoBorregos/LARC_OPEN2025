#include "Communication.hpp"
#include <string.h>
#include <ArduinoJson.h>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)

    // UARTs on Teensy 4.1
    Serial1.begin(115200);      // UART on pins 0 (RX) and 1 (TX), UART1 - Raspberry Pi Vision
    Serial2.begin(115200);      // UART on pins 7 (RX) and 8 (TX), UART2
    Serial3.begin(115200);      // UART on pins 15 (RX) and 14 (TX), UART3
    
    // Initialize bean matrix
    top_bean_ = "NULL";
    bottom_bean_ = "NULL";
    vision_active_ = false;
}

void Communication::begin() {
    // Send initialization message to vision system
    Serial1.println("TEENSY_READY");
}

void Communication::update() {
    // Read and process any incoming data from vision system
    readVisionData();
}

string Communication::getCommand() {
    string msg = readCommunication();

    if (msg == "BENEFICIADERO") {
        // TODO: Implement obstacle logic
    }

    return msg;
}

string Communication::readCommunication() {
    String msg;
    
    // Check Serial1
    if (Serial1.available()) {
        msg = Serial1.readStringUntil('\n');
        Serial.print("Vision data: ");
        Serial.println(msg);
        return msg.c_str();
    }
    
    return "";
}

void Communication::readVisionData() {
    if (Serial1.available() > 0) {
        String incoming = Serial1.readStringUntil('\n');
        incoming.trim();
        
        if (incoming.length() == 0) return;
        
        // Handle status messages
        if (incoming == "VISION_START") {
            vision_active_ = true;
            Serial.println("Vision system activated");
            return;
        }
        else if (incoming == "VISION_STOP") {
            vision_active_ = false;
            top_bean_ = "NULL";
            bottom_bean_ = "NULL";
            Serial.println("Vision system deactivated");
            return;
        }
        
        // Try to parse JSON data
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, incoming);
        
        if (!error) {
            // Successfully parsed JSON - extract bean data
            if (doc.containsKey("top") && doc.containsKey("bottom")) {
                top_bean_ = doc["top"].as<String>().c_str();
                bottom_bean_ = doc["bottom"].as<String>().c_str();
                
                Serial.print("Received beans - Top: ");
                Serial.print(top_bean_.c_str());
                Serial.print(", Bottom: ");
                Serial.println(bottom_bean_.c_str());
            }
        } else {
            // Not JSON, treat as plain message
            Serial.print("Raw message from vision: ");
            Serial.println(incoming);
        }
    }
}

string Communication::getCommand() {
    string msg = readCommunication();

    if (msg == "BENEFICIADERO") {
        // TODO: Implement obstacle logic
    }

    return msg;
}

string Communication::readCommunication() {
    String msg;
    
    // Check Serial1
    if (Serial1.available()) {
        msg = Serial1.readStringUntil('\n');
        Serial.print("Vision data: ");
        Serial.println(msg);
        return msg.c_str();
    }
    
    return "";
}

void Communication::startDetection(){
    Serial1.println("START_DETECTION");
    Serial.println("Started detection");
}

void Communication::stopDetection() {
    Serial1.println("STOP_DETECTION");
    Serial.println("Stopped detection");
}

string Communication::getTopBean() {
    return top_bean_;
}

string Communication::getBottomBean() {
    return bottom_bean_;
}

bool Communication::isVisionActive() {
    return vision_active_;
}

void Communication::sendDataToMQTT(const string& data) {
    // Send data to vision system to be forwarded to MQTT
    Serial1.print("MQTT:");
    Serial1.println(data.c_str());
}

void Communication::setState(int state) {
    // Handle state changes if needed
    (void)state;
}