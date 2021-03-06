char singlePhase[4] = {0x01,0x02,0x04,0x08};
char fullPhase[4] = {0x03,0x06,0x0C,0x18};
//char fullDoublePhase[8] = {0x03,0x03,0x06,0x06,0x0C,0x0C,0x18,0x18};
char halfPhase[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x18};

void setup() {
  DDRB = 0x0F;    //0b0000 1111
  DDRD = 0xE3;    //0b1110 0011
  PORTD = PORTD | 0b00011100;

  //PORTB = singlePhase[0];
  //PORTB = fullPhase[0];
  //PORTB = halfPhase[0];
}

void loop() {
  if((PIND&0x04)==0){
    while((PIND&0x04)==0);
    PORTB = singlePhase[0];
    delay(10);
    for(int j=0;j<257;j++){     // 1degree = 1.42778 j   90 = 128.5j  180 = 257j 360 = 514j
      for(int i=0;i<4;i++){
        PORTB = singlePhase[i];
        delay(5);
      }
    }
    PORTB = 0x00;
  }
  if((PIND&0x08)==0){
    while((PIND&0x08)==0);
    PORTB = fullPhase[0];
    delay(10);
    for(int j=0;j<514;j++){
      for(int i=0;i<4;i++){
        PORTB = fullPhase[i];
        delay(2);
      }
    }
    PORTB = 0x00;
  }
  if((PIND&0x10)==0){
    while((PIND&0x10)==0);
    PORTB = halfPhase[0];
    delay(10);
    for(int j=0;j<514;j++){
      for(int i=0;i<8;i++){
        PORTB = halfPhase[i];
        delay(4);
      }
    }
    PORTB = 0x00;
  }
  
}
