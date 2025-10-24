#pragma once

#include <Arduino.h>
#include <functional>

class LineSensorChecker {
public:
    LineSensorChecker(std::function<bool()> leftSensor, std::function<bool()> rightSensor, double toleranceSec = 0.3)
        : leftSensor(leftSensor), rightSensor(rightSensor),
          toleranceMs(toleranceSec * 1000), state(State::IDLE), startTime(0) {}

    // Call this every loop cycle
    bool update() {
        bool left = leftSensor();
        bool right = rightSensor();

        switch (state) {
            case State::IDLE:
                if (left && right) return true;
                if (left || right) {
                    startTime = millis();
                    firstWasLeft = left;
                    state = State::WAITING_SECOND;
                }
                break;

            case State::WAITING_SECOND:
                if (left && right) {
                    reset();
                    return true;
                }
                if (millis() - startTime > toleranceMs) {
                    reset();  // timeout
                }
                break;
        }

        return false;
    }

    void reset() { state = State::IDLE; }

private:
    enum class State { IDLE, WAITING_SECOND };

    std::function<bool()> leftSensor;
    std::function<bool()> rightSensor;
    unsigned long toleranceMs;
    unsigned long startTime;
    bool firstWasLeft;
    State state;
};
