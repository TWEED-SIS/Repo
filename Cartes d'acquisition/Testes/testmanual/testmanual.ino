#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ven D4 
char message_buff[100];
char message_buff_temp[8];
int i=0;
const char* ssid = "Laptop";
const char* password = "kbo123456";
const char* mqtt_server =  "192.168.137.146";

WiFiClient espClient;
PubSubClient client(espClient);

void onvent(){
   digitalWrite(ven,LOW);
}
void offvent(){
   digitalWrite(ven,HIGH);
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

  if(msgString=="on"){
    onvent();
    Serial.println("on");
  }
else if(msgString =="off"){
  offvent();
   Serial.println("off");
}

  // Switch on the LED if an 1 was received as first character
  /*if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level

    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
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





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Ventilateur automatic test!"));
  pinMode(ven,OUTPUT); 
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:


   if (!client.connected()) {
    reconnect();
    }
client.loop();

}
