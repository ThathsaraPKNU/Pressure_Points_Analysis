#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";  // Replace with your WiFi SSID
const char* password = "2";  // Replace with your WiFi password

const char* googleSheetsUrl = "";  // Replace with your Google Sheets script URL

int fsrPin = 34;  // FSR sensor connected to D34 pin

unsigned long previousMillis = 0;
const long interval = 0.0001;  // Interval in milliseconds (1 millisecond)

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

  // Initialize FSR pin
  pinMode(fsrPin, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Check if it's time to send data
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read FSR sensor value
    float fsrValue = analogRead(fsrPin);

    // Send data to Google Sheets
    sendDataToGoogleSheets(fsrValue, 0.0, 0.0);  // Send FSR value as value1, value2 and value3 as 0

    // Print debug information
    Serial.print("FSR Value: ");
    Serial.println(fsrValue);
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
