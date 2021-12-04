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
	};
	void clear()
	{
		led->clear();
	}
	
	uint32_t newColor;
	uint32_t prevColor;
	
	Adafruit_NeoPixel * led;
	elapsedMillis 		ledElapsed_ms;
	uint32_t 			ledCounter;
};

/*
class Led
{
  public:
	Led( Adafruit_NeoPixel * led_in, char * name );
	
	void clear();
	void show();
	void newState();
	void setPixelColor(const uint8_t & id, const uint32_t & color) 
	{
		led->setPixelColor(id, color);
	};
	const uint16_t numPixels() const
	{
		return led->numPixels();
	};
	uint32_t newColor;
	uint32_t prevColor;
	
	char * name_;
	Adafruit_NeoPixel * led;
	
	elapsedMillis ledElapsed_ms;
	uint32_t ledCounter;
	
};
*/

#endif
