const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
void setup() {
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);  
}
void loop() {
int val=analogRead(A0);
 Serial.print(val);
 Serial.print("\n");
if(val<512){
  Serial.print("rain \n");
}else{
  Serial.print("not rain \n");
}
 delay(2000);
}
