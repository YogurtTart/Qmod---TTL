#include "EEPromHandler.h"


// Define the global configuration variable
EEPROMConfig eepromConfig;

// EEPROM address where config is stored
#define EEPROM_CONFIG_ADDR 0
#define EEPROM_MAGIC 0x55AA  // Magic number to detect initialized EEPROM

// Default values
#define DEFAULT_SLAVE_ID 1
#define DEFAULT_POLL_INTERVAL 2000
#define DEFAULT_TIMEOUT 200

// Reset to default values and save to EEPROM
void ResetToDefaults() {
    eepromConfig.magic = EEPROM_MAGIC;
    eepromConfig.slaveID = DEFAULT_SLAVE_ID;
    eepromConfig.pollInterval = DEFAULT_POLL_INTERVAL;
    eepromConfig.timeout = DEFAULT_TIMEOUT;
    
    SaveEEPROM();
    Serial.println("Reset to default configuration");
}

// Load configuration from EEPROM or create defaults
void LoadEEPROM() {
    EEPROM.begin(sizeof(EEPROMConfig));
    
    // Try to load from EEPROM
    EEPROM.get(EEPROM_CONFIG_ADDR, eepromConfig);
    
    // Check magic number first - ensures EEPROM was properly initialized
    bool useDefaults = false;
    
    if (eepromConfig.magic != EEPROM_MAGIC) {
        useDefaults = true;
        Serial.println("Magic number not found - first boot or corrupted EEPROM");
    }
    // Keep range validation as additional safety
    else if (eepromConfig.slaveID == 0 || eepromConfig.slaveID > 247) {
        useDefaults = true;
        Serial.println("Invalid slave ID in EEPROM");
    }
    else if (eepromConfig.pollInterval < 100 || eepromConfig.pollInterval > 10000) {
        useDefaults = true;
        Serial.println("Invalid poll interval in EEPROM");
    }
    else if (eepromConfig.timeout < 50 || eepromConfig.timeout > 1000) {
        useDefaults = true;
        Serial.println("Invalid timeout in EEPROM");
    }
    
    if (useDefaults) {
        // Use default values
        ResetToDefaults();
        Serial.println("Using default configuration");
    } else {
        Serial.println("EEPROM configuration loaded");
    }
    
    // Print loaded values for debugging
    Serial.print("Slave ID: ");
    Serial.println(eepromConfig.slaveID);
    Serial.print("Poll Interval: ");
    Serial.println(eepromConfig.pollInterval);
    Serial.print("Timeout: ");
    Serial.println(eepromConfig.timeout);
}

// Save current configuration to EEPROM
void SaveEEPROM() {
    // Ensure magic number is set
    eepromConfig.magic = EEPROM_MAGIC;
    
    EEPROM.put(EEPROM_CONFIG_ADDR, eepromConfig);
    EEPROM.commit();
    
    Serial.println("Configuration saved to EEPROM"); 
}

// Compare current EEPROM values with Hreg values and update if different
void CompareAndUpdateEEPROM() {
    bool changed = false;
    bool IDchanged = false;
    
    // Check each register and update if changed
    if (eepromConfig.slaveID != mb.Hreg(0)) {
        eepromConfig.slaveID = mb.Hreg(0);
        changed = true;
        IDchanged = true;
        Serial.println("Slave ID changed");
    }
    
    if (eepromConfig.pollInterval != mb.Hreg(1)) {
        eepromConfig.pollInterval = mb.Hreg(1);
        changed = true;
        Serial.println("Poll Interval changed");
    }
    
    if (eepromConfig.timeout != mb.Hreg(2)) {
        eepromConfig.timeout = mb.Hreg(2);
        changed = true;
        Serial.println("Timeout changed");
    }
    
    // If any value changed, save to EEPROM
    if (changed) {
        SaveEEPROM();
    }

    if(IDchanged){
        initSlave(deviceCount);
    }
}