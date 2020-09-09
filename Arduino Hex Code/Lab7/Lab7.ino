void setup() {
  Serial.begin(9600);
  DDRC = 0b11111000;
  PORTB = PORTB | 0b00000111;
}

void loop() {
  if(digitalRead(14)==0){
    while(digitalRead(14)==0);
    Serial.write("cw");
  }
  else if(digitalRead(15)==0){
    while(digitalRead(15)==0);
    Serial.write("ccw");
  }
  else if(digitalRead(16)==0){
    while(digitalRead(16)==0);
    Serial.write("rst");
  }

}
