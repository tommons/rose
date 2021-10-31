#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>
#include "setup.h"

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

elapsedMillis ledElapsed1_ms = 0;
elapsedMillis ledElapsed2_ms = 0;
uint32_t ledCounter1 = 0;
uint32_t ledCounter2 = 0;

void chase( Adafruit_NeoPixel & led, 
			const uint32_t & state_counter, 
			elapsedMillis & elapsed_ms, 
			uint32_t color1,
			uint32_t color2,
			const uint32_t interval_ms,
			const uint8_t chaseOffset,
			uint32_t & ledCounter,
			boolean forwardDirection )
{
	if( elapsed_ms > interval_ms )
	{
		elapsed_ms = 0;

		for(uint32_t i=0; i < led.numPixels(); i++) // For each pixel in strip...
			led.setPixelColor(i, led.gamma32(color1));
			
		uint32_t chaseStart 	= ledCounter % chaseOffset;
		
		if( forwardDirection == false )
		{
			chaseStart = (chaseOffset-1) - chaseStart;
		}
		
		for(uint32_t i=chaseStart; i < led.numPixels(); i += chaseOffset)
			led.setPixelColor(i, led.gamma32(color2));
				
		led.show();	

		ledCounter++;
	}
}

void wipe( Adafruit_NeoPixel & led, 
			const uint32_t & state_counter, 
			elapsedMillis & elapsed_ms, 
			const uint16_t h, 
			const uint8_t s, 
			const uint8_t v,
			const uint32_t interval_ms,
			const uint8_t wipeLength,
			uint32_t & ledCounter,
			boolean forwardDirection )
{
	if( elapsed_ms > interval_ms )
	{
		elapsed_ms = 0;
		
		led.clear();
		const uint32_t numPixels = led.numPixels();
		
		uint32_t wipeStart 	= ledCounter % numPixels;
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

		ledCounter++;
	}
}

void chaseBackward(const uint32_t state_counter)
{
	const uint32_t chaseInterval_ms = 50;
	const uint8_t chaseOffset 		= 3;
	if( state_counter == 0 )
	{
		ledCounter1 = 0;
		ledCounter2 = 0;
	}
	
	chase( led_outer_ring, 
			state_counter, 
			ledElapsed1_ms,
			led_outer_ring.Color(0,255,0), // green
			led_outer_ring.Color(255,0,0), //red
			chaseInterval_ms,
			chaseOffset,
			ledCounter1,
			false);
  
  	chase( led_inner_ring, 
			state_counter, 
			ledElapsed2_ms,
			led_outer_ring.Color(64,64,64), // dimmer white
			led_outer_ring.Color(255,255,255), // white
			chaseInterval_ms,
			chaseOffset,
			ledCounter2,
			false );
}
	
void wipeRed(const uint32_t state_counter, bool forwardDirection)
{
	const uint32_t wipeInterval_ms 	= 50;
	const uint8_t wipeLength 		= 5;
	if( state_counter == 0 )
	{
		ledCounter1 = 0;
		ledCounter2 = 0;
	}
	
	wipe( led_outer_ring, 
			state_counter, 
			ledElapsed1_ms,
			0, 255, 255, // RED
			wipeInterval_ms,
			wipeLength,
			ledCounter1,
			forwardDirection );
  
	wipe( led_inner_ring, 
			state_counter, 
			ledElapsed2_ms,
			0, 255, 255, // RED
			wipeInterval_ms,
			wipeLength,
			ledCounter2,
			forwardDirection );
}	

void fadeInRed( const uint32_t state_counter )
{
	const uint32_t fadeTime_ms = 5000;
	const uint32_t fadeupcount = fadeTime_ms / DELAY_MS;

	fadeIn( led_outer_ring, state_counter, 0, 255, 255, fadeupcount ); // RED
	fadeIn( led_inner_ring, state_counter, 0, 0, 255, fadeupcount ); // WHITE	  
}

void fadeOutPurple( const uint32_t state_counter )
{
	const uint32_t fadeTime_ms = 3000;
	const uint32_t fadeoutcount = fadeTime_ms / DELAY_MS;

	fadeOut( led_outer_ring, state_counter, 65535*306/360, 255, 255, fadeoutcount ); // PURPLE
	fadeOut( led_inner_ring, state_counter, 0, 0, 255, fadeoutcount ); // WHITE
}  
  
#endif
