#include <ESP8266WiFi.h> //Library can be download below
#include <WiFiClient.h> //Library can be downloaded below
#include <ESP8266WebServer.h> //Library can be downloaded below
#include <PubSubClient.h> 

int sensorPin1 = D5;

//int flowPin2 = 4;
//int flowPin3 = 3;
//int flowPin4 = 2;

unsigned long flow1count = D0;
unsigned long flow2count = D0;

#define countof(a) (sizeof(a) / sizeof(a[0]))


void setup()
{

Serial.begin(9600);
pinMode(sensorPin1, INPUT_PULLUP);
attachInterrupt(sensorPin1, flow1, CHANGE);

}

void loop()
{
flow1();
flow2();
Serial.print("14CORE | Flow Sensor ") ;   
Serial.print("Flow 1 in Liters: ");
Serial.print(flow1count/450.0);
Serial.print("\tFlow 2 in Liters: ");
Serial.print(flow2count/450.0);
Serial.print("\tFlow 1 in pulses: ");
Serial.print(flow1count);
Serial.print("\tFlow 2 in pulse: ");
Serial.println(flow2count);
delay(500);
}

void flow1()
{
flow1count +=1;
}

void flow2()
{
flow2count +=1;
}
