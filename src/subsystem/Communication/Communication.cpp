#include "Communication.hpp"
#include <string.h>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)

    // UARTs on Teensy 4.1
    Serial1.begin(115200);      // UART on pins 0 (RX) and 1 (TX), UART1 - Raspberry Pi Vision
    Serial2.begin(115200);      // UART on pins 7 (RX) and 8 (TX), UART2
    Serial3.begin(115200);      // UART on pins 15 (RX) and 14 (TX), UART3
}

void Communication::begin() {
    // Send initialization message to vision system
    Serial1.println("TEENSY_READY");
}

void Communication::update() {
    
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

void Communication::setState(int state) {
    // Handle state changes if needed
    (void)state;
}