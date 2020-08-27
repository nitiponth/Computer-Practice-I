//#include math.h
void initial_LCD();
void lcd_command(char cmd);
void lcd_text(char text);
void lcd_puts(char *ch);
char y[10];
int adc_value;
double rTh = -1  , temp = -1;


void setup() {
  Serial.begin(9600);
  DDRC = 0x30;  //0011 0000 
  DDRD = 0x0F;
  DDRB = 0xFC;
  PORTB = PORTB | 0b00000011;
  initial_LCD();
  lcd_command(0x80);
  lcd_puts("Temperature ");
}
int myADC(char ch){
    int ADCval;
    ADMUX = ch;
    ADMUX |= 0x40;
    ADMUX &= 0xDF;
    ADCSRA |= 0x07;
    ADCSRA |= 0xC0;

    while(ADCSRA & 0x40);
    ADCval = ADCW;
    return ADCval;
}

void loop() {

  adc_value = myADC(0);
   double Vo = ((double)adc_value)*(5.0/1024.0);
   rTh = ((5.0 * 10000.0)/Vo)  - 10000.0;
  
   temp = 1.0/((1.0/3435.0)*log(rTh/10000.0)+(1.0/298.15));
   temp = temp - 273.15;

   Serial.println(temp);
   
  
  if((PINB&0x01)==0){
    while((PINB&0x01)==0); 

    dtostrf(temp,4,2,y);
    lcd_command(0xC0);
    lcd_puts("                 ");
    lcd_command(0xC0);
    lcd_puts(y);
    lcd_puts(" Celsius");
  }

  else if((PINB&0x02)==0){
    while((PINB&0x02)==0);
    
    temp = temp * (9.0/5.0) + 32.0;

    dtostrf(temp,4,2,y);
    lcd_command(0xC0);
    lcd_puts("                 ");
    lcd_command(0xC0);
    lcd_puts(y);
    lcd_puts(" Fahrenheit");
  }
}

void lcd_command(char cmd){
  unsigned char buff;
  buff = (cmd & 0xF0) >> 4;
  PORTC = PORTC & 0b11101111;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 1 then E = 1  => Start LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(5);
  PORTC = PORTC & 0b11011111;   //PC5 = 0 then E = 0  => Stop LCD Clock
  delay(1);

  buff = (cmd & 0x0F);
  PORTC = PORTC & 0b11101111;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 1 then E = 1  => Start LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(5);
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
  PORTC = PORTC | 0b00010000;   //PC4 = 0 then RS = 0 => Command Mode
  PORTC = PORTC | 0b00100000;   //PC5 = 0 then E = 0  => Stop LCD Clock
  PORTD = (PORTD & 0xF0)|buff;
  delay(1);
  PORTC = PORTC & 0b11011111;
  delay(1);
}

void initial_LCD(){
  delay(100);
  lcd_command(0x33);  delay(15);
  lcd_command(0x32);  delay(1);
  lcd_command(0x28);  delay(1);
  
  lcd_command(0x0C);
  delay(1);
  lcd_command(0x01);
  delay(1);
  lcd_command(0x06);
  delay(1);
}

void lcd_puts(char ch[]){
  int k=0;
  while (ch[k] != '\0'){
    lcd_text(ch[k]);
    k++;
  }
}
