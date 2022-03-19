
#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

#define LED_PIN    9

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 24
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

elapsedMillis ledElapsed1_ms = 0;
void setup() {
  // put your setup code here, to run once:

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(127); // Set BRIGHTNESS to about 1/5 (max = 255)
  
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

uint8_t ledCount = 0;
void wipe(const uint16_t h, const uint8_t s, const uint8_t v, int wait)
{
  if( ledElapsed1_ms > wait )
  {
    strip.clear();
  
    uint8_t startPixel = ledCount % 24;
    for( uint8_t i=0; i < 8; i++ )
    {
      uint8_t v1 = v / (7-i+1);
      strip.setPixelColor((startPixel + i) % 24, strip.gamma32(strip.ColorHSV(h, s, v1)) );  
    }
  
    strip.show();
  
    ledCount++;
    ledElapsed1_ms = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
   wipe(0, 255, 255, 10);
  //colorWipe(strip.Color(255,   0,   0), 10); // Red
  //colorWipe(strip.Color(  0, 255,   0), 10); // Green
  //colorWipe(strip.Color(  0,   0, 255), 10); // Blue
  delay(10);
}
