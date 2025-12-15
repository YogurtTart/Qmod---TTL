#include "EspToMeter.h"

SoftwareSerial s(12, 13);

ModbusMaster meter;
MeterParam m;

// void preTransmission() {/* TTL doesn't need this */}
// void postTransmission() {/* TTL doesn't need this */}

bool readRegisters(ModbusMaster &meter, int address, int index) {  
  // Read all registers (0-9)
  uint8_t result = meter.readInputRegisters(0, 10);
  
  if (result != meter.ku8MBSuccess) {
    Serial.print("Failed to read registers from device at address ");
    Serial.println(address);

    // Check specific errors
    if (result == meter.ku8MBResponseTimedOut) {
      Serial.println("Timeout (no response)");
    } else if (result == meter.ku8MBIllegalFunction) {
      Serial.println("Illegal function");
    } else if (result == meter.ku8MBIllegalDataAddress) {
      Serial.println("Invalid register address");
    } else if (result == meter.ku8MBIllegalDataValue) {
      Serial.println("Invalid data value");
    } else if (result == meter.ku8MBSlaveDeviceFailure) {
      Serial.println("Slave device failure");
    } else if (result == meter.ku8MBInvalidSlaveID) {
      Serial.println("Invalid slave ID");
    } else if (result == meter.ku8MBInvalidFunction) {
      Serial.println("Invalid function");
    } else if (result == meter.ku8MBInvalidCRC) {
      Serial.println("CRC error");
    } else {
      Serial.print("Unknown error: 0x");
      Serial.println(result, HEX);
    }

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

  mb.task();
  StoreHreg(m, index);
  
  // Print success message
  Serial.print("Successfully read data from device ");
  Serial.println(address);
  
  return true;
}

bool initMasterQuery(){

  s.begin(9600);
  s.setTimeout(100);

  meter.begin(1, s);
  // meter.preTransmission(preTransmission);
  // meter.postTransmission(postTransmission);
  meter.clearResponseBuffer();
  meter.clearTransmitBuffer();
  
  Serial.println("✅ Modbus initialized");
  return true;
}

bool QueryMeter(int address, int index) {
  // Setup meter
  meter.begin(address, s);
  meter.clearResponseBuffer();
  meter.clearTransmitBuffer();

  mb.task();
  bool success = readRegisters(meter, address, index);
  mb.task();

  return success;
}