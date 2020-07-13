void setup() {
  // put your setup code here, to run once:
  DDRB = 0b11100111;
  DDRC = 0b00001111;
}
char x = 0;
void loop() {
  // put your main code here, to run repeatedly:
  x = PINB & 0b00011000;          
  if(x==0x08)               //SW2
    PORTC = PORTC | 0x0C;   
  else if(x==0x10)          //SW1
    PORTC = PORTC | 0x03;   
   else
    PORTC = PORTC & 0xF0;
}
