
#define DIP_0 12
#define DIP_1 11
#define DIP_2 10
#define DIP_3 9
#define DIP_4 8
#define DIP_5 7
#define DIP_6 6
#define PIN_INDICATOR_LED 2

void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  // put your setup code here, to run once:
pinMode(DIP_0, INPUT_PULLUP);
pinMode(DIP_1, INPUT_PULLUP);
pinMode(DIP_2, INPUT_PULLUP);
pinMode(DIP_3, INPUT_PULLUP);
pinMode(DIP_4, INPUT_PULLUP);
pinMode(DIP_5, INPUT_PULLUP);
pinMode(DIP_6, INPUT_PULLUP);
pinMode(PIN_INDICATOR_LED, OUTPUT);
digitalWrite(PIN_INDICATOR_LED,LOW);


  
}

uint32_t count = 0;
uint8_t indicator = 0;

void loop() {
  // put your main code here, to run repeatedly:


  uint8_t dip_0 = !digitalRead(DIP_0);
  uint8_t dip_1 = !digitalRead(DIP_1);
  uint8_t dip_2 = !digitalRead(DIP_2);
  uint8_t dip_3 = !digitalRead(DIP_3);
  uint8_t dip_4 = !digitalRead(DIP_4);
  uint8_t dip_5 = !digitalRead(DIP_5);
  uint8_t dip_6 = !digitalRead(DIP_6);

  uint8_t value = dip_0 + (dip_1 << 1) + (dip_2 << 2) + (dip_3 << 3) + (dip_4 << 4);
  uint8_t dmx_address = (value * 16) + 1;
  Serial.print(count); Serial.print(" ");
  Serial.print(dip_6);
  Serial.print(dip_5);
  Serial.print(dip_4);
  Serial.print(dip_3);
  Serial.print(dip_2);
  Serial.print(dip_1);
  Serial.print(dip_0);
  Serial.print(" value "); Serial.print(value);
  Serial.print(" dmx_address "); Serial.print(dmx_address);
  Serial.println("");

  uint8_t value1  = dmx_address;
  uint8_t digit_3 = value1 / 100;
  value1          = value1 - digit_3*100;
  uint8_t digit_2 = value1 / 10;
  value1          = value1 - digit_2*10;
  uint8_t digit_1 = value1;

  Serial.print(" digits "); Serial.print(digit_3);
  Serial.print(" "); Serial.print(digit_2);
  Serial.print(" "); Serial.print(digit_1);
  Serial.println("");

  digitalWrite(PIN_INDICATOR_LED,LOW);
  delay(3000);
  for( uint8_t i = 0; i < digit_3; i++ )
  {
    digitalWrite(PIN_INDICATOR_LED,HIGH);
    delay(250);
    digitalWrite(PIN_INDICATOR_LED,LOW);
    delay(250);
  }
  delay(1000);
  for( uint8_t i = 0; i < digit_2; i++ )
  {
    digitalWrite(PIN_INDICATOR_LED,HIGH);
    delay(250);
    digitalWrite(PIN_INDICATOR_LED,LOW);
    delay(250);
  }
  delay(1000);
  for( uint8_t i = 0; i < digit_1; i++ )
  {
    digitalWrite(PIN_INDICATOR_LED,HIGH);
    delay(250);
    digitalWrite(PIN_INDICATOR_LED,LOW);
    delay(250);
  }
    
  
  count++;
  
  delay(100);

}
