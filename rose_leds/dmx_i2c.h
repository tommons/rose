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

#ifndef DMX_I2C_H
#define DMX_I2C_H

#include <Wire.h>
#include <elapsedMillis.h>

#include "led_functions.h"

#define DMX_I2C_ID 4
#define DMX_GAMMA_CORRECT 0

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveI2CEvent(int howMany)
{
	const uint8_t msgID = Wire.read();

	switch( msgID )
	{
		case 2:
			const uint8_t control1 	= Wire.read();
			const uint8_t red1     	= Wire.read();
			const uint8_t green1   	= Wire.read();
			const uint8_t blue1    	= Wire.read();
			const uint8_t control2 	= Wire.read();
			const uint8_t red2     	= Wire.read();
			const uint8_t green2   	= Wire.read();
			const uint8_t blue2    	= Wire.read();
			const uint8_t servo    	= Wire.read();	  
			uint32_t color1   		= Adafruit_NeoPixel::Color(red1, green1, blue1);	  
			uint32_t color2   		= Adafruit_NeoPixel::Color(red2, green2, blue2);
			
			if( DMX_GAMMA_CORRECT == 1 )
			{
				color1 = Adafruit_NeoPixel::gamma32(color1);
				color2 = Adafruit_NeoPixel::gamma32(color2);
			}
			
			// set the new colors and control for the LEDs
			led_inner_ring.color_new = color1;
			if( control1 != led_inner_ring.state_new )
				led_inner_ring.stateElapsed_ms = 0;
			led_inner_ring.state_new = control1;
			
			
			led_outer_ring.color_new = color2;
			if( control2 != led_outer_ring.state_new )
				led_outer_ring.stateElapsed_ms = 0;			
			led_outer_ring.state_new = control2;
			
			#ifdef ROSE_DEBUG
			Serial.print("control1 "); Serial.println(control1);
			Serial.print("color1 "); Serial.println(color1,HEX);
			Serial.print("control2 "); Serial.println(control2);
			Serial.print("color2 "); Serial.println(color2,HEX);
			#endif 
			
			break;
		default:
			while(Wire.available())
			{
				byte x = Wire.read();
			}
		break;
	}
}

void setupDMXI2C()
{	
	Wire.begin(DMX_I2C_ID);                	// join i2c bus with address
	Wire.onReceive(receiveI2CEvent); 		// register event
}
	
#endif
