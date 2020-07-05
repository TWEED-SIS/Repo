
#include "DHT.h"

#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

bool debug =true;


float Temperature(){
  delay(2000);
  float temp=dht.readTemperature();
  float f=dht.readTemperature(true);
 if (debug&&(isnan(temp) || isnan(f))) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 1;}
    if(debug){
  Serial.print("%  Temperature: ");
  Serial.print(temp);
  Serial.print("°C ");
  Serial.print(f);
  Serial.print("°F  Heat index: "); 
    }
  return temp;
}
float humidity(){
  delay(2000);
  float hum=dht.readHumidity();
 if (debug&&(isnan(hum))) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return 1;}
    if(debug){
  Serial.print("  Humidity: ");
  Serial.print(hum);
    }
  return hum;
}




void setup() {
  // put your setup code here, to run once:
dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
   int temperature=Temperature();
   int humidite = humidity();
}
