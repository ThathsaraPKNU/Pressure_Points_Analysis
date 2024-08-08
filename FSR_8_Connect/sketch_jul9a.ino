// Pins connected to FSR sensors on ESP32
const int fsrPins[] = {34, 35, 32, 33, 12, 13, 14, 27};

// Variables to store the readings from FSR sensors
int fsrReadings[8];

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bps
}

void loop() {
  // Read analog values from each FSR sensor
  for (int i = 0; i < 8; i++) {
    fsrReadings[i] = analogRead(fsrPins[i]);
  }

  // Print readings to the serial monitor
  for (int i = 0; i < 8; i++) {
    Serial.print("FSR ");
    Serial.print(i + 1);
    Serial.print(" Reading: ");
    Serial.print(fsrReadings[i]);
    if (i < 7) {
      Serial.print("\t");
    }
  }
  Serial.println();

  delay(100); // Adjust delay as needed
}
