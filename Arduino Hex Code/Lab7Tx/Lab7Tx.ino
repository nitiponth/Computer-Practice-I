void setup() {
  Serial.begin(9600);
  DDRC = 0b11111000;
  PORTC = PORTC | 0b00000111;
  //pinMode(8,OUTPUT);
}

void loop() {
  if(digitalRead(A0)==0){
    while(digitalRead(A0)==0);
    Serial.write("cws");
  }
  else if(digitalRead(A1)==0){
    while(digitalRead(A1)==0);
    Serial.write("ccw");
  }
  else if(digitalRead(A2)==0){
    while(digitalRead(A2)==0);
    Serial.write("rst");
  }
}
