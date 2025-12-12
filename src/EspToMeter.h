#pragma once

#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include "EspToQmod.h"

struct MeterParam{
  uint16_t Voltage;     //Measuring range:80～260V
  uint16_t currentLow, currentHigh;     //Measuring range: 0～10A(PZEM-004T-10A); 0～100A(PZEM-004T-100A)
  uint16_t powerLow, powerHigh;       //Measuring range: 0～2.3kW(PZEM-004T-10A); 0～23kW(PZEM-004T-100A) 
  uint16_t energyLow, energyHigh;      //Measuring range: 0～9999.99kWh  
  uint16_t Frequency;   //Measuring range: 45Hz～65Hz  
  uint16_t PowerFactor; //Measuring range: 0.00～1.00
  uint16_t AlarmStatus; //
};

bool readRegisters(ModbusMaster &meter, int address, int index);

// Initialize Modbus master
extern bool initMasterQuery();

// Execute query (returns immediately with success/fail)
extern bool QueryMeter(int address, int index);

// Extern the global MeterParam to access readings
extern MeterParam m;