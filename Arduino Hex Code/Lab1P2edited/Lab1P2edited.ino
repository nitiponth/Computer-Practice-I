void setup() {
  // put your setup code here, to run once:
  DDRB = 0b11100111;
  DDRC = 0b00001111;
}
char x = 0;
void loop() {
  // put your main code here, to run repeatedly:
  x = PINB & 0b00011000;          //
  
  if(x==0x10){
    do{
      PORTC = PORTC & 0xF0;
      delay(500);
      PORTC = PORTC | 0x01;
      delay(500);
      PORTC = PORTC | 0x03;
      delay(500);
      PORTC = PORTC | 0x07;
      delay(500);
      PORTC = PORTC | 0x0F;
      delay(500);

      x = PINB & 0b00011000;
    }
    while(!(x==0x08));
    PORTC = PORTC & 0xF0;
  }
}
