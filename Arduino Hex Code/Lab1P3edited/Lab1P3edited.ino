void lcd_command(char cmd);
void lcd_text(char text);
void initial_LCD();
void lcd_puts(char ch[]);

float volt=0; char result[4];
void setup() {
  // put your setup code here, to run once:
  DDRC = 0x30;
  DDRD = 0xFF;
  DDRB = 0xFC;
  PORTB = PORTB | 0b00000011;
  initial_LCD();
  
  dtostrf(volt, 2, 1, result);
  
  lcd_command(0x80);
  lcd_puts("     Voltage");
  lcd_command(0xc0);
  lcd_puts("       ");
  lcd_puts(result);
  lcd_puts("  V");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int change = -1;
  if((PINB&0x01)==0){
    while((PINB&0x01)==0);
    change = 1;
    if(volt > 99.8)
      volt = 0;
    else
      volt = volt + 0.1;
  }
  
  if((PINB&0x02)==0){
    while((PINB&0x02)==0);
    change = 1;
    if(volt >= 0.1)
      volt = volt - 0.1;
     else
      volt = 0;
  }
  
  if(change == 1){
    lcd_command(0x01);
    dtostrf(volt, 2, 1, result);
    
    lcd_command(0x80);
    lcd_puts("     Voltage");
    lcd_command(0xc0);
    lcd_puts("       ");
    lcd_puts(result);
    lcd_puts("  V");
    change = -1;
  }
}

void lcd_puts(char ch[]){
  int k=0;
  while(ch[k]!='\0'){
    lcd_text(ch[k]);
    k++;
  }
}

void lcd_command(char cmd){
  PORTC =PORTC &0b11101111;
  PORTD = cmd;
  PORTC =PORTC | 0b00100000;
  delay(1);
  PORTC = PORTC & 0b11011111;
  delay(1);
}
void lcd_text(char text){
  PORTC =PORTC | 0b00010000;
  PORTD = text;
  PORTC = PORTC | 0b00100000;
  delay(1);
  PORTC = PORTC & 0b11011111;
  delay(1);
}
void initial_LCD(){
  delay(100);
  lcd_command(0x38); delay(15);
  lcd_command(0x38); delay(1);
  lcd_command(0x38); delay(1);
  lcd_command(0x38);
  lcd_command(0x0c);
  delay(1);
  lcd_command(0x01);
  delay(1);
  lcd_command(0x06);
  delay(1);
}


  
