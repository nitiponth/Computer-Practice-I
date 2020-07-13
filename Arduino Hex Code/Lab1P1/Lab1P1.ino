void setup() {
  // put your setup code here, to run once:
  DDRB = 0b11110111;
}
char x;
void loop() {
  // put your main code here, to run repeatedly:
  x = PINB & 0b00001000;          //if PB3 unclick then x = 1 else x = 0 (pull up) 
  if(x==0)
    PORTB = PORTB | 0b00010000;   //Set PB4 = 1
  else
    PORTB = PORTB & 0b11101111;   //Set PB4 = 0
}
