#include "Arduino.h"
#include "EspToMeter.h"

int device[] = {1, 2, 3};
const int deviceCount = sizeof(device) / sizeof(device[0]); // Dynamic array size

unsigned long previousMillis = 0;
const unsigned long interval = 2000; // 2 second interval
int currentDeviceIndex = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check if it's time to query the next device
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Query current device
    bool success = initMasterQuery(device[currentDeviceIndex]);
    
    if (success) {
      // Access the meter readings from the global 'm' struct
      // Example: Serial.println(m.Voltage);
      // Example: Serial.println(m.Current);
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