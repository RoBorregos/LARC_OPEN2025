void setup() {
  Serial.begin(115200);       // USB serial (para depurar)
  Serial5.begin(115200);      // UART en pines 20 (RX) y 21 (TX)
  while (!Serial);
  Serial.println("Teensy lista UART5");
}

void loop() {
  if (Serial5.available()) {
    String msg = Serial5.readStringUntil('\n');
    
    Serial5.print("Teensy recibio: ");
    Serial5.println(msg);

    Serial.print("Debug USB: ");
    Serial.println(msg);
  }
}
