#include "Communication.hpp"
#include <string.h>
#include <algorithm>
using namespace std;

Communication::Communication(){
    Serial.begin(9600);       // USB serial
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

void Communication::sendStartSearching() {
    Serial.println("START_SEARCHING");
}

void Communication::sendStopSearching() {
    Serial.println("STOP_SEARCHING");
}

void Communication::sendPowerOff() {
    Serial.println("SHUTDOWN");
}

vector<int> Communication::getMatrix() {
    string s = getCommand();
    if (s.empty()) return {0, 0};

    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());

    size_t comma = s.find(',');
    if (comma == string::npos) return {0, 0};

    string top_str = s.substr(0, comma);
    string bottom_str = s.substr(comma + 1);

    int top = 0;
    int bottom = 0;

    if (!top_str.empty() && all_of(top_str.begin(), top_str.end(), ::isdigit)) {
        top = stoi(top_str);
    }

    if (!bottom_str.empty() && all_of(bottom_str.begin(), bottom_str.end(), ::isdigit)) {
        bottom = stoi(bottom_str);
    }

    top = max(0, min(top, 2));
    bottom = max(0, min(bottom, 2));
    Serial.println(top + bottom);

    return {top, bottom};
}


vector<int> Communication::getBenefitsPositions() {
    string s = getCommand();
    vector<int> positions;


    return positions;
}