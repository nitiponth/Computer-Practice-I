void initial_LCD();
void lcd_command(char cmd);
void lcd_text(char text);
void lcd_puts(char *ch);
char y[10];
double B;
int z;

void setup() {
  DDRC = 0x30;
  DDRD = 0x0F;
  //DDRB = 0xFC;
  //PORTB = PORTB | 0b00000011;
  initial_LCD();

  B = 3.42;
  lcd_command(0x80);
  lcd_puts("B =: ");
  itoa((int)B,y,10);
  lcd_puts(y); lcd_text('.');
  z = (B-(int)B)*100;
  itoa(z,y,10);
  lcd_puts(y);
  
}

void loop() {
//----------
}

void lcd_command(char cmd){
  unsigned char buff;
  buff = (cmd & 0xF0) >> 4;
  PORTC = PORTC & 0b11101111;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 1 then E = 1  => Start LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(1);
  PORTC = PORTC & 0b11011111;   //PC5 = 0 then E = 0  => Stop LCD Clock
  delay(1);

  buff = (cmd & 0x0F);
  PORTC = PORTC & 0b11101111;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 1 then E = 1  => Start LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(1);
  PORTC = PORTC & 0b11011111;   //PC5 = 0 then E = 0  => Stop LCD Clock
  delay(1);
}

void lcd_text(char text){
  unsigned char buff;
  buff = (text & 0xF0) >> 4;
  PORTC = PORTC | 0b00010000;   //PC4 = 1 then RS = 1 => Data Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 1 then E = 1  => Start LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(1);
  PORTC = PORTC & 0b11011111;   //PC5 = 0 then E = 0  => Stop LCD Clock
  delay(1);

  buff = (text & 0x0F);
  PORTC = PORTC & 0b11101111;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 0 then E = 0  => Stop LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(1);
  PORTC = PORTC & 0b11011111;
  delay(1);
}

void initial_LCD(){
  delay(100);
  lcd_command(0x33);  delay(15);
  lcd_command(0x33);  delay(1);
  lcd_command(0x33);  delay(1);
  lcd_command(0x22);  delay(1);
  lcd_command(0x28);  delay(1);

  lcd_command(0x0F);
  delay(1);
  lcd_command(0x01);
  delay(1);
  lcd_command(0x06);
  delay(1);
  lcd_command(0x80);
  delay(1);
}

void lcd_puts(char ch[]){
  int k=0;
  while (ch[k] != '\0'){
    lcd_text(ch[k]);
    k++;
  }
}
