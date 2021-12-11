#ifndef BUTTON_H
#define BUTTON_H

#include <elapsedMillis.h>

#include "dmx.h"

elapsedMillis elapsedButton_petal_ms;
elapsedMillis elapsedButton_led_ms;
elapsedMillis elpasedButtonCheck_ms;

#define BUTTON_CHECK_TIME_MS 50

#define PIN_PETAL 3
#define PIN_LED   4

uint8_t button_led_state = 0;

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
		const uint8_t buttonState_petal 	= !digitalRead(PIN_PETAL);
		const uint8_t buttonState_led 		= !digitalRead(PIN_LED);
		
		if( buttonState_petal == LOW )
		{
			elapsedButton_petal_ms = 0;
		}
		
		if( buttonState_led == LOW )
		{
			elapsedButton_led_ms = 0;
		}
		
		// if petal button is pushed
		if( elapsedButton_petal_ms > 1000 )
		{
      #ifdef ROSE_DEBUG
      Serial.println("petal button push");
      #endif
      
			dmx_servo_value += 32;

			elapsedButton_petal_ms = 0;
			sendDMXI2C();

      if( dmx_servo_value >= 128 )
        dmx_servo_value = 0;     
		}
		
		// if led button is pushed
		if( elapsedButton_led_ms > 1000 )
		{          
			button_led_state++;
      if( button_led_state > 3 )
        button_led_state = 0;

      #ifdef ROSE_DEBUG
      Serial.print("led button push. button_led_state: "); Serial.println(button_led_state);
      #endif
      
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
			sendDMXI2C();
		}		
	}

}

#endif
