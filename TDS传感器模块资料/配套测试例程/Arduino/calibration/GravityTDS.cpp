#include "GravityTDS.h"
#include "Arduino.h"

char cmdReceivedBuffer[ReceivedBufferLength+1];   // store the serial cmd from the serial monitor
byte cmdReceivedBufferIndex = 0;

GravityTDS::GravityTDS():tdsSensorPin(A1)
{
  sampleTimepoint = 0;
}

GravityTDS::~GravityTDS()
{
}

void GravityTDS::setup()
{
    pinMode(tdsSensorPin,INPUT);
    temperature = 25;
    EEPROM_read(KValueAddress, kValue);  
    if(EEPROM.read(KValueAddress)==0xFF && EEPROM.read(KValueAddress+1)==0xFF && EEPROM.read(KValueAddress+2)==0xFF && EEPROM.read(KValueAddress+3)==0xFF)
    {
      kValue = 1.0;   // default value: K = 1.0
      EEPROM_write(KValueAddress, kValue);
    }
}

void GravityTDS::update()
{
   if(millis()-sampleTimepoint > 500U)  // every seconds, read the data from sensors and print them
   {
        sampleTimepoint=millis();
        analogValue = analogRead(tdsSensorPin);
        voltage = analogValue/(float)(ADCRANGE)*(float)(AREF);//convert ADC value to real voltage; AREF:5.0V
        //Serial.print(analogValue);
        //Serial.print("  ");
        ecvalue=(133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage)*kValue;
        ECvalue25  =  ecvalue / (1.0+0.02*(temperature-25.0));
        //Serial.print(ECvalue25/2.0,0);  //tds = ec / 2
        //Serial.println("ppm");
   }
}

float GravityTDS::getRawValue()
{
    return (133.42*voltage*voltage*voltage - 255.86*voltage*voltage + 857.39*voltage);
}

float GravityTDS::getValue()
{
    tdsValue25 = ECvalue25/2.0;
    return tdsValue25;
}

GravityTDS gravityTDS;

boolean cmdSerialDataAvailable(void)
{
  char cmdReceivedChar;
  static unsigned long cmdReceivedTimeOut = millis();
  while (Serial.available()>0) 
  {   
    if (millis() - cmdReceivedTimeOut > 500U) 
    {
      cmdReceivedBufferIndex = 0;
      memset(cmdReceivedBuffer,0,(ReceivedBufferLength+1));
    }
    cmdReceivedTimeOut = millis();
    cmdReceivedChar = Serial.read();
    if (cmdReceivedChar == '\n' || cmdReceivedBufferIndex==ReceivedBufferLength){
    cmdReceivedBufferIndex = 0;
    strupr(cmdReceivedBuffer);
    return true;
    }else{
      cmdReceivedBuffer[cmdReceivedBufferIndex] = cmdReceivedChar;
      cmdReceivedBufferIndex++;
    }
  }
  return false;
}


byte cmdParse(void)
{
  byte modeIndex = 0;
  if(strstr(cmdReceivedBuffer, "ENTER") != NULL) 
      modeIndex = 1;
  else if(strstr(cmdReceivedBuffer, "EXIT") != NULL) 
      modeIndex = 3;
  else if(strstr(cmdReceivedBuffer, "CAL:") != NULL)   
      modeIndex = 2;
  return modeIndex;
}


void tdsCalibration(byte mode)
{
    char *cmdReceivedBufferPtr;
    static boolean ecCalibrationFinish = 0;
    static boolean enterCalibrationFlag = 0;
    float KValueTemp,rawECsolution;
    switch(mode)
    {
      case 0:
      if(enterCalibrationFlag)
         Serial.println(F("Command Error"));
      break;
      
      case 1:
      enterCalibrationFlag = 1;
      ecCalibrationFinish = 0;
      Serial.println();
      Serial.println(F(">>>Enter Calibration Mode<<<"));
      Serial.println(F(">>>Please put the probe into the stand buffer solution<<<"));
      Serial.println();
      break;
     
      case 2:
      cmdReceivedBufferPtr=strstr(cmdReceivedBuffer, "CAL:");
      cmdReceivedBufferPtr+=strlen("CAL:");
      rawECsolution = strtod(cmdReceivedBufferPtr,NULL);
      if(enterCalibrationFlag)
      {
          Serial.print("rawECsolution:");
          Serial.print(rawECsolution);
          Serial.print("  ecvalue:");
          Serial.println(gravityTDS.ecvalue);
          KValueTemp = rawECsolution*2/gravityTDS.getRawValue();  //calibrate in the  buffer solution, such as 1413us/cm
          if((rawECsolution>0) && (rawECsolution<2000) && (KValueTemp>0.2) && (KValueTemp<5.0))
          {
              Serial.println();
              Serial.print(F(">>>Confrim Successful,K:"));
              Serial.print(KValueTemp);
              Serial.println(F(", Send EXIT to Save and Exit<<<"));
              gravityTDS.kValue =  KValueTemp;
              ecCalibrationFinish = 1;
          }
          else{
            Serial.println();
            Serial.println(F(">>>Confirm Failed,Try Again<<<"));
            Serial.println();
            ecCalibrationFinish = 0;
          }        
      }
      break;

        case 3:
        if(enterCalibrationFlag)
        {
            Serial.println();
            if(ecCalibrationFinish)
            {
               EEPROM_write(KValueAddress, gravityTDS.kValue);
               Serial.print(F(">>>Calibration Successful,K Value Saved,"));
            }
            else Serial.print(F(">>>Calibration Failed"));       
            Serial.println(F(",Exit Calibration Mode<<<"));
            Serial.println();
            ecCalibrationFinish = 0;
            enterCalibrationFlag = 0;
        }
        break;
    }
}

void tdsCalibrationProcess()
{
   if(cmdSerialDataAvailable() > 0)  // if received serial cmd from the serial monitor, enter into the calibration mode; Calibration Buffer Solution: 1413us/cm
   {
      tdsCalibration(cmdParse());  
   }
}
