#include <Wire.h>
#include <elapsedMillis.h>
#include <Adafruit_NeoPixel.h>

elapsedMillis elapsed;
#define NEOPIXEL_PIN 6
#define STRING_LENGTH 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRING_LENGTH, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte control, red, green, blue = 0;
uint32_t color, prev_color = 0;
boolean newLedData = false;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  elapsed = 0;
  strip.begin();
  strip.clear();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  if( newLedData )
  {
    if( prev_color != color )
    {
      for( int i = 0; i<STRING_LENGTH; i++){
        strip.setPixelColor(i, color);
      }
      strip.show();
      newLedData = false;
      prev_color = color;
    }
  }
  delay(50);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{

  byte msgID = Wire.read();
  Serial.print(F("New Data "));
  Serial.println(msgID);
  //Serial.print(" ");
  //Serial.println(elapsed);
  
  switch( msgID )
  {
    case 2:
      const uint8_t control1  = Wire.read();
      const uint8_t red1      = Wire.read();
      const uint8_t green1    = Wire.read();
      const uint8_t blue1     = Wire.read();
      const uint8_t control2  = Wire.read();
      const uint8_t red2      = Wire.read();
      const uint8_t green2    = Wire.read();
      const uint8_t blue2     = Wire.read();
      const uint8_t servo     = Wire.read();  

      Serial.print("control1 "); Serial.println(control1);
      Serial.print("red1 "); Serial.println(red1,HEX);
      Serial.print("control2 "); Serial.println(control2);
      Serial.print("red2 "); Serial.println(red2,HEX);
      
      break;
    default:
      while(Wire.available())
      {
        byte x = Wire.read();
      }
      break;
  }
  //Serial.println(F("Done New Data "));
}
