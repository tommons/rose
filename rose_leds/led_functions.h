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

#define WIPE_CHASE_LENGTH 8
uint8_t wipeForward_value[WIPE_CHASE_LENGTH];
uint8_t wipeBackward_value[WIPE_CHASE_LENGTH];

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
	
	// initialize wipe value arrays
	for( uint8_t i=0; i < WIPE_CHASE_LENGTH; i++ )
	{
		// dim to bright
		wipeForward_value[i] = 255L / (WIPE_CHASE_LENGTH-i);
		// bright to dim
		wipeBackward_value[i] = 255L / (i+1);

		#if 0
		Serial.print(i); Serial.print(" "); 
		Serial.print(wipeForward_value[i]); Serial.print(" "); 
		Serial.println(wipeBackward_value[i]);
		#endif
	}	
}

void clearLeds( Led & led )
{
	if( led.new_state == true )
	{
		led.led->clear();
		led.led->show();
	}
}

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

	if( led.ledElapsed_ms >= wait_ms )
	{
		if( led.ledCounter > 2 )
			led.ledCounter = 0;
		
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

void rainbow( Led & led, const uint32_t color, const uint16_t wait_ms ) 
{
	// Hue of first pixel runs 5 complete loops through the color wheel.
	// Color wheel has a range of 65536 but it's OK if we roll over, so
	// just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
	// means we'll make 5*65536/256 = 1280 passes through this outer loop:
	if( led.ledElapsed_ms >= wait_ms )
	{
		const uint8_t numPixels = led.led->numPixels();
		
		if( rainbow_firstPixelHue >= 65536 )
			rainbow_firstPixelHue = 0;
		
		const long HuePerPixel = 65536L / numPixels;
		const uint8_t sat = 255;
		const uint8_t val = color >> 16 & 0xFF; // red channel is value
		for(uint8_t i=0; i < numPixels; i++) 
		{ 
			// For each pixel in strip...
			// Offset pixel hue by an amount to make one full revolution of the
			// color wheel (range of 65536) along the length of the strip
			// (strip.numPixels() steps):
			int pixelHue = rainbow_firstPixelHue + (i * HuePerPixel);
			
			// strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
			// optionally add saturation and value (brightness) (each 0 to 255).
			// Here we're using just the single-argument hue variant. The result
			// is passed through strip.gamma32() to provide 'truer' colors
			// before assigning to each pixel:
			led.led->setPixelColor(i, Adafruit_NeoPixel::gamma32( Adafruit_NeoPixel::ColorHSV(pixelHue, sat, val) ) );
		}
		
		led.led->show(); // Update strip with new contents

		rainbow_firstPixelHue += 256;
		led.ledElapsed_ms = 0;
		
	}
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow( Led & led, const uint32_t color, const uint16_t wait_ms ) 
{
	if( led.ledElapsed_ms >= wait_ms )
	{
		if( led.ledCounter > 2 )
			led.ledCounter = 0;
	
		if( rainbow_firstPixelHue >= 65536 )
			rainbow_firstPixelHue = 0;     // First pixel starts at red (hue 0)
	
		const uint8_t numPixels = led.led->numPixels();
		const long HuePerPixel = 65536L / numPixels;
		const uint8_t sat = 255;
		const uint8_t val = color >> 16 & 0xFF; // red channel is value
		
		//  'b' counts from 0 to 2...
		led.led->clear();         //   Set all pixels in RAM to 0 (off)
		
		// 'c' counts up from 'b' to end of strip in increments of 3...
		for(uint8_t c = led.ledCounter; c < numPixels; c += 3) 
		{
			// hue of pixel 'c' is offset by an amount to make one full
			// revolution of the color wheel (range 65536) along the length
			// of the strip (strip.numPixels() steps):
			int      hue   = rainbow_firstPixelHue + c * HuePerPixel;
			uint32_t color = Adafruit_NeoPixel::gamma32( Adafruit_NeoPixel::ColorHSV(hue, sat, val) ); // hue -> RGB
			led.led->setPixelColor(c, color); // Set pixel 'c' to value 'color'
		}
		led.led->show();                // Update strip with new contents
		
		rainbow_firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
		led.ledCounter++;
		led.ledElapsed_ms = 0;
	}
}

void wipeChaseForward( Led & led, const uint32_t & color, uint8_t wait_ms )
{
	if( led.ledElapsed_ms >= wait_ms )
	{		
		const uint8_t numPixels = led.led->numPixels();
		const uint8_t r = color >> 16 & 0xFF;
		const uint8_t g = color >> 8  & 0xFF;
		const uint8_t b = color       & 0xFF;
		
		if( led.ledCounter >= numPixels )
			led.ledCounter = 0;
		
		led.led->clear();
		
		const uint8_t startPixel = led.ledCounter;
		
		for( uint8_t i = 0; i < WIPE_CHASE_LENGTH; i++ )
		{
			const uint8_t p 				= (startPixel+i) % numPixels;
			const uint8_t wipeBrightness 	= wipeForward_value[i];;
			
			const uint8_t r1 				= ( r * wipeBrightness ) >> 8;
			const uint8_t g1 				= ( g * wipeBrightness ) >> 8;
			const uint8_t b1 				= ( b * wipeBrightness ) >> 8;
			
			led.led->setPixelColor(p, r1, g1, b1);
		}
		
		led.led->show();

		// increment the pixel we color each time through
		led.ledCounter++;
		// reset the timer
		led.ledElapsed_ms = 0;	
	}
}

void wipeChaseBackward( Led & led, const uint32_t & color, uint8_t wait_ms )
{
	if( led.ledElapsed_ms >= wait_ms )
	{		
		const uint8_t numPixels = led.led->numPixels();
		const uint8_t r = color >> 16 & 0xFF;
		const uint8_t g = color >> 8  & 0xFF;
		const uint8_t b = color       & 0xFF;
		
		if( led.ledCounter >= numPixels )
			led.ledCounter = 0;
		
		led.led->clear();
		
		const uint8_t startPixel = led.ledCounter;
		
		for( uint8_t i = 0; i < WIPE_CHASE_LENGTH; i++ )
		{
			const uint8_t p 				= (startPixel+i) % numPixels;
			const uint8_t wipeBrightness 	= wipeBackward_value[i];;
		
			const uint8_t r1 				= ( r * wipeBrightness ) >> 8;
			const uint8_t g1 				= ( g * wipeBrightness ) >> 8;
			const uint8_t b1 				= ( b * wipeBrightness ) >> 8;
			
			led.led->setPixelColor(p, r1, g1, b1);
		}
		
		led.led->show();
					
		// increment the pixel we color each time through
		if( led.ledCounter == 0 )
			led.ledCounter = numPixels-1;
		else
			led.ledCounter--;
		
		// reset the timer
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
			rainbow( led, led.color_new, 10 );
			break;
		case 40:
			theaterChaseRainbow(led, led.color_new, 50);
			break;
		case 50:
			wipeChaseForward( led, led.color_new, 2000 / 24 );
			break;			
		case 51:
			wipeChaseForward( led, led.color_new, 1000 / 24 );
			break;
		case 52:
			wipeChaseForward( led, led.color_new, 500 / 24 );
			break;	
		case 53:
			wipeChaseForward( led, led.color_new, 200 / 24 );
			break;
		case 60:
			wipeChaseBackward( led, led.color_new, 2000 / 24 );
			break;
		case 61:
			wipeChaseBackward( led, led.color_new, 1000 / 24 );
			break;
		case 62:
			wipeChaseBackward( led, led.color_new, 500 / 24 );
			break;	
		case 63:
			wipeChaseBackward( led, led.color_new, 200 / 24 );
			break;	
		default:
			clearLeds(led);
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
