/***********Notice and Trouble shooting***************
 1. This code is tested on Arduino Uno with Arduino IDE 1.0.5 r2 and 1.8.2.
 2. Calibration CMD:
     enter -> enter the calibration mode
     cal:tds value -> calibrate with the known tds value(25^c). e.g.cal:707
     exit -> save the parameters and exit the calibration mode
 ****************************************************/

#include <EEPROM.h>
#include "GravityTDS.h"

#define phSens A1
#define tdsSens A2
GravityTDS gravityTds;

#define phPower 2
#define phGround 3
#define tdsGround 4
#define tdsPower 5

int tds[10],ph[10],temp;
float temperature = 25,tdsValue = 0;
unsigned long int tdsAvgValue;
unsigned long int phAvgValue;

void setup()
{
  Serial.begin(115200);
  Serial.println('Ready');
  gravityTds.setPin(tdsSens);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  pinMode(phPower, OUTPUT);
  pinMode(phGround, OUTPUT);
  pinMode(tdsGround, OUTPUT);
  pinMode(tdsPower, OUTPUT);
}

void loop()
{
  digitalWrite(tdsPower, HIGH);
  digitalWrite(tdsGround, HIGH);
  delay(1000);
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  {
    tds[i]=analogRead(tdsSens);
    delay(100);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(tds[i]>tds[j])
      {
        temp=tds[i];
        tds[i]=tds[j];
        tds[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
  {
    avgValue+=tds[i];
  }
  float tdsValue=(float)avgValue*5.0/1024/6; //6 at end is for average
  tdsValue=tdsValue*3.5;
  Serial.print(tdsValue, 2)
  Serial.println(" ppm")
  // gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  // gravityTds.update();  //sample and calculate
  // tdsValue = gravityTds.getTdsValue();  // then get the value
  // Serial.print(tdsValue,0);
  // Serial.println("ppm");
  delay(500);
  digitalWrite(tdsPower, LOW);
  digitalWrite(tdsGround, LOW);
  delay(500);
  digitalWrite(phPower, HIGH);
  digitalWrite(phGround, HIGH);
  delay(1000);
  //  TODO: ADD PH SENSOR CODE HERE
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  {
    ph[i]=analogRead(phSens);
    delay(100);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(ph[i]>ph[j])
      {
        temp=ph[i];
        ph[i]=ph[j];
        ph[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
  {
    avgValue+=ph[i];
  }
  float phValue=(float)avgValue*5.0/1024/6; //6 at end is for average
  phValue=phValue*3.5;
  Serial.print(phValue, 2)
  Serial.print(" ph")
  delay(500);
  digitalWrite(phPower, LOW);
  digitalWrite(phGround, LOW);
  delay(500);
}
