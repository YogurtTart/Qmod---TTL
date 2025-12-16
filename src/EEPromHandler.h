#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include "EspToQmod.h"

// EEPROM configuration structure
struct EEPROMConfig {
    uint16_t magic;         // Magic number to detect initialized EEPROM (0x55AA)
    uint8_t slaveID;        // Modbus slave address (default: 5)
    uint16_t pollInterval;  // Batch polling interval in ms (default: 2000)
    uint16_t timeout;       // Modbus response timeout in ms (default: 200)
};

// Global configuration accessible from all files
extern EEPROMConfig eepromConfig;

// Function declarations
void LoadEEPROM();
void SaveEEPROM();
void CompareAndUpdateEEPROM();
void ResetToDefaults();