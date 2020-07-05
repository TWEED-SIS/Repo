#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "Laptop"
#define wifi_password "kbo123456"


char message_buff[100];
bool debug = true;    //Display log message if True

#define mqtt_server "192.168.137.146"
#define mqtt_user ""
#define mqtt_password ""
WiFiClient espClient;
PubSubClient client(espClient);


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





void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
     Serial.begin(9600);
     setup_wifi();
     client.setServer(mqtt_server,1883);
     client.setCallback(callback);
}

void loop() {
  
  // put your main code here, to run repeatedly:
if(brokerConnect())
{

publishe("/Sensor","hello from wemos");
 


}
}
