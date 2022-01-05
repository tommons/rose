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

#define ROSE_DEBUG 
#undef ROSE_DEBUG // comment this out to enable debug

#include "dmx.h"
#include "button.h"
#include "blink.h"
#include "servo_functions.h"
#include "indicator.h"

void setup() 
{
  // setup indicator first so we have access to the error state
  setupIndicator();
    
	#ifdef ROSE_DEBUG
	Serial.begin(9600);
  // while we wait for a serial connection, set the error state on the indicator
	while(!Serial)
  {
    indicator_state = INDICATOR_STATE::ERROR_SERIAL;
    handleIndicator();
  }
	#endif

  // resume init indicator state
  indicator_state = INDICATOR_STATE::INIT;
  
	#ifdef ROSE_DEBUG
	Serial.println("start setup");
	#endif

	setupDMX();
	setupButton();
	setupBlink();
	setupServos();

	#ifdef ROSE_DEBUG
	Serial.println("done setup");  
	#endif  
}

void loop() 
{
	handleDMX();
	handleButton();
	handleBlink();
	handleServos();
	handleIndicator();
}
