void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Touch Sensor Test");
 pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  int value = touchRead(4);
  Serial.println(value);
  if(value >= 50)
  {
    digitalWrite(2,HIGH);
    Serial.println("LED ON");
    delay(1000);
    }else if(value < 50)
    {
      digitalWrite(2,LOW);
      Serial.println("LED OFF");
    delay(1000);
      }
  delay(1000);
}
