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
    if (Serial.available()) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        return string(msg.c_str()); 
    }
    
    return "";
}

void Communication::sendStartPicking() {
    Serial.println("START_PICKING");
}

void Communication::sendStopPicking() {
    Serial.println("STOP_PICKING");
}

void Communication::sendStartSearching() {
    Serial.println("START_SEARCHING");
}

void Communication::sendStopSearching() {
    Serial.println("STOP_SEARCHING");
}

vector<int> Communication::getMatrix() {
    string s = getCommand();
    if (s.empty()) return {-1, -1};
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    size_t comma = s.find(',');
    if (comma == string::npos) return {-1, -1};

    int top = stoi(s.substr(0, comma));
    int bottom = stoi(s.substr(comma + 1));

    return {top, bottom};
}

vector<int> Communication::getBenefitsPositions() {
    string s = getCommand();
    vector<int> positions;


    return positions;
}