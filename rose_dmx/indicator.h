#ifndef INDICATOR_H
#define INDICATOR_H

#include <elapsedMillis.h>

#include "dmx.h"

elapsedMillis elapsedIndicator_ms;

#define PIN_INDICATOR 2

enum INDICATOR_STATE
{
	DELAY_1,
	DIGIT_3,
	DELAY_2,
	DIGIT_2,
	DELAY_3,
	DIGIT_1,
	DELAY_4,
	IDLE
};

uint8_t indicator_state = 0;
uint8_t indicator_output = 0;
uint8_t digit_counter = 0;

void setupIndicator()
{
	pinMode(PIN_INDICATOR, OUTPUT);
	
	elapsedIndicator_ms 	= 0;
	digit_counter 			= 0;
}

void handleIndicator()
{
	switch( indicator_state )
	{
		case DELAY_1:
      //Serial.println("Delay1");
			if( elapsedIndicator_ms > 3000 )
			{
				indicator_state = DIGIT_3;
				elapsedIndicator_ms = 0;
			}
			break;
		case DIGIT_3:
      Serial.println("Digit3");
			if( digit_counter < dmx_base_address_digit_3 )
			{
				if( elapsedIndicator_ms > 250 )
				{
					indicator_output = !indicator_output;
					
					digitalWrite(PIN_INDICATOR,indicator_output);
					
					if( indicator_output == 0 )
					{
						digit_counter++;
					}
				}
			}
			else
			{		
				indicator_state 	= DELAY_2;
				elapsedIndicator_ms = 0;
				digit_counter       = 0;
			}
			break;
		case DELAY_2:
      Serial.println("Delay2");   
			if( elapsedIndicator_ms > 3000 )
			{
				indicator_state 	= DIGIT_2;
				elapsedIndicator_ms = 0;
			}
			break;	
		case DIGIT_2:
      Serial.println("Digit2");   
			if( digit_counter < dmx_base_address_digit_2 )
			{
				if( elapsedIndicator_ms > 250 )
				{
					indicator_output = !indicator_output;
					
					digitalWrite(PIN_INDICATOR,indicator_output);
					
					if( indicator_output == 0 )
					{
						digit_counter++;
					}
				}
			}
			else
			{		
				indicator_state 	= DELAY_3;
				elapsedIndicator_ms = 0;
				digit_counter       = 0;
			}
			break;
		case DELAY_3:
      Serial.println("Delay3");   
			if( elapsedIndicator_ms > 3000 )
			{
				indicator_state 	= DIGIT_1;
				elapsedIndicator_ms = 0;
			}
			break;	
		case DIGIT_1:
      Serial.println("Digit1");   
			if( digit_counter < dmx_base_address_digit_1 )
			{
				if( elapsedIndicator_ms > 250 )
				{
					indicator_output = !indicator_output;
					
					digitalWrite(PIN_INDICATOR,indicator_output);
					
					if( indicator_output == 0 )
					{
						digit_counter++;
					}
				}
			}
			else
			{		
				indicator_state 	= IDLE;
				elapsedIndicator_ms = 0;
				digit_counter       = 0;
			}
			break;
		case IDLE:
      Serial.println("IDLE");    
			break;
	}
}

#endif
