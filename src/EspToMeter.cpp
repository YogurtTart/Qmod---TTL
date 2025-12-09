#include "EspToMeter.h"

SoftwareSerial s(12, 13);

ModbusMaster meter;
MeterParam m;

// void preTransmission() {/* TTL doesn't need this */}
// void postTransmission() {/* TTL doesn't need this */}

bool readRegisters(ModbusMaster &meter, int address) {  
  // Read all registers (0-9)
  uint8_t result = meter.readInputRegisters(0, 10);
  
  if (result != meter.ku8MBSuccess) {
    Serial.print("Failed to read registers from device at address ");
    Serial.println(address);
    return false;
  }
  
  // Store all register values into MeterParam struct
  m.Voltage = meter.getResponseBuffer(0);
  
  // Combine low and high 16-bit for Current (registers 1 & 2)
  m.currentLow = meter.getResponseBuffer(1);
  m.currentHigh = meter.getResponseBuffer(2);
  
  // Combine low and high 16-bit for Power (registers 3 & 4)
  m.powerLow = meter.getResponseBuffer(3);
  m.powerHigh = meter.getResponseBuffer(4);
  
  // Combine low and high 16-bit for Energy (registers 5 & 6)
  m.energyLow = meter.getResponseBuffer(5);
  m.energyHigh = meter.getResponseBuffer(6);
  
  // Store single 16-bit values
  m.PowerFactor = meter.getResponseBuffer(7);
  m.Frequency = meter.getResponseBuffer(8);
  m.AlarmStatus = meter.getResponseBuffer(9);
  
  // Print success message
  Serial.print("Successfully read data from device ");
  Serial.println(address);
  
  return true;
}

bool initMasterQuery(){

  s.begin(9600);

  meter.begin(1, s);
  // meter.preTransmission(preTransmission);
  // meter.postTransmission(postTransmission);
    
  Serial.println("✅ Modbus initialized");
  return true;
}

bool QueryMeter(int address){  // Changed return type to bool
  Serial.print("Querying device ");
  Serial.println(address);
  meter.begin(address, s);
  
  // meter.preTransmission(preTransmission);
  // meter.postTransmission(postTransmission);
  meter.clearResponseBuffer();
  meter.clearTransmitBuffer();
  
  // Small delay to ensure clean state
  delay(300);

  bool success = readRegisters(meter, address);
  
  // Add minimum delay between Modbus queries
  delay(100);
  
  return success;
}