#include "EspToMeter.h"

SoftwareSerial s(12, 13);

ModbusMaster meter;
MeterParam m;

// Function to combine two 16-bit registers into 32-bit
uint32_t combineRegisters(uint16_t highRegister, uint16_t lowRegister) {
    return ((uint32_t)highRegister << 16) | lowRegister;
}

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
  uint16_t currentLow = meter.getResponseBuffer(1);
  uint16_t currentHigh = meter.getResponseBuffer(2);
  m.Current = combineRegisters(currentHigh, currentLow);
  
  // Combine low and high 16-bit for Power (registers 3 & 4)
  uint16_t powerLow = meter.getResponseBuffer(3);
  uint16_t powerHigh = meter.getResponseBuffer(4);
  m.Power = combineRegisters(powerHigh, powerLow);
  
  // Combine low and high 16-bit for Energy (registers 5 & 6)
  uint16_t energyLow = meter.getResponseBuffer(5);
  uint16_t energyHigh = meter.getResponseBuffer(6);
  m.Energy = combineRegisters(energyHigh, energyLow);
  
  // Store single 16-bit values
  m.PowerFactor = meter.getResponseBuffer(7);
  m.Frequency = meter.getResponseBuffer(8);
  m.AlarmStatus = meter.getResponseBuffer(9);
  
  // Print success message
  Serial.print("Successfully read data from device ");
  Serial.println(address);
  
  return true;
}

bool initMasterQuery(int address){
  
  // Set the device address
  meter.begin(address, s);
  
  // Clear any pending data in serial buffer
  while(s.available() > 0) {
    s.read();
  }
  
  // Clear the response buffer
  meter.clearResponseBuffer();
  
  // Small delay to ensure clean state
  delay(10);
  
  // Test connection by reading register 0 (voltage)
  uint8_t result = meter.readInputRegisters(0, 1);
  
  if (result != meter.ku8MBSuccess) {
    Serial.print("Failed to connect to device at address ");
    Serial.println(address);
    return false;
  }
  
  // Connection successful, read all registers
  return readRegisters(meter, address);
}