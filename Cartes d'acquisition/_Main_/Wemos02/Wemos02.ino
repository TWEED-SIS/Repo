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
//*******relays*************
#define Relay01 D5
#define Relay02 D5
#define SensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

//*******Water flow sensor*************
#define waterflowsensorpin  D4

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

void OnRelay01(){
   digitalWrite(Relay01,LOW);
}
void OffRelay01(){
   digitalWrite(Relay01,HIGH);
}

void OnRelay02(){
   digitalWrite(Relay02,LOW);
}
void OffRelay02(){
   digitalWrite(Relay02,HIGH);
}



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

if(msgString=="onVent"){
    OnRelay01();
    Serial.println("onVent ");
  }
else if(msgString =="offVent"){
  OffRelay01();
   Serial.println("offVent");
}else if(msgString=="onPomp"){
    OnRelay02();
    Serial.println("onPomp ");
  }
else if(msgString =="offPomp"){
  OffRelay02();
   Serial.println("offPomp");
}
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

float phvalue(){
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  return phValue;
}


//***********************************************************************
//*********************************fuctions END
//***********************************************************************








void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(Relay01,OUTPUT);
  pinMode(Relay02,OUTPUT);  
  setupwaterflow();
   dht.begin();
}

void loop() {

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



      //*******************************************PH******************************************
   floatvariable= phvalue();
   stringvariable=String(floatvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Ph", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);

   //*******************************************Temperature******************************************
   floatvariable= temperature();
   stringvariable=String(floatvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Temperature02", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);
   //*******************************************Humidité******************************************
   floatvariable= humidite();
   stringvariable=String(floatvariable);
   stringvariable.toCharArray(message_subs, stringvariable.length()+1);
   client.publish("/Sensors/Humidity02", message_subs);
   Serial.print("Publish message: ");
   Serial.println(message_subs);


delay(25000);
client.loop();  
}
