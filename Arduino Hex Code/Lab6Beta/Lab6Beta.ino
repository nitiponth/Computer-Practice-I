void setup() {
  Serial.begin(9600);
  
    reset();
  Serial.print(0xCC,BIN);

}
int curTemp = 0;
double displayTemp = 0;
unsigned char tempLSB , tempMSB;
void loop() {
  reset();
  masterWrite(0xCC);
  masterWrite(0x44);
  reset();
  masterWrite(0xCC);
  masterWrite(0xBE);
  tempLSB = masterRead();
  tempMSB = masterRead();
  //Serial.print(tempMSB,HEX);
  //Serial.print("  ");
  //Serial.println(tempLSB,HEX);

  curTemp = tempLSB;
  curTemp &= 0x00FF;
  curTemp |= (tempMSB << 8);
  displayTemp = curTemp / 16.0;
  Serial.println(displayTemp);
  delay(1000);
  reset();
  

}

void reset(){
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  delayMicroseconds(500);
  digitalWrite(8,HIGH);
  delayMicroseconds(60);
  digitalWrite(8,LOW);
  delayMicroseconds(240);
  digitalWrite(8,HIGH);
  delay(1);
}

unsigned char masterRead(){
  unsigned char data = 0x00;
  for(int i=8;i>0;i--){
    pinMode(8,OUTPUT);
    digitalWrite(8,LOW);
    delayMicroseconds(15);
    pinMode(8,INPUT);
    //delay(1);
    data = data >> 1;
    if(digitalRead(8)){  
      data = data | 0x80;
    }
    //Serial.print(data,BIN);
    
    delayMicroseconds(45);
    
    digitalWrite(8,HIGH);
    delayMicroseconds(10);
  }
  
  //Serial.println();
  //Serial.println(data,BIN);
  return data;
}

unsigned char masterWrite(unsigned char sendData){
  pinMode(8,OUTPUT);
  for(int i=8;i>0;i--){
    digitalWrite(8,LOW);
    delayMicroseconds(15);
    if((sendData & 0x01) == 0x01){
      digitalWrite(8,HIGH);
      delayMicroseconds(40);
    }
    else{
      digitalWrite(8,LOW);
      delayMicroseconds(40);
    }
    digitalWrite(8,HIGH);
    delayMicroseconds(5);
    sendData = sendData >> 1;
  }
  //Serial.println("Finish Sending");  
} 
  
