#include "BluetoothSerial.h"  // Include the Bluetooth library for ESP32

BluetoothSerial SerialBT;     // Initialize the Bluetooth Serial object

// MUX control pins
const int S0 = 25;
const int S1 = 26;
const int S2 = 27;
const int Z = 33;

// Number of FSR sensors
const int numFSRs = 8;
int fsrReadings[numFSRs];

// Function to select MUX channel
void selectMuxChannel(int channel) {
    digitalWrite(S0, channel & 0x01);
    digitalWrite(S1, (channel >> 1) & 0x01);
    digitalWrite(S2, (channel >> 2) & 0x01);
}

void setup() {
    // Initialize MUX control pins as outputs
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    
    // Initialize MUX output pin as input
    pinMode(Z, INPUT);
    
    // Start Serial communication for debugging
    Serial.begin(115200);
    Serial.println("Starting Bluetooth...");

    // Start Bluetooth Serial communication
    SerialBT.begin("ESP32_FSR");  // You can name it as desired
    Serial.println("Bluetooth is ready. Connect to 'ESP32_FSR'");
}

void loop() {
    // Read each FSR sensor connected to the MUX
    for (int i = 0; i < numFSRs; i++) {
        selectMuxChannel(i);
        delayMicroseconds(10);  // Wait for MUX to stabilize
        fsrReadings[i] = analogRead(Z);  // Read analog value
    }

    // Prepare data string
    String data = "FSR Readings: ";
    for (int i = 0; i < numFSRs; i++) {
        data += "value" + String(i + 1) + "=" + String(fsrReadings[i]) + " ";
    }

    // Send data over Bluetooth
    SerialBT.println(data);

    // Print data to Serial Monitor for debugging
    Serial.println(data);

    delay(1000);  // Wait for 1 second before the next reading
}
