char singlePhase[4] = {0x01, 0x02, 0x04, 0x08};
void setup() {
  Serial.begin(9600);
  Serial.println("input forward(fw) 0-359 or reverse(rs) 0-359");
  DDRB = 0x0F;    //0b0000 1111
  DDRD = 0xE3;    //0b1110 0011
  PORTD = PORTD | 0b00011100;

}
String input = "";
char x[2];
int newDG = 0;
int degree = 0;
int rotate = 0;
int j = 0;
void loop() {
  if (Serial.available()>0) {
    delay(100);
    input = Serial.readString();
    input.trim();

    if (input.equalsIgnoreCase("test")) {
      Serial.print("Check : Motor point to ");
      Serial.print(newDG);
      Serial.println(" degree");
    }
    else if (input.equalsIgnoreCase("reset")) {
      rotate = 0;
      if(newDG >=180)
        forward();
      else
        reverse();
    }
    else {
      x[0] = input.charAt(0);
      x[1] = input.charAt(1);
      input.remove(0, 2);
      rotate = input.toInt();


      if (x[0] == 'f' && x[1] == 'w') { //need single quote
        forward();
      }
      else if (x[0] == 'r' && x[1] == 's') {
        reverse();
      }
    }
  }
}

void forward() {
  j = 0;
  degree = 0;
  while (newDG != rotate) {
    for (int i = 0; i < 4; i++) {
      PORTB = singlePhase[i];
      delay(5);
    }
    j++;

    if (degree != floor(j / 1.42778)) {
      degree = floor(j / 1.42778);
      if (newDG > 359)
        newDG = 0;
      else
        newDG++;
      Serial.print("Motor point to ");
      Serial.print(newDG);
      Serial.println(" degree");
      //Serial.println(4*j);
    }
  }
}

void reverse() {
  j = 0;
  degree = 0;
  while (newDG != rotate) {
    for (int i = 3; i >= 0; i--) {
      PORTB = singlePhase[i];
      delay(5);
    }
    j++;
    if (degree != floor(j / 1.42778)) {
      degree = floor(j / 1.42778);
      if (newDG > 0)
        newDG--;
      else
        newDG = 359;
      Serial.print("Motor point to ");
      Serial.print(newDG);
      Serial.println(" degree");
    }
  }
}
