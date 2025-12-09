#include <ModbusMaster.h>
#include <SoftwareSerial.h>

SoftwareSerial modbusSerial(12, 13);  // RX=12, TX=13
ModbusMaster meter1, meter2;

void setup() {
  Serial.begin(9600);
  modbusSerial.begin(9600);
  
  meter1.begin(1, modbusSerial);  // Meter address 1
  meter2.begin(2, modbusSerial);  // Meter address 2
  
  Serial.println("READING INPUT REGISTERS...");
}

void loop() {
  // Read Meter 1 - Input Register 0
  if (meter1.readInputRegisters(0, 1) == 0) {
    uint16_t value1 = meter1.getResponseBuffer(0);
    Serial.print("M1: ");
    Serial.println(value1);
  }
  
  delay(100);
  
  // Read Meter 2 - Input Register 0  
  if (meter2.readInputRegisters(0, 1) == 0) {
    uint16_t value2 = meter2.getResponseBuffer(0);
    Serial.print("M2: ");
    Serial.println(value2);
  }
  
  Serial.println("---");
  delay(2000);
}