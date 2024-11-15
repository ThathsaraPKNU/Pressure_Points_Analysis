#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h> // Include this to enable notifications

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

// BLE setup
BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
const char *serviceUUID = "12345678-1234-1234-1234-1234567890ab";      // Random UUID for service
const char *characteristicUUID = "abcd1234-abcd-1234-abcd-12345678abcd"; // Random UUID for characteristic

// Callback for client connection status
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    // Initialize MUX control pins
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(Z, INPUT);

    Serial.begin(115200);
    delay(2000);  // Short delay to ensure stable startup

    // Initialize BLE
    BLEDevice::init("ESP32_FSR_BLE");  // Name of your BLE device
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create BLE service and characteristic
    BLEService *pService = pServer->createService(serviceUUID);
    pCharacteristic = pService->createCharacteristic(
                        characteristicUUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

    // Enable notifications for the characteristic
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(serviceUUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // Set advertising interval
    pAdvertising->setMaxPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("BLE service started, waiting for connections...");
}

void loop() {
    // Collect FSR data
    for (int i = 0; i < numFSRs; i++) {
        selectMuxChannel(i);
        delayMicroseconds(10);
        fsrReadings[i] = analogRead(Z);
    }

    // Convert data to a string to send over BLE
    String data = "";
    for (int i = 0; i < numFSRs; i++) {
        data += String(fsrReadings[i]) + ",";
    }
    data.remove(data.length() - 1);  // Remove the trailing comma

    // Only send data if a device is connected
    if (deviceConnected) {
        pCharacteristic->setValue(data.c_str());
        pCharacteristic->notify();  // Send data to connected client
        Serial.println("Sent over BLE: " + data);
    }

    delay(1000);  // Send data every second
}
