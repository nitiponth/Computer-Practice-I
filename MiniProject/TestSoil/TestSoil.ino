void setup() {
  pinMode(4,INPUT);
  Serial.begin(115200);

}

void loop() {
  int value = analogRead(4);
  Serial.print("ADC Value : ");
  Serial.println(value);
  value = map(value,4095,0,0,100);
  Serial.print("Percent Value : ");
  Serial.println(value);
  delay(500);
  

}
