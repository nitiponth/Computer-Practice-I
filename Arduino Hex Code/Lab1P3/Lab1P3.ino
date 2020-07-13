void lcd_command(char cmd);
void lcd_text(char text);
void initial_LCD();
void lcd_puts(char ch[]);
void setup() {
  // put your setup code here, to run once:
  DDRC = 0x30;
  DDRD = 0xFF;
  DDRB = 0xFC;
  PORTB = PORTB | 0b00000011;
  initial_LCD();
  lcd_text('G');
  lcd_text('1');
}

int i=0;

void loop() {
  // put your main code here, to run repeatedly:
  if((PINB&0x01)==0){
    while((PINB&0x01)==0);
    i++;
  }
  if(i==3){
    i=0;
    lcd_command(0x01);
  }
  else if(i==1){
    lcd_command(0x80);
    lcd_puts("first line");
  }
  else if(i==2){
    lcd_command(0xc0);
    lcd_puts("second line");
  }
  if((PINB&0x02)==0){
    while((PINB&0x02)==0);
    lcd_command(0x01);
    i=0;
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


  
