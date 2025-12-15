#pragma once
#include <ModbusRTU.h>
#include "EspToMeter.h"

struct MeterParam;

const int DE_RE_PIN = 5;
const uint8_t SLAVE_ID = 5;
const int REGISTERS_PER_DEVICE = 10;

extern void initSlave(int deviceCount);
extern void StoreHreg(const MeterParam& m, int deviceIndex);

extern ModbusRTU mb;
