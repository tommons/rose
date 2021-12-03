#include "led.h"

Led::Led( Adafruit_NeoPixel * led_in, char * name ):led(led_in), name_(name), ledElapsed_ms(0), ledCounter(0)
{
	led->begin();
	clear();
};
	
void Led::clear()
{
	led->clear();
	led->show();
};

void Led::show()
{
	led->show();
};

void Led::newState()
{
	ledElapsed_ms 	= 0;
	ledCounter 		= 0;
};