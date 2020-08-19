char singlePhase[4] = {0x01,0x02,0x04,0x08};
void setup() {
  Serial.begin(9600);
  Serial.println("Start Arduino nano :D");
  DDRB = 0x0F;    //0b0000 1111
  DDRD = 0xE3;    //0b1110 0011
  PORTD = PORTD | 0b00011100;
  
}
String input;
char x[2];
int degree = 0;
int newDG = 0;
int rotate = 0;
int rotateTo = 0;
int j = 0;
void loop() {
  if(Serial.available()< 2 && Serial.available() > 0){
    delay(100);
    input = Serial.readString();
    x[0] = input.charAt(0);
    x[1] = input.charAt(1);
    input.remove(0,2);
    rotate = input.toInt();
    rotateTo = ceil(rotate *1.42778);
    
    if(x[0] == 'f' && x[1] == 'w'){   //need single quote
      //for(int j=0;j<1;j++){     // 1degree = 1.42778 j   90 = 128.5j  180 = 257j 360 = 514j
      j=0;
      degree = 0;
      while(newDG != rotate){
        for(int i=0;i<4;i++){
            PORTB = singlePhase[i];
            delay(5);
        }
        j++;
        
        if(degree != ceil(j/1.42778)){
          degree = ceil(j/1.42778);
          if(newDG > 359)
            newDG = 0;
          else
            newDG++;
          Serial.print(newDG);
          Serial.println(" degree");
        }
      }
    }
    else if(x[0] == 'r' && x[1] == 's'){
      j = 0;
      degree = 0;
      while(newDG != rotate){
        for(int i=3;i>=0;i--){
          PORTB = singlePhase[i];
          delay(5);
        }
        j++;
        if(degree != ceil(j/1.42778)){
          degree = 360 - ceil(j/1.42778);
          if(newDG > 0)
            newDG--;
          else
            newDG = 359;
          Serial.print(newDG);
          Serial.println(" degree");
        }  
      }


      
    }
  }
}
  
