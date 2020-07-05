
//***********************************************************************
//********************************Librairies
//***********************************************************************

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"


//***********************************************************************
//******************************** variables
//***********************************************************************

//*******dht22*************
#define DHTPIN D3 
#define DHTTYPE DHT22
//*******soilmoisture*************
#define SoilMoistureSensorPin A0 
//*******ultra_son*************
#define echoPin D7 // Echo Pin
#define trigPin D6 // Trigger Pin
//*******Water flow sensor*************
#define waterflowsensorpin  D4


//*******genarals*************
float sensorValue = 0; 
int moisture;
int percentage;
//*******ultra_son waterlevel*************
long duration, distance; // Duration used to calculate distance
long eau=0;
long volum=0;

int map_low = 1024;
int map_high = 300;
char message_buff[100];
char message_subs[50];
float floatvariable;
int intvariable;
long longvariable;
String stringvariable;
int i=0;




long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
float flowRatesec;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

//***********************************************************************
//*********************************Wifi parametre
//***********************************************************************

const char* ssid = "Laptop";
const char* password = "kbo123456";

//***********************************************************************
//**********************************mqtt parametre
//***********************************************************************

const char* mqtt_server =  "192.168.137.146";
//#define mqtt_user "guest"
//#define mqtt_password "guest"
WiFiClient espClient;
PubSubClient client(espClient);







//***********************************************************************
//*********************************fuctions START
//***********************************************************************
DHT dht(DHTPIN, DHTTYPE);


void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setupwaterflow(){
   pinMode(waterflowsensorpin, INPUT_PULLUP);
   pulseCount = 0;
   flowRate = 0.0;
   flowMilliLitres = 0;
   totalMilliLitres = 0;
   previousMillis = 0;
   attachInterrupt(digitalPinToInterrupt(waterflowsensorpin), pulseCounter, FALLING);
}

float flowratemlsec(){

}




void setupultrason(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
/*
void relay01on(){
   digitalWrite(ven,LOW);
}
void relay01off(){
   digitalWrite(ven,HIGH);
}
*/
float RawMoisture(){
  moisture = analogRead(SoilMoistureSensorPin);
  Serial.print("Raw: ");
  Serial.print(moisture);
  return moisture;
}
int PercentageMoisture(){
  percentage = map(RawMoisture(), map_low, map_high, 0, 100);
  Serial.print(" | Percentage: ");
  Serial.print(percentage);
  Serial.println("%");
  return percentage;
}
float temperature(){
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 1;
  }
  return t;
}

float humidite(){
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 1;
  }
  return h;
}


long Volumedeau(){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
//Calculate the distance (in cm) based on the speed of sound.
distance = duration/58.2;
eau=18-distance;//18=hauteur du resrvoir
volum=3.14*6*6*eau;//6=diametre du reservoir

return volum;
}




void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  Serial.println();
/*
if(msgString=="on"){
    relay01on();
    Serial.println("on ");
  }
else if(msgString =="off"){
  relay01off();
   Serial.println("off");
}*/
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...

      
    //  client.publish("/Sensors/WaterLevel", message_buff_temp);
      // ... and resubscribe
      client.subscribe("/Action/vent");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




//***********************************************************************
//*********************************fuctions END
//***********************************************************************


void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  dht.begin();
  setupultrason();
  setupwaterflow();
}




void loop() {
  // Wait a few seconds between measurements.
   delay(1000);
   if (!client.connected()) {
    reconnect();
    } 
   //*******************************************Temperature******************************************
   floatvariable= temperature();
   stringvariable=String(floatvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Temperature01", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);
   //*******************************************Humidité******************************************
   floatvariable= humidite();
   stringvariable=String(floatvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Humidity01", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);
   //*******************************************soilMoisture******************************************
   intvariable= PercentageMoisture();
   stringvariable=String(intvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Soilhumidity", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);
   //*******************************************Volume d'eau******************************************
   longvariable= Volumedeau();
   stringvariable=String(longvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/WaterVolume", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);



 //*******************************************debit d'eau******************************************
   currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    Serial.print("Flow rate: ");
    Serial.print(flowRate);  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    flowRatesec=(flowRate/60)*1000;
    Serial.print("Flow rate: ");
    Serial.print(flowMilliLitres);  // Print the integer part of the variable
    Serial.print("milL/sec");
    Serial.print("\t         ");       // Print tab space
    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
  }
   intvariable= flowMilliLitres;
   stringvariable=String(intvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/WaterFlow01", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);

 delay(10000);
   

client.loop();  
}
