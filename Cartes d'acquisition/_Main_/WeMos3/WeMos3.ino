//************************************************************************************************************************
//*******Connecter le Wemos 3 on Wifi ************************************************************************************
//*******connecter le wemos avec le broker (Mosquitto installer on le raspberry pi 4 )************************************
//*******collecter les donnée par les capteur (WaterLevel SENSOR ,WaterFlowSensor ... )***********************************
//*******Subscribe to the topics******************************************************************************************
//*******/Sensor/WaterFlow03**********************************************************************************************
//*******/Sensor/WaterLevel***********************************************************************************************
//*******comander le ventillateur et mini pompe***********************************************************************************************
//************************************************************************************************************************

//***********************************************************************
//********************************Librairies
//***********************************************************************

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//***********************************************************************
//*********************************Wifi parametre
//***********************************************************************

#define wifi_ssid "yourSSID"
#define wifi_password "yourPASSWORD"

//***********************************************************************
//**********************************mqtt parametre
//***********************************************************************

#define mqtt_server "10.0.2.15"
#define mqtt_user "guest"
#define mqtt_password "guest"
WiFiClient espClient;
PubSubClient client(espClient);



//***********************************************************************
//************************************generale Variable
//***********************************************************************

#define WaterSensorLevelPin A0
float WaterLevelValue = 0; 
bool debug = false;    //Display log message if True
char message_buff[100];
long timeBetweenMessages = 1000 * 20 * 1;

char message_buff_wfd[8];
char message_buff_wld[8];
char message_buff_wmill[8];

//waterflowsensor var.....................
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;










void setup() {
  // put your setup code here, to run once:
//***********************************************************************
//**************************************functions
//***********************************************************************

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  if(debug)
  {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  }
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(debug){
    Serial.print(".");
    }
  }
  if(debug){
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}}

void reconnection(){

  while (!client.connected()){
    Serial.print("Connexion au serveur MQTT ...");
      if (client.connect("WeMos1_ESP8266Client",mqtt_user,mqtt_password)){
        Serial.println("connected");
//********************Subscribe to topic start """""""""""""""
//client.subscribe("Topic1");
//********************Subscribe to topic end """""""""""""""
//********************publish to topic start """""""""""""""
//client.publish(temperature_topic, String(t).c_str(), true);  
//********************publish to topic end """""""""""""""    
      } else {
        Serial.print("failed , rc = ");
        Serial.print(client.state());
        Serial.print("on attend 5 secondes avant de recommencer ...");
       delay(5000);
      }
    } 
}


bool brokerConnect(){
if(client.connected()){
return true;
}else{
  while (!client.connected()){
    if (client.connect("WeMos1_ESP8266Client",mqtt_user,mqtt_password)){
      if(debug){
        Serial.println("connected");
               }
        return true;       
    }else{
      if(debug){
         Serial.print("failed , rc = ");
        Serial.print(client.state());
        Serial.print("on attend 5 secondes avant de recommencer ...");
        delay(5000);
      }
      return false;
    }
     
  }
} 
}

void Subscribe(char* Topic){
  client.subscribe(Topic);
}

void publishe(char* Topic,char* msg){
  client.publish(Topic, msg , true);  
}


void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  if ( debug ) {
    Serial.println("Message recu =>  topic: " + String(topic));
    Serial.print(" | longueur: " + String(length,DEC));
  }
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  } 
  //******manipulation du call back msg   exp: if ( msgString == "ON" )... 
}



float WaterLevelData()
{
  float data=0;
 data=analogRead(WaterSensorLevelPin);
//******test selon les données
 
  return data;
}


void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}



void WaterFlowSensor_Setup()
{
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
   // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

float WaterFlowSensor01_FlowRate()
{
 if((millis()-oldTime)>1000){
  detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    unsigned int frac;
  
    if(debug){
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");      // Print tab space

    // Print the cumulative total of litres flowed since starting
  Serial.print("Output Liquid Quantity: ");        
  Serial.print(totalMilliLitres);
  Serial.println("mL"); 
  Serial.print("\t");       // Print tab space
  Serial.print(totalMilliLitres/1000);
  Serial.print("L");}
    

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    return flowMilliLitres;
 }  
}





void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server,1883);
    client.setCallback(callback);
    WaterFlowSensor_Setup();
    
}

void loop() {
  // put your main code here, to run repeatedly:

float WFD=WaterFlowSensor01_FlowRate();//waterflow data
float WLD=WaterLevelData();            //waterlevel data
float WMill=totalMilliLitres;

if(brokerConnect())
{
 String stringWFD=String(WFD);
 stringWFD.toCharArray(message_buff_wfd, stringWFD.length()+1);
 publishe("/Sensor/WaterFlow03",message_buff_wfd);
 String stringWLD=String(WLD);
 stringWLD.toCharArray(message_buff_wfd, stringWLD.length()+1);
 publishe("/Sensor/WaterLevel",message_buff_wld);
 String stringWMill=String(WMill);
 stringWMill.toCharArray(message_buff_wmill, stringWMill.length()+1);
 publishe("/Sensor/Watermil03",message_buff_wmill);

 
}





}
}

void loop() {
  // put your main code here, to run repeatedly:

}
