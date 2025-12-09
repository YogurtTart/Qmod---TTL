#include "Arduino.h"
#include "EspToMeter.h"

int device[] = {1, 2, 3};
const int deviceCount = sizeof(device) / sizeof(device[0]);

unsigned long previousMillis = 0;
const unsigned long interval = 2000; // 2 second interval
int currentDeviceIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Modbus Multi-Device Test...");

  if (!initMasterQuery()) {
    Serial.println("❌ ModBus initialization failed!");
  } else {
    Serial.println("✅ ModBus initialized successfully");
  }
  
  Serial.print("Total devices to query: ");
  Serial.println(deviceCount);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check if it's time to query the next device
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Query current device
    bool success = QueryMeter(device[currentDeviceIndex]);
    
    // Optional: Print results or handle failures
    if (success) {
      Serial.print("Device ");
      Serial.print(device[currentDeviceIndex]);
      Serial.print(": V=");
      Serial.print(m.Voltage);
      Serial.print(" Alarm = ");
      Serial.println(m.AlarmStatus);
    }
    
    // Move to next device
    currentDeviceIndex++;
    
    // Wrap around to start if we've reached the end
    if (currentDeviceIndex >= deviceCount) {
      currentDeviceIndex = 0;
    }
  }
  
  // Your other non-blocking code can run here
}