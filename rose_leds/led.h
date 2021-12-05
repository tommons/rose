#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

struct Led 
{
	void newState()
	{
		ledCounter 		= 0;
		ledElapsed_ms 	= 0;
		new_state       = true;
	};
	void clear()
	{
		//led->clear();
	}
	
	uint8_t 	state;
	uint8_t		state_new;
	uint8_t 	state_prev;
	uint32_t 	color;
	uint32_t 	color_new;
	uint32_t 	color_prev;
	bool 		new_state;
	
	Adafruit_NeoPixel * led;
	elapsedMillis 		ledElapsed_ms;
	elapsedMillis 		stateElapsed_ms;
	uint32_t 			ledCounter;
};

#endif
