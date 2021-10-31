#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>

void colorFill( Adafruit_NeoPixel & led, 
				const uint32_t color )
{
	for(uint32_t i=0; i < led.numPixels(); i++) // For each pixel in strip...
		led.setPixelColor(i, color); //  Set pixel's color (in RAM)	
	led.show();		
}
				
void fadeIn( 	Adafruit_NeoPixel & led, 
				const uint32_t & state_counter, 
				const uint16_t h, 
				const uint8_t s, 
				const uint8_t v,
				const uint32_t fadeCount )
{
	// Fade in from off
	if( state_counter <= fadeCount )
	{
		const uint8_t v1 = map( state_counter, 0, fadeCount, 0, 255);

		colorFill( led, led.ColorHSV(h, s, v1) );
	}
}

#endif