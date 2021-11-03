#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>
#include "setup.h"

uint8_t wipeForward8_v[8];
uint8_t wipeBackward8_v[8];

elapsedMillis ledElapsed1_ms = 0;
elapsedMillis ledElapsed2_ms = 0;
elapsedMillis ledUpdate1_ms = 0;
elapsedMillis ledUpdate2_ms = 0;
const uint16_t ledFadeUpdate_ms = 50;

uint32_t ledCounter1 = 0;
uint32_t ledCounter2 = 0;

void led_setup()
{
	for( int i=0; i < 8; i++ )
	{
		// dim to bright
		wipeForward8_v[i] = 255 / (7-i+1);
		// bright to dim
		wipeBackward8_v[i] = 255 / (i+1);
	}
}

void clearLeds()
{
	led_outer_ring.clear();
	led_outer_ring.show();
	led_inner_ring.clear();
	led_inner_ring.show();
}

void colorFill( Adafruit_NeoPixel & led, 
				const uint32_t color )
{
	uint32_t gammaColor = led.gamma32(color);
	
	for(uint32_t i=0; i < led.numPixels(); i++) // For each pixel in strip...
		led.setPixelColor(i, gammaColor); //  Set pixel's color (in RAM)	
	led.show();
}
				
void fadeIn( 	Adafruit_NeoPixel & led, 
				elapsedMillis & ledElapsed_ms, 
				elapsedMillis & ledUpdate_ms, 
				const uint16_t h, 
				const uint8_t s, 
				const uint8_t v,
				const unsigned long fadeTime_ms )
{
	// Fade in from off
	if( ledUpdate_ms >= ledFadeUpdate_ms )
	{
		const unsigned long eTime_ms = ledElapsed_ms;
		
		const uint8_t v1 = map( min(eTime_ms,fadeTime_ms), 0, fadeTime_ms, 0, 255);

		colorFill( led, led.ColorHSV(h, s, v1) );
		ledUpdate_ms = 0;		
	}
}

void fadeOut( 	Adafruit_NeoPixel & led, 
				elapsedMillis & ledElapsed_ms, 
				elapsedMillis & ledUpdate_ms, 
				const uint16_t h, 
				const uint8_t s, 
				const uint8_t v,
				const unsigned long fadeTime_ms )
{
	// Fade out from on
	if( ledUpdate_ms >= ledFadeUpdate_ms )
	{
		const unsigned long eTime_ms = ledElapsed_ms;

		const uint8_t v1 = map( min(eTime_ms,fadeTime_ms), 0, fadeTime_ms, 255, 0);

		colorFill( led, led.ColorHSV(h, s, v1) );
		ledUpdate_ms = 0;
	}
}

void chase( Adafruit_NeoPixel & led, 
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
		ledElapsed1_ms = 0;
		ledElapsed2_ms = 0;
		ledUpdate1_ms = 0;
		ledUpdate2_ms = 0;
		ledCounter1 = 0;
		ledCounter2 = 0;
	}
	
	chase( led_outer_ring, 
			ledElapsed1_ms,
			led_outer_ring.Color(0,255,0), // green
			led_outer_ring.Color(255,0,0), //red
			chaseInterval_ms,
			chaseOffset,
			ledCounter1,
			false);
  
  	chase( led_inner_ring, 
			ledElapsed2_ms,
			led_outer_ring.Color(64,64,64), // dimmer white
			led_outer_ring.Color(255,255,255), // white
			chaseInterval_ms,
			chaseOffset,
			ledCounter2,
			false );
}
	
void wipeRed(const uint32_t state_counter, const uint16_t wipeInterval_ms, bool forwardDirection)
{
	const uint8_t wipeLength 		= 5;
	if( state_counter == 0 )
	{
		ledElapsed1_ms = 0;
		ledElapsed2_ms = 0;
		ledUpdate1_ms = 0;
		ledUpdate2_ms = 0;
		ledCounter1 = 0;
		ledCounter2 = 0;
	}
	
	wipe( led_outer_ring, 
			ledElapsed1_ms,
			0, 255, 255, // RED
			wipeInterval_ms,
			wipeLength,
			ledCounter1,
			forwardDirection );
  
	wipe( led_inner_ring, 
			ledElapsed2_ms,
			0, 255, 255, // RED
			wipeInterval_ms,
			8,
			ledCounter2,
			forwardDirection );
}	

void fadeInRed( const uint32_t state_counter )
{
	const uint32_t fadeTime_ms = 5000;
	
	if( state_counter == 0 )
	{
		ledElapsed1_ms = 0;
		ledElapsed2_ms = 0;
		ledUpdate1_ms = 0;
		ledUpdate2_ms = 0;
		ledCounter1 = 0;
		ledCounter2 = 0;		
	}
	
	fadeIn( led_outer_ring, ledElapsed1_ms, ledUpdate1_ms, 0, 255, 255, fadeTime_ms ); // RED
	fadeIn( led_inner_ring, ledElapsed2_ms, ledUpdate2_ms, 0, 0, 255, fadeTime_ms ); // WHITE	  
}

void fadeOutPurple( const uint32_t state_counter )
{
	const uint32_t fadeTime_ms = 3000;

	if( state_counter == 0 )
	{
		ledElapsed1_ms = 0;
		ledElapsed2_ms = 0;
		ledUpdate1_ms = 0;
		ledUpdate2_ms = 0;
		ledCounter1 = 0;
		ledCounter2 = 0;		
	}
	
	fadeOut( led_outer_ring, ledElapsed1_ms, ledUpdate1_ms, 65535*306/360, 255, 255, fadeTime_ms ); // PURPLE
	fadeOut( led_inner_ring, ledElapsed2_ms, ledUpdate2_ms, 0, 0, 255, fadeTime_ms ); // WHITE
}  
  
#endif
