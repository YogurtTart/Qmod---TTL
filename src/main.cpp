#include "Arduino.h"
#include "EspToMeter.h"
#include "EspToQmod.h"
#include "EEPromHandler.h" 

int device[] = {1, 2, 3};
const int deviceCount = sizeof(device) / sizeof(device[0]);

// State machine states
enum QueryState {
  IDLE,
  QUERYING_DEVICE,
  BATCH_COMPLETE
};

// State machine variables
QueryState currentState = IDLE;
unsigned long batchTimer = 0;
int currentDeviceIndex = 0;
bool queryInProgress = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Modbus Multi-Device Test...");

  // Load EEPROM configuration FIRST
  LoadEEPROM();
  
  if (!initMasterQuery()) {
    Serial.println("❌ ModBus initialization failed!");
  } else {
    Serial.println("✅ ModBus initialized successfully");
  }

  initSlave(deviceCount);
  
  Serial.print("Total devices to query: ");
  Serial.println(deviceCount);
  
  // Initialize timer
  batchTimer = millis();
  currentState = IDLE;
  
  Serial.print("Poll interval: ");
  Serial.print(eepromConfig.pollInterval);
  Serial.println(" ms");
}

void loop() {
  // Always process Modbus slave tasks
  mb.task();
  
  unsigned long currentMillis = millis();
  
  // State machine
  switch (currentState) {
    
    case IDLE:
      // Wait for batch interval to elapse - using eepromConfig.pollInterval
      if (currentMillis - batchTimer >= eepromConfig.pollInterval) {
        Serial.println("=== Starting new batch ===");
        currentDeviceIndex = 0;
        currentState = QUERYING_DEVICE;
        queryInProgress = false;
      }
      break;
      
    case QUERYING_DEVICE:
      if (!queryInProgress) {
        // Start the query
        Serial.print("Querying device ");
        Serial.print(device[currentDeviceIndex]);
        Serial.print(" (");
        Serial.print(currentDeviceIndex + 1);
        Serial.print("/");
        Serial.print(deviceCount);
        Serial.println(")");
        
        mb.task();
        bool success = QueryMeter(device[currentDeviceIndex], currentDeviceIndex);
        mb.task();

        if (success) {
          // Successfully got response
          Serial.print("  ✓ V=");
          Serial.print(m.Voltage);
          Serial.print("V, I=");
          Serial.print(m.currentLow);
          Serial.print("/");
          Serial.print(m.currentHigh);
          Serial.print(", P=");
          Serial.print(m.powerLow);
          Serial.print("/");
          Serial.println(m.powerHigh);
        } else {
          Serial.println("  ✗ Query failed");
        }

      }
      
        // Move to next device
        currentDeviceIndex++;
        
        // Check if batch is complete
        if (currentDeviceIndex >= deviceCount) {
          currentState = BATCH_COMPLETE;
        }
      
      
      break;
      
    case BATCH_COMPLETE:
      Serial.println("=== Batch complete ===");
      Serial.println();
      
      // Check for EEPROM updates at the end of each batch
      CompareAndUpdateEEPROM();
      
      // Reset timer and go back to idle
      batchTimer = currentMillis;
      currentState = IDLE;
      break;
  }
  
  mb.task();
}