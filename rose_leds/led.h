// 
// The MIT License (MIT)
// 
// Copyright (c) 2022 Thomas M. Hall
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef LED_H
#define LED_H

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>

// Class to manage the persisent state info of an LED strip
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
