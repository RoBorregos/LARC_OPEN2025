#include "Communication.hpp"
#include <string.h>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial (para debuggear)
    Serial6.begin(115200);      // UART on pins 25 (RX) and 24 (TX), UART6
}

void Communication::begin() {
    mode_ = Mode::START;
    startMode();
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

void Communication::sendMessage(const string &msg){
    Serial6.print(msg.c_str());
    if(msg.back() != '\n') Serial6.print('\n');
    Serial.print("Sent to Jetson: ");
    Serial.println(msg.c_str());
}

void Communication::startMode(){
    sendMessage("START");
}

void Communication::sendData(const string &payload){
    string msg = string("DATA:") + payload;
    sendMessage(msg);

    string reply = readCommunication();
    if(!reply.empty()){
        Serial.print("Jetson reply: ");
        Serial.println(reply.c_str());
    }
}