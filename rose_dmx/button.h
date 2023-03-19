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

#ifndef BUTTON_H
#define BUTTON_H

#include <elapsedMillis.h>

#include "dmx.h"
#include "indicator.h"

elapsedMillis elapsedButton_petal_ms;
elapsedMillis elapsedButton_led_ms;
elapsedMillis elapsedButton_petal2_ms;
elapsedMillis elapsedButton_led2_ms;
elapsedMillis elpasedButtonCheck_ms;
elapsedMillis elpasedButtonLowTimer_ms;

#define BUTTON_CHECK_TIME_MS 50

#define PIN_PETAL 3
#define PIN_LED   4

uint8_t button_led_state 		= 0;
uint8_t buttonState_petal_prev 	= LOW;
uint8_t buttonState_led_prev 	= LOW;

void setupButton()
{
	pinMode(PIN_PETAL, INPUT_PULLUP);
	pinMode(PIN_LED, INPUT_PULLUP);
	
	elapsedButton_petal_ms 	= 0;
	elapsedButton_led_ms 	= 0;
	elpasedButtonCheck_ms   = 0;
}

void handleButton()
{
	if( elpasedButtonCheck_ms > BUTTON_CHECK_TIME_MS )
	{
		// get the state of the buttons
		const uint8_t buttonState_petal 	= !digitalRead(PIN_PETAL);
		const uint8_t buttonState_led 		= !digitalRead(PIN_LED);
		
		// if a button has been pressed, suspend DMX control until we say it's ok to resume
		if( buttonState_petal == HIGH || buttonState_led == HIGH )
		{
			suspend_dmxI2C = true;
		}
		else if( buttonState_petal == LOW && buttonState_led == LOW ) // both buttons are low
		{
			// if both buttons are low and enough time has passed since the transition to low
			// allow dmx control
			if( elpasedButtonLowTimer_ms > 5000 )
				suspend_dmxI2C = false;
		}
		
		// if petal button isn't pushed
		if( buttonState_petal == LOW )
		{
			elapsedButton_petal_ms 		= 0;
			elapsedButton_petal2_ms 	= 0;
		}
		
		// if led button isn't pushed
		if( buttonState_led == LOW )
		{
			elapsedButton_led_ms 		= 0;
			elapsedButton_led2_ms 		= 0;
		}
		
		// look for button transitions
		if( buttonState_petal != buttonState_petal_prev || buttonState_led != buttonState_led_prev )
		{
			// if we've transitioned from high to low, reset a low button timer
			if( buttonState_petal == LOW || buttonState_led == LOW )
			{
				elpasedButtonLowTimer_ms = 0;
			}
		}		
		
		// if petal button is pushed for long enough
		if( elapsedButton_petal_ms > 1000 && buttonState_led == LOW )
		{
			#ifdef ROSE_DEBUG
			Serial.println("petal button push");
			#endif
			
			// cycle through petal servos
			dmx_servo_value += 32;

			elapsedButton_petal_ms = 0;
			sendDMXI2C(true);

			if( dmx_servo_value >= 128 )
				dmx_servo_value = 0;     
		}
		
		// if led button is pushed for long enough
		if( elapsedButton_led_ms > 1000 && buttonState_petal == LOW )
		{          
			button_led_state++;
			if( button_led_state > 3 )
				button_led_state = 0;

			#ifdef ROSE_DEBUG
			Serial.print("led button push. button_led_state: "); Serial.println(button_led_state);
			#endif
			
			// some preset led states to cycle through
			if( button_led_state == 1 )
			{
				dmx_control1_value  = 30;
				dmx_led1_r_value    = 255;
				dmx_led1_g_value    = 255;
				dmx_led1_b_value    = 255;
				dmx_control2_value  = 30;
				dmx_led2_r_value    = 255;
				dmx_led2_g_value    = 255;
				dmx_led2_b_value    = 255;       
			}
			else if( button_led_state == 2 )
			{
				dmx_control1_value  = 40;
				dmx_led1_r_value    = 255;
				dmx_led1_g_value    = 255;
				dmx_led1_b_value    = 255;
				dmx_control2_value  = 40;
				dmx_led2_r_value    = 255;
				dmx_led2_g_value    = 255;
				dmx_led2_b_value    = 255;       
			}  
			else if( button_led_state == 3 )
			{
				dmx_control1_value  = 52;
				dmx_led1_r_value    = 255;
				dmx_led1_g_value    = 0;
				dmx_led1_b_value    = 0;
				dmx_control2_value  = 62;
				dmx_led2_r_value    = 255;
				dmx_led2_g_value    = 0;
				dmx_led2_b_value    = 0;       
			}   
			else
			{
				dmx_control1_value  = 0;
				dmx_led1_r_value    = 0;
				dmx_led1_g_value    = 0;
				dmx_led1_b_value    = 0;        
				dmx_control2_value  = 0;
				dmx_led2_r_value    = 0;
				dmx_led2_g_value    = 0;
				dmx_led2_b_value    = 0;        
			}
			
			elapsedButton_led_ms = 0;
			sendDMXI2C(true);
		}	

		// backdoor long button pushes to override dmx address
		if( elapsedButton_led2_ms > 5000 && elapsedButton_petal2_ms > 5000 )
		{
			dmx_base_address 			= 1;
			setDMXAddresses();
			elapsedButton_led2_ms 		= 0;
			elapsedButton_petal2_ms 	= 0;
			// reset normal button counters to avoid accidental trigger on release
			elapsedButton_led_ms 		= 0;			
			elapsedButton_petal_ms 		= 0;
			indicator_state   			= INDICATOR_STATE::INIT;
		}
		
		// save the old state of the buttons
		buttonState_petal_prev 				= buttonState_petal;
		buttonState_led_prev 				= buttonState_led;
	}

}

#endif
