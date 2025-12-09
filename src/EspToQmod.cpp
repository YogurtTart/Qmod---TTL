#include "EspToQmod.h"
#include "EspToMeter.h"

ModbusRTU mb;

void initSlave(int deviceCount) {
  pinMode(DE_RE_PIN, OUTPUT);
  
  mb.begin(&Serial, DE_RE_PIN);
  mb.slave(SLAVE_ID);
  
  // Pre-allocate all registers
  int totalRegisters = deviceCount * 10;
  for (int i = 0; i < totalRegisters; i++) {
    mb.addHreg(i, 0);
  }
  
  Serial.print("Allocated ");
  Serial.print(totalRegisters);
  Serial.println(" registers");
}

void StoreHreg(const MeterParam& m, int deviceIndex) {
  // Dynamic calculation - no maxDevices check
  int baseRegister = deviceIndex * REGISTERS_PER_DEVICE;
  
  // Create registers on-the-fly (modbus-esp8266 auto-creates)
  mb.Hreg(baseRegister + 0, m.Voltage);
  mb.Hreg(baseRegister + 1, m.currentLow);
  mb.Hreg(baseRegister + 2, m.currentHigh);
  mb.Hreg(baseRegister + 3, m.powerLow);
  mb.Hreg(baseRegister + 4, m.powerHigh);
  mb.Hreg(baseRegister + 5, m.energyLow);
  mb.Hreg(baseRegister + 6, m.energyHigh);
  mb.Hreg(baseRegister + 7, m.PowerFactor);
  mb.Hreg(baseRegister + 8, m.Frequency);
  mb.Hreg(baseRegister + 9, m.AlarmStatus);
  
  // Optional debug
  Serial.print("Stored device ");
  Serial.print(deviceIndex);
  Serial.print(" at Hreg ");
  Serial.println(baseRegister);
}

void loopRTU() {
  mb.task();
  yield();
}