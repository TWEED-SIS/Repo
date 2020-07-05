#define RPIN D4

void setup() {
  Serial.begin(9600);
  Serial.println("Ventilateur automatic test!");
  pinMode(RPIN,OUTPUT); 

}

void loop() {
   digitalWrite(RPIN,LOW);

}
