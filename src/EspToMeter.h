#pragma once

#include <ModbusMaster.h>
#include <SoftwareSerial.h>

struct MeterParam{

  uint16_t Voltage;     //Measuring range:80～260V
  uint32_t Current;     //Measuring range: 0～10A(PZEM-004T-10A); 0～100A(PZEM-004T-100A)
  uint32_t Power;       //Measuring range: 0～2.3kW(PZEM-004T-10A); 0～23kW(PZEM-004T-100A) 
  uint32_t Energy;      //Measuring range: 0～9999.99kWh  
  uint16_t Frequency;   //Measuring range: 45Hz～65Hz  
  uint16_t PowerFactor; //Measuring range: 0.00～1.00
  uint16_t AlarmStatus; //

};

uint32_t combineRegisters(uint16_t highRegister, uint16_t lowRegister);
bool readRegisters(ModbusMaster &meter, int address);

// Returns true if successful, false if failed
extern bool initMasterQuery(int address);

// Extern the global MeterParam to access readings
extern MeterParam m;