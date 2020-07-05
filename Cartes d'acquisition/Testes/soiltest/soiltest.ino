#define SensorPin A0 
float sensorValue = 0; 
int moisture;
int percentage;

int map_low = 1024;
int map_high = 300;



void setup() { 
 Serial.begin(9600); 
} 
/*
void loop() {

  
  sensorValue = 0; 
 for (int i = 0; i <= 100; i++) 
 { 
   sensorValue = sensorValue + analogRead(SensorPin); 
   delay(1); 
 } 
 int percentMoist=map(sensorValue,0,1024,100,0);
 sensorValue = sensorValue/100.0; 
 Serial.println(percentMoist); 
 Serial.println("%");
 delay(300); 
} */
void loop() {
  moisture = analogRead(SensorPin);
  Serial.print("Raw: ");
  Serial.print(moisture);

  percentage = map(moisture, map_low, map_high, 0, 100);

  Serial.print(" | Percentage: ");
  Serial.print(percentage);

  Serial.println("%");

  delay(1000);
}
