#include "EspToQmod.h"

ModbusRTU mb;

void initSlave(int deviceCount) {
  pinMode(DE_RE_PIN, OUTPUT);
  
  // Use eepromConfig.slaveID instead of SLAVE_ID constant
  mb.begin(&Serial, DE_RE_PIN);
  mb.slave(eepromConfig.slaveID); 
  
  // Pre-allocate all registers
  int totalRegisters = (deviceCount * REGISTERS_PER_DEVICE) + 10;
  for (int i = 0; i < totalRegisters; i++) {
    mb.addHreg(i, 0);
  }
  
  // Initialize configuration registers with current EEPROM values
  mb.Hreg(0, eepromConfig.slaveID);       
  mb.Hreg(1, eepromConfig.pollInterval);   
  mb.Hreg(2, eepromConfig.timeout);       

  Serial.print("Allocated ");
  Serial.print(totalRegisters);
  Serial.println(" registers");
  
  Serial.print("Slave ID set to: ");
  Serial.println(eepromConfig.slaveID);
}

void StoreHreg(const MeterParam& m, int deviceIndex) {
  // Dynamic calculation - no maxDevices check
  int baseRegister = deviceIndex * REGISTERS_PER_DEVICE;
  
  // Create registers on-the-fly (modbus-esp8266 auto-creates)
  mb.Hreg(baseRegister + 10, m.Voltage);
  mb.Hreg(baseRegister + 11, m.currentLow);
  mb.Hreg(baseRegister + 12, m.currentHigh);
  mb.Hreg(baseRegister + 13, m.powerLow);
  mb.Hreg(baseRegister + 14, m.powerHigh);
  mb.Hreg(baseRegister + 15, m.energyLow);
  mb.Hreg(baseRegister + 16, m.energyHigh);
  mb.Hreg(baseRegister + 17, m.PowerFactor);
  mb.Hreg(baseRegister + 18, m.Frequency);
  mb.Hreg(baseRegister + 19, m.AlarmStatus);
  
  // Optional debug
  Serial.print("Stored device ");
  Serial.print(deviceIndex);
  Serial.print(" at Hreg ");
  Serial.println(baseRegister);
}