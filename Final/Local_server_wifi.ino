#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "209_2.4G";  // Replace with your WiFi SSID 209_5G  TN
const char* password = "Helloworld!";  // Replace with your WiFi password Helloworld!  thathsara59390

const char* serverUrl = "http://192.168.81.57:8080";  // Replace with your server's IP address and port

// Sensor pins
const int fsrPin1 = 36;  // FSR sensor connected to pin 36
const int fsrPin2 = 39;  // FSR sensor connected to pin 39
const int fsrPin3 = 34;  // FSR sensor connected to pin 34
const int fsrPin4 = 35;  // FSR sensor connected to pin 35
const int fsrPin5 = 32;  // FSR sensor connected to pin 32
const int fsrPin6 = 33;  // FSR sensor connected to pin 33

unsigned long previousMillis = 0;
const long interval = 1000;  // Interval in milliseconds (1000 milliseconds or 1 second)

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Initialize sensor pins
  pinMode(fsrPin1, INPUT);
  pinMode(fsrPin2, INPUT);
  pinMode(fsrPin3, INPUT);
  pinMode(fsrPin4, INPUT);
  pinMode(fsrPin5, INPUT);
  pinMode(fsrPin6, INPUT);

  Serial.println("Setup completed.");
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send data
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read sensor values
    float fsrValue1 = analogRead(fsrPin1);
    float fsrValue2 = analogRead(fsrPin2);
    float fsrValue3 = analogRead(fsrPin3);
    float fsrValue4 = analogRead(fsrPin4);
    float fsrValue5 = analogRead(fsrPin5);
    float fsrValue6 = analogRead(fsrPin6);

    // Print debug information to Serial Monitor
    Serial.print("FSR Value1: ");
    Serial.println(fsrValue1);
    Serial.print("FSR Value2: ");
    Serial.println(fsrValue2);
    Serial.print("FSR Value3: ");
    Serial.println(fsrValue3);
    Serial.print("FSR Value4: ");
    Serial.println(fsrValue4);
    Serial.print("FSR Value5: ");
    Serial.println(fsrValue5);
    Serial.print("FSR Value6: ");
    Serial.println(fsrValue6);

    // Send data to server
    sendDataToServer(fsrValue1, fsrValue2, fsrValue3, fsrValue4, fsrValue5, fsrValue6);
  }

  // Other tasks can be performed here
  // Avoid using delay() to keep WiFi and other tasks running smoothly
}

void sendDataToServer(float value1, float value2, float value3, float value4, float value5, float value6) {
  HTTPClient http;

  // Prepare data to send
  String postData = "value1=" + String(value1) +
                    "&value2=" + String(value2) +
                    "&value3=" + String(value3) +
                    "&value4=" + String(value4) +
                    "&value5=" + String(value5) +
                    "&value6=" + String(value6);

  // Send HTTP POST request to server
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(postData);

  // Print HTTP response code to Serial Monitor
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error sending data to server. HTTP error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
