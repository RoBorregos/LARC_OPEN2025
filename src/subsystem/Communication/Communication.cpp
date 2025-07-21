#include "Communication.hpp"
#include <string.h>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)

    // UARTs on Teensy 4.1
    Serial1.begin(115200);      // UART on pins 0 (RX) and 1 (TX), UART1
    Serial2.begin(115200);      // UART on pins 7 (RX) and 8 (TX), UART2
    Serial3.begin(115200);      // UART on pins 15 (RX) and 14 (TX), UART3
    Serial4.begin(115200);      // UART on pins 16 (RX) and 17 (TX), UART4
    Serial5.begin(115200);      // UART on pins 20 (RX) and 21 (TX), UART5
    Serial6.begin(115200);      // UART on pins 25 (RX) and 24 (TX), UART6
    Serial7.begin(115200);      // UART on pins 28 (RX) and 29 (TX), UART7
    Serial8.begin(115200);      // UART on pins 34 (RX) and 35 (TX), UART8
}

string Communication::getCommand() {
    string msg = readCommunication();

    if (msg == "TREE") {
        // TODO Whats the logic behind
    } else if (msg == "BEAN") {
        
    }
    else {
    }

    return msg;
}


string Communication::readCommunication() {
    String msg;
    if (Serial1.available()) {
        msg = Serial1.readStringUntil('\n');
        Serial1.print("Teensy receive: ");
        Serial1.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial2.available()) {
        msg = Serial2.readStringUntil('\n');
        Serial2.print("Teensy receive: ");
        Serial2.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial3.available()) {
        msg = Serial3.readStringUntil('\n');
        Serial3.print("Teensy receive: ");
        Serial3.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial4.available()) {
        msg = Serial4.readStringUntil('\n');
        Serial4.print("Teensy receive: ");
        Serial4.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial5.available()) {
        msg = Serial5.readStringUntil('\n');
        Serial5.print("Teensy receive: ");
        Serial5.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial6.available()) {
        msg = Serial6.readStringUntil('\n');
        Serial6.print("Teensy receive: ");
        Serial6.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial7.available()) {
        msg = Serial7.readStringUntil('\n');
        Serial7.print("Teensy receive: ");
        Serial7.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    if (Serial8.available()) {
        msg = Serial8.readStringUntil('\n');
        Serial8.print("Teensy receive: ");
        Serial8.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    return "";
}