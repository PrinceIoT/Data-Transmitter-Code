// ESP32 code
#define RXp2 16
#define TXp2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); // Initialize Serial2 for communication with Arduino Mega
}

void loop() {
  if (Serial2.available() > 0) {
    // Read data from Arduino Mega via Serial2
    String dataFromMega = Serial2.readStringUntil('\n');
    Serial.print("Received Data from Mega: ");
    Serial.println(dataFromMega);
    // You can process the data or take any action here
  }
}
