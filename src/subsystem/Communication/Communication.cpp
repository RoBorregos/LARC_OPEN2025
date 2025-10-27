#include "Communication.hpp"
#include <string.h>
#include <algorithm>
using namespace std;

Communication::Communication(){
    Serial.begin(115200);       // USB serial
}

void Communication::begin() {
    Serial.println("TEENSY READY");
}

void Communication::update() {
}

void Communication::setState(int state) {
    (void)state;
}

string Communication::getCommand() {
    string msg = readCommunication();

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


void Communication::sendMessage(const string &msg){
    Serial.println(msg.c_str());
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

vector<int> Communication::getMatrix(const string &msg) {
    string s = msg;

    if (!s.empty() && s.front() == '[') s.erase(0,1);
    if (!s.empty() && s.back() == ']') s.pop_back();

    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    size_t comma = s.find(',');
    if (comma == string::npos) return {-1, -1};

    string topStr = s.substr(0, comma);
    string bottomStr = s.substr(comma + 1);

    int valTop = -1, valBottom = -1;
    try {
        valTop = std::stoi(topStr);
        valBottom = std::stoi(bottomStr);
    } catch (...) {
        return {-1, -1};
    }

    return {valTop, valBottom};
}