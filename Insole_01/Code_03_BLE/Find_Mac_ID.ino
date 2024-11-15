#include <BLEDevice.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE...");

    // Initialize BLE and get the MAC address
    BLEDevice::init("ESP32_FSR_BLE");
    String macAddress = BLEDevice::getAddress().toString().c_str();
    Serial.print("MAC Address: ");
    Serial.println(macAddress);
}

void loop() {
    // You can leave this empty if you only need to print the MAC address once
}
