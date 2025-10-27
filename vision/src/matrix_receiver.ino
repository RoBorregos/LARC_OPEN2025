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
            Serial.print("Received matrix: ");
            Serial.println(incoming);

            // Echo the received matrix back
            Serial.print("Echoing back: ");
            Serial.println(incoming);
        }
    }
}