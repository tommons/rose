#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>

void colorFill( Adafruit_NeoPixel & led, 
				const uint32_t color )
{
	uint32_t gammaColor = led.gamma32(color);
	
	for(uint32_t i=0; i < led.numPixels(); i++) // For each pixel in strip...
		led.setPixelColor(i, gammaColor); //  Set pixel's color (in RAM)	
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

void fadeOut( 	Adafruit_NeoPixel & led, 
				const uint32_t & state_counter, 
				const uint16_t h, 
				const uint8_t s, 
				const uint8_t v,
				const uint32_t fadeCount )
{
	// Fade out from on
	if( state_counter <= fadeCount )
	{
		const uint8_t v1 = map( state_counter, 0, fadeCount, 255, 0);

		colorFill( led, led.ColorHSV(h, s, v1) );
	}
}

uint32_t chaseCounter1 = 0;
uint32_t chaseCounter2 = 0;

void chase( Adafruit_NeoPixel & led, 
			const uint32_t & state_counter, 
			uint32_t color1,
			uint32_t color2,
			const uint32_t interval,
			const uint8_t chaseOffset,
			uint32_t & chaseCounter,
			boolean forwardDirection )
{
	if( state_counter % interval == 0 )
	{
		for(uint32_t i=0; i < led.numPixels(); i++) // For each pixel in strip...
			led.setPixelColor(i, led.gamma32(color1));
			
		uint32_t chaseStart 	= chaseCounter % chaseOffset;
		
		if( forwardDirection == false )
		{
			chaseStart = (chaseOffset-1) - chaseStart;
		}
		
		for(uint32_t i=chaseStart; i < led.numPixels(); i += chaseOffset)
			led.setPixelColor(i, led.gamma32(color2));
				
		led.show();	

		chaseCounter++;
	}
}

uint32_t wipeCounter1 = 0;
uint32_t wipeCounter2 = 0;

void wipe( Adafruit_NeoPixel & led, 
			const uint32_t & state_counter, 
			const uint16_t h, 
			const uint8_t s, 
			const uint8_t v,
			const uint32_t interval,
			const uint8_t wipeLength,
			uint32_t & wipeCounter,
			boolean forwardDirection )
{
	if( state_counter % interval == 0 )
	{
		led.clear();
		const uint32_t numPixels = led.numPixels();
		
		uint32_t wipeStart 	= wipeCounter % numPixels;
		if( forwardDirection == false )
		{
			wipeStart = (numPixels-1) - wipeStart;
		}
		
		int pixel = 0;
		
		for( uint32_t i=0; i < wipeLength; i++ )
		{
			// calculate pixel index behind the starting index
			if( forwardDirection == true )
				pixel = wipeStart - i;
			else
				pixel = wipeStart + i;
			
			if( pixel < 0 )
			{
				pixel += numPixels;
			}
			else if( pixel >= numPixels )
			{
				pixel -= numPixels;
			}
			
			// dim pixel in relation to how far behind the front
			uint8_t v1 = v / (i+1);
			
			led.setPixelColor(pixel, led.gamma32(led.ColorHSV(h, s, v1)));
		}
						
		led.show();	

		wipeCounter++;
	}
}

#endif
