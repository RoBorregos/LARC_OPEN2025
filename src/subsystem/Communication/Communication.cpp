#include "Communication.hpp"
#include <string.h>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)
    Serial6.begin(115200);      // UART on pins 25 (RX) and 24 (TX), UART6
}

void Communication::begin() {

}

string Communication::getCommand() {
    string msg = readCommunication();

    if(msg.find('[') != string::npos && msg.find(']') != string::npos){
        // Process matrix data
        // intake_.doPose(msg);
    }else if(msg == "R_BENEFIT"){
        // drive_.goBenefit(R);
    }else if(msg == "B_BENEFIT"){
        // drive_.goBenefit(B);
    }

    return msg;
}


string Communication::readCommunication() {
    String msg;

    if (Serial6.available()) {
        msg = Serial6.readStringUntil('\n');
        Serial6.print("Teensy receive: ");
        Serial6.println(msg);
        Serial.print("Debug USB: ");
        Serial.println(msg);
        return msg.c_str();
    }
    return "";
}

void Communication::update() {
}

void Communication::setState(int state) {
    (void)state;
}