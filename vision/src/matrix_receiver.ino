#include <Arduino.h>

void setup() {
    Serial.begin(115200); // Initialize USB serial communication
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Arduino ready to receive matrix data.");
}

void loop() {
    // Check if data is available on the USB serial port
    if (Serial.available() > 0) {
        String incoming = Serial.readStringUntil('\n');
        incoming.trim();

        if (incoming.length() > 0) {
            int matrixVals[2] = { -1, -1 };
            int idx = 0;
            int start = 0;
            for (int i = 0; i <= incoming.length() && idx < 2; ++i) {
                if (i == incoming.length() || incoming.charAt(i) == ',') {
                    String token = incoming.substring(start, i);
                    token.trim();
                    if (token.length() > 0) {
                        matrixVals[idx] = token.toInt();
                    } else {
                        matrixVals[idx] = -1;
                    }
                    idx++;
                    start = i + 1;
                }
            }
        }
    }
}