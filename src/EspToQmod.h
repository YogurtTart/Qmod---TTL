#pragma once
#include <ModbusRTU.h>
#include "EspToMeter.h"
#include "EEPromHandler.h"

struct MeterParam;

const int DE_RE_PIN = 5;

const int REGISTERS_PER_DEVICE = 10;

extern void initSlave(int deviceCount);
extern void StoreHreg(const MeterParam& m, int deviceIndex);

extern const int deviceCount;
extern ModbusRTU mb;