#include <WiFi.h>
#include <FirebaseESP32.h>  // Ensure this line is present

#define FIREBASE_HOST "https://esp32-36a28-default-rtdb.asia-southeast1.firebasedatabase.app/"  // Your Firebase project URL
#define FIREBASE_AUTH "AIzaSyD4b9OHP8qvJBQb9H10HTWrkute1p4pJds"  // Your database secret

const char* ssid = "Lab902";  // Replace with your WiFi SSID
const char* password = "lab@@902";  // Replace with your WiFi password

int fsrPin1 = 34;  // FSR sensor connected to D34 pin
int fsrPin2 = 32;  // Additional sensor connected to D32 pin
int fsrPin3 = 35;  // Additional sensor connected to D35 pin

unsigned long previousMillis = 0;
const long interval = 1000;  // Interval in milliseconds (1000 milliseconds or 1 second)

FirebaseData firebaseData;
FirebaseConfig firebaseConfig;
FirebaseAuth firebaseAuth;

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

  // Configure Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&firebaseConfig, &firebaseAuth);
  Firebase.reconnectWiFi(true);

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

    // Send data to Firebase
    sendDataToFirebase(fsrValue1, fsrValue2, fsrValue3);  // Send sensor values to respective nodes

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

void sendDataToFirebase(float value1, float value2, float value3) {
  if (Firebase.ready()) {
    // Write data to Firebase
    if (Firebase.setFloat(firebaseData, "/sensor/value1", value1)) {
      Serial.println("Value1 sent successfully");
    } else {
      Serial.print("Failed to send value1, ");
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.setFloat(firebaseData, "/sensor/value2", value2)) {
      Serial.println("Value2 sent successfully");
    } else {
      Serial.print("Failed to send value2, ");
      Serial.println(firebaseData.errorReason());
    }

    if (Firebase.setFloat(firebaseData, "/sensor/value3", value3)) {
      Serial.println("Value3 sent successfully");
    } else {
      Serial.print("Failed to send value3, ");
      Serial.println(firebaseData.errorReason());
    }
  } else {
    Serial.println("Firebase not ready");
  }
}
