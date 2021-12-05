#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

#include "led.h"

// LEDs
#define PIN_LED_OUTER_RING   6
#define PIN_LED_INNER_RING   7
#define LED_COUNT_OUTER_RING 24
#define LED_COUNT_INNER_RING 24
#define LED_MAX_BRIGHTNESS 128
#define LED_STATE_DELAY_MS 50

Adafruit_NeoPixel led_outer_ring_neo(LED_COUNT_OUTER_RING, PIN_LED_OUTER_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_inner_ring_neo(LED_COUNT_INNER_RING, PIN_LED_INNER_RING, NEO_GRB + NEO_KHZ800);
Led led_outer_ring;
Led led_inner_ring;
long rainbow_firstPixelHue = 0;

void setupLeds()
{	
	led_inner_ring.color  		= 0;
	led_inner_ring.color_prev  	= 0;
	led_inner_ring.led 			= &led_inner_ring_neo;
	led_inner_ring_neo.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	led_inner_ring_neo.clear();
	led_inner_ring_neo.setBrightness(LED_MAX_BRIGHTNESS); // Set BRIGHTNESS (max = 255)
	led_inner_ring_neo.show();            // Turn OFF all pixels ASAP

	led_outer_ring.color  		= 0;
	led_inner_ring.color_prev  	= 0;
	led_outer_ring.led 			= &led_outer_ring_neo;
	led_outer_ring_neo.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	led_outer_ring_neo.clear();
	led_outer_ring_neo.setBrightness(LED_MAX_BRIGHTNESS); // Set BRIGHTNESS (max = 255)
	led_outer_ring_neo.show();            // Turn OFF all pixels ASAP
}

/*
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
	
*/

void colorFill( Led & led, const uint32_t & color )
{
	if( led.ledElapsed_ms > 50 )
	{
		led.color_prev = led.color_new;
		led.color      = color;
		
		for(uint32_t i=0; i < led.led->numPixels(); i++) // For each pixel in strip...
			led.led->setPixelColor(i, color);
		led.led->show();
		
		// reset the timer
		led.ledElapsed_ms = 0;
	}
}

void colorWipe( Led & led, const uint32_t & color, uint8_t wait_ms )
{
	if( led.new_state == true )
	{
		led.led->clear();
		//Serial.println(F("color wipe reset"));
	}
	
	if( led.ledElapsed_ms >= wait_ms && led.ledCounter < led.led->numPixels() )
	{		
		led.led->setPixelColor(led.ledCounter, color);
		led.led->show();
				
		// increment the pixel we color each time through
		led.ledCounter++;
		// reset the timer
		led.ledElapsed_ms = 0;	
	}
}

void theaterChase(Led & led, const uint32_t color, const uint16_t wait_ms) 
{
	if( led.ledCounter > 2 )
		led.ledCounter = 0;
		
	if( led.ledElapsed_ms >= wait_ms )
	{
		led.ledElapsed_ms = 0;
		
		led.led->clear();         //   Set all pixels in RAM to 0 (off)
		// 'c' counts up from 'b' to end of strip in steps of 3...
		for(uint8_t c=led.ledCounter; c < led.led->numPixels(); c += 3) 
		{
			led.led->setPixelColor(c, color); // Set pixel 'c' to value 'color'
		}
		led.led->show(); // Update strip with new contents

		led.ledCounter++;
	}
}

void rainbow( Led & led, const uint16_t wait_ms ) 
{
	// Hue of first pixel runs 5 complete loops through the color wheel.
	// Color wheel has a range of 65536 but it's OK if we roll over, so
	// just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
	// means we'll make 5*65536/256 = 1280 passes through this outer loop:
	if( led.ledElapsed_ms >= wait_ms )
	{
		const uint8_t numPixels = led.led->numPixels();
		
		if( rainbow_firstPixelHue >= 5*65536 )
			rainbow_firstPixelHue = 0;
		
		for(uint8_t i=0; i < numPixels; i++) 
		{ 
			// For each pixel in strip...
			// Offset pixel hue by an amount to make one full revolution of the
			// color wheel (range of 65536) along the length of the strip
			// (strip.numPixels() steps):
			int pixelHue = rainbow_firstPixelHue + (i * 65536L / numPixels);
			
			// strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
			// optionally add saturation and value (brightness) (each 0 to 255).
			// Here we're using just the single-argument hue variant. The result
			// is passed through strip.gamma32() to provide 'truer' colors
			// before assigning to each pixel:
			led.led->setPixelColor(i, Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue)));
		}
		
		led.led->show(); // Update strip with new contents

		rainbow_firstPixelHue += 256;
		led.ledElapsed_ms = 0;
		
	}
}

void handleLed(Led & led)
{
	// avoid race-conditions with the dmx handler and get a copy of the state
	const uint8_t newState = led.state_new;
	// require a change in state to exist for a time before we declare it
	if( newState != led.state && led.stateElapsed_ms > LED_STATE_DELAY_MS )
	{
		led.newState();
		led.state_prev 	= led.state;
		led.state 		= newState;
	}
	
	switch( led.state )
	{
		case 0: // Fill All
			colorFill( led, led.color_new );
			break;
		case 1: // No-op - use to pre-select colors without affecting control
			
			break;			
		case 10: // Color Wipe
			colorWipe( led, led.color_new, 500 );			
			break;
		case 11: // Color Wipe
			colorWipe( led, led.color_new, 200 );			
			break;
		case 12: // Color Wipe
			colorWipe( led, led.color_new, 100 );			
			break;			
		case 13: // Color Wipe
			colorWipe( led, led.color_new, 50 );			
			break;	
		case 20: 
			theaterChase( led, led.color_new, 50 );
			break;
		case 30: 
			rainbow( led, 10 );
			break;
	}		

	led.new_state 	= false;
}

void handleLeds()
{
	handleLed(led_inner_ring);
	handleLed(led_outer_ring);
}	
#endif
