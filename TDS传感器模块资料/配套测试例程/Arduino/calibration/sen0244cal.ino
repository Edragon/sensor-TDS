/*
 * 淘宝    ：https://ilovemcu.taobao.com
*/

#include <EEPROM.h>
#include"GravityTDS.h"

extern GravityTDS gravityTDS;

void setup() 
{
  Serial.begin(115200);
  gravityTDS.setup();
}

unsigned long tdsValue;

void loop() 
{
  tdsCalibrationProcess();
  gravityTDS.update();
  
  static unsigned long timepoint = millis();
  if(millis()-timepoint>1000U)
  {
       timepoint = millis();
       tdsValue = gravityTDS.getValue();
       Serial.print(tdsValue);
       Serial.println("ppm");
   }

}
