#ifndef BLINK_H
#define BLINK_H

#include <elapsedMillis.h>

elapsedMillis blink_ms;
uint8_t blinkState;

void setupBlink()
{
	pinMode(LED_BUILTIN, OUTPUT);
	blink_ms = 0;
	blinkState = LOW;
}

void handleBlink()
{
	if( blink_ms > 1000 )
	{
		blink_ms = 0;
		if( blinkState == HIGH )
		{
			blinkState = LOW;
		}
		else
		{
			blinkState = HIGH;
		}
		digitalWrite(LED_BUILTIN, blinkState);
	}
}

#endif