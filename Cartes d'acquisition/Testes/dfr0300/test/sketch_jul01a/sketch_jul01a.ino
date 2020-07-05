
#include "DFRobot_EC.h"

#define EC_PIN A0
float voltage,ecValue,temperature = 25;
DFRobot_EC ec;

void setup()
{
  Serial.begin(115200);  
  ec.begin();
}

void loop()
{
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U)  //time interval: 1s
    {
      timepoint = millis();
      voltage = analogRead(EC_PIN)/1024.0*5000;   // read the voltage
      //temperature = readTemperature();          // read your temperature sensor to execute temperature compensation
      ecValue =  ec.readEC(voltage,temperature);  // convert voltage to EC with temperature compensation
     // Serial.print("temperature:");
     // Serial.print(temperature,1);
      Serial.print("  EC:");
      Serial.print(ecValue,2);
      Serial.println("us/cm");
    }
    ec.calibration(voltage,temperature);          // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
}
