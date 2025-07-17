#include "Communication.hpp"
#include <string.h>

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)
    Serial5.begin(115200);      // UART en pines 20 (RX) y 21 (TX) este es UART5
    while (!Serial);
    Serial.println("Teensy lista UART5");
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


string Communication::readCommunication(){
    if (Serial5.available()) {
        String msg = Serial5.readStringUntil('\n');
        
        Serial5.print("Teensy recibio: ");
        Serial5.println(msg);

        Serial.print("Debug USB: ");
        Serial.println(msg);
    }
}