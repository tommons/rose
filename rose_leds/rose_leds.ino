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

// Require this to run on a Leonardo
#ifndef ARDUINO_AVR_UNO
#error "ROSE LED requires an Uno to run on"
#endif

#define ROSE_DEBUG 
#undef ROSE_DEBUG // comment this out to enable debug

#include "dmx_i2c.h"
#include "led_functions.h"
#include "blink.h"

void setup() 
{
	#ifdef ROSE_DEBUG
	Serial.begin(9600);           // start serial for output
	while(!Serial);
	#endif

	setupDMXI2C();
	setupLeds();
	setupBlink();
}

void loop() 
{
	handleLeds();
	handleBlink();
}
