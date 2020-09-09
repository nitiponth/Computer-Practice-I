char singlePhase[4] = {0x01,0x02,0x04,0x08};
void setup() {
  Serial.begin(9600);
  DDRB = 0b00001111;
}

String recData;
int degree = 0;
void loop() {
  if(Serial.available() >= 3){0
    recData = Serial.readString();
    //recData.trim();
  }
  if(recData.equals("cws")){
    forward();
    recData = "";
  }
  else if(recData.equals("ccw")){
    reverse();
    recData = "";
  }
  else if(recData.equals("rst")){
      while(degree != 0){
        if(degree > 180)
          forward();
        else
          reverse();
      }
  }
}

void forward(){
  for(int j=0;j<64;j++){     // 1degree = 1.42778 j   90 = 128.5j  180 = 257j 360 = 514j
    for(int i=0;i<4;i++){
      PORTB = singlePhase[i];
      delay(10);
    }
  }
  PORTB = 0x00;
  degree += 45;
  degree %= 360;
}

void reverse(){
  for(int j=0;j<64;j++){     // 1degree = 1.42778 j   90 = 128.5j  180 = 257j 360 = 514j
    for(int i=3;i>=0;i--){
      PORTB = singlePhase[i];
      delay(10);
    }
  }
  degree -= 45;
  if(degree < 0)
    degree = 360 + degree;
  PORTB = 0x00;
}
