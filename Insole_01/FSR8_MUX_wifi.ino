#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi Configuration
const char* ssid = "209_2.4G";         // Replace with your WiFi SSID
const char* password = "Helloworld!";  // Replace with your WiFi Password

const char* serverUrl = "http://192.168.0.6:8000";  // Update to your computer's IP address

// MUX control pins
const int S0 = 25; // MUX control pin S0
const int S1 = 26; // MUX control pin S1
const int S2 = 27; // MUX control pin S2
const int Z = 33;  // MUX output pin connected to ESP32 analog input pin (ADC)

// Number of FSR sensors
const int numFSRs = 8;

// Array to store the readings from FSR sensors
int fsrReadings[numFSRs];

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
    
    // Initialize serial communication
    Serial.begin(115200);
    
    // Connect to Wi-Fi
    Serial.print("Connecting to Wi-Fi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to Wi-Fi");
}

void loop() {
    // Loop through each FSR sensor connected to the MUX
    for (int i = 0; i < numFSRs; i++) {
        selectMuxChannel(i);
        delayMicroseconds(10); // Wait for MUX to stabilize
        fsrReadings[i] = analogRead(Z); // Read analog value
    }
    
    // Prepare data to send
    String postData = "value1=" + String(fsrReadings[0]) + "&value2=" + String(fsrReadings[1]) +
                      "&value3=" + String(fsrReadings[2]) + "&value4=" + String(fsrReadings[3]) +
                      "&value5=" + String(fsrReadings[4]) + "&value6=" + String(fsrReadings[5]) +
                      "&value7=" + String(fsrReadings[6]) + "&value8=" + String(fsrReadings[7]);

    // Debug print
    Serial.print("Sending FSR readings: ");
    Serial.println(postData);

    // Send data to server
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Send HTTP POST request
        int httpResponseCode = http.POST(postData);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response:");
            Serial.println(response);
        } else {
            Serial.print("Error sending data to server. HTTP error code: ");
            Serial.println(httpResponseCode);
        }

        // End HTTP connection
        http.end();
    } else {
        Serial.println("Wi-Fi not connected. Skipping data send.");
    }

    delay(1000); // Wait for 1 second before next POST
}
