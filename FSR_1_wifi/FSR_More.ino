#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";  // Replace with your WiFi SSID
const char* password = "";  // Replace with your WiFi password

const char* googleSheetsUrl = "";  // Replace with your Google Sheets script URL

int fsrPin1 = 34;  // FSR sensor connected to D34 pin
int fsrPin2 = 32;  // Additional sensor connected to D32 pin
int fsrPin3 = 35;  // Additional sensor connected to D35 pin

unsigned long previousMillis = 0;
const long interval = 1;  // Interval in milliseconds (1000 milliseconds or 1 second)

void setup() {
  Serial.begin(9600);
  delay(1000);

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

    // Send data to Google Sheets
    sendDataToGoogleSheets(fsrValue1, fsrValue2, fsrValue3);  // Send sensor values to respective columns

    // Print debug information
    Serial.print("FSR Value1: ");
    Serial.println(fsrValue1);
    Serial.print("FSR Value2: ");
    Serial.println(fsrValue2);
    Serial.print("FSR Value3: ");
    Serial.println(fsrValue3);
  }

  // Other tasks can be performed here
  // Remember not to use delay() which would block WiFi and other tasks
}

void sendDataToGoogleSheets(float value1, float value2, float value3) {
  HTTPClient http;

  // Prepare data to send
  String postData = "value1=" + String(value1) +
                    "&value2=" + String(value2) +
                    "&value3=" + String(value3);

  // Send HTTP POST request to Google Sheets API
  http.begin(googleSheetsUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.print("Error sending data to Google Sheets. HTTP error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}