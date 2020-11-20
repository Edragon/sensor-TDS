#pragma once

#include "Arduino.h"
#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define ReceivedBufferLength 20
#define KValueAddress 8    //the address of the K value stored in the EEPROM
#define AREF  5.0 // default 5.0V on Arduino UNO
#define ADCRANGE 1024.0

class GravityTDS
{
public:
  int tdsSensorPin;   
  float tdsValue25;
  float temperature;
  float ecvalue;
  float voltage;
  float kValue;

  GravityTDS();
  ~GravityTDS();

  void setup();
  void update();
  float getValue();
  float getRawValue();

private:
  float analogValue;
  float ECvalue25;
  unsigned long sampleTimepoint;
};

boolean cmdSerialDataAvailable(void);
byte cmdParse(void);
void tdsCalibration(byte mode);
void tdsCalibrationProcess();



