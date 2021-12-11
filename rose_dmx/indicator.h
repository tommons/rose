#ifndef INDICATOR_H
#define INDICATOR_H

#include <elapsedMillis.h>

#include "dmx.h"

elapsedMillis elapsedIndicator_ms;
elapsedMillis elapsedIndicator1_ms;
elapsedMillis totalElapsed_ms;

#define PIN_INDICATOR 5

enum INDICATOR_STATE
{
  INIT,
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
uint8_t init_counter = 0;

void setupIndicator()
{
	pinMode(PIN_INDICATOR, OUTPUT);
	digitalWrite(PIN_INDICATOR,LOW);
 
	elapsedIndicator_ms 	= 0;
  elapsedIndicator1_ms  = 0;
	digit_counter 			  = 0;
  indicator_state       = INDICATOR_STATE::INIT;
  totalElapsed_ms       = 0;
}

void handleIndicator()
{
  if( elapsedIndicator1_ms > 10 )
  {
  	switch( indicator_state )
  	{
      case INDICATOR_STATE::INIT:
        if( elapsedIndicator_ms > 250 )
        {
          indicator_output    = !indicator_output;

          #ifdef INDICATOR_DEBUG
          Serial.print("init: "); Serial.print(totalElapsed_ms); Serial.print(" ind: "); Serial.println(indicator_output);
          #endif

          digitalWrite(PIN_INDICATOR,indicator_output);
          elapsedIndicator_ms = 0;
          init_counter++;
          
          if( init_counter > 6 )
          {
            indicator_output    = 0;
            digitalWrite(PIN_INDICATOR,indicator_output);
            indicator_state     = INDICATOR_STATE::DELAY_1;    
            elapsedIndicator_ms = 0; 
            init_counter        = 0;              
          }
        }
        break;
  		case INDICATOR_STATE::DELAY_1:
  			if( elapsedIndicator_ms > 3000 )
  			{
          #ifdef INDICATOR_DEBUG
          Serial.print("delay1: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
          #endif
          
  				indicator_state     = INDICATOR_STATE::DIGIT_3;
  				elapsedIndicator_ms = 0;
  			}
  			break;
  		case INDICATOR_STATE::DIGIT_3:
        //Serial.println("Digit3");
  			if( digit_counter < dmx_base_address_digit_3 )
  			{
  				if( elapsedIndicator_ms > 500 )
  				{
  					indicator_output = !indicator_output;
           
            #ifdef INDICATOR_DEBUG
  					Serial.print("digit3: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
            #endif
           
  					digitalWrite(PIN_INDICATOR,indicator_output);
  					elapsedIndicator_ms = 0;
  					if( indicator_output == 0 )
  					{
  						digit_counter++;
  					}
  				}
  			}
  			else
  			{		
  				indicator_state 	  = INDICATOR_STATE::DELAY_2;
  				elapsedIndicator_ms = 0;
  				digit_counter       = 0;
          indicator_output    = 0;
          digitalWrite(PIN_INDICATOR,indicator_output);          
  			}
  			break;
  		case INDICATOR_STATE::DELAY_2:
        //Serial.println("Delay2");   
  			if( elapsedIndicator_ms > 1000 )
  			{
  				indicator_state 	= INDICATOR_STATE::DIGIT_2;
  				elapsedIndicator_ms = 0;
          indicator_output    = 0;

          #ifdef INDICATOR_DEBUG
          Serial.print("delay2: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
          #endif
          
          digitalWrite(PIN_INDICATOR,indicator_output);          
  			}
  			break;	
  		case DIGIT_2:
        //Serial.println("Digit2");   
  			if( digit_counter < dmx_base_address_digit_2 )
  			{
  				if( elapsedIndicator_ms > 500 )
  				{
  					indicator_output = !indicator_output;

            #ifdef INDICATOR_DEBUG
  					Serial.print("digit2: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
            #endif
           
  					digitalWrite(PIN_INDICATOR,indicator_output);
  					elapsedIndicator_ms = 0;
  					if( indicator_output == 0 )
  					{
  						digit_counter++;
  					}
  				}
  			}
  			else
  			{		
  				indicator_state 	= INDICATOR_STATE::DELAY_3;
  				elapsedIndicator_ms = 0;
  				digit_counter       = 0;
          indicator_output    = 0;
          digitalWrite(PIN_INDICATOR,indicator_output);
  			}
  			break;
  		case INDICATOR_STATE::DELAY_3:
        //Serial.println("Delay3");   
  			if( elapsedIndicator_ms > 1000 )
  			{
  				indicator_state 	  = INDICATOR_STATE::DIGIT_1;
  				elapsedIndicator_ms = 0;
          indicator_output    = 0;

          #ifdef INDICATOR_DEBUG
          Serial.print("delay3: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
          #endif
          
          digitalWrite(PIN_INDICATOR,indicator_output);          
  			}
  			break;	
  		case INDICATOR_STATE::DIGIT_1:
        //Serial.println("Digit1");   
  			if( digit_counter < dmx_base_address_digit_1 )
  			{
  				if( elapsedIndicator_ms > 500 )
  				{
  					indicator_output = !indicator_output;

            #ifdef INDICATOR_DEBUG
  					Serial.print("digit1: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
            #endif
           
  					digitalWrite(PIN_INDICATOR,indicator_output);

            elapsedIndicator_ms = 0;
  					if( indicator_output == 0 )
  					{
  						digit_counter++;
  					}
  				}
  			}
  			else
  			{		
  				indicator_state 	= INDICATOR_STATE::DELAY_4;
  				elapsedIndicator_ms = 0;
  				digit_counter       = 0;
          indicator_output    = 0;
          digitalWrite(PIN_INDICATOR,indicator_output);          
  			}
  			break;
      case INDICATOR_STATE::DELAY_4:
        //Serial.println("Delay3");   
        if( elapsedIndicator_ms > 2000 )
        {
          indicator_state     = INDICATOR_STATE::IDLE;
          elapsedIndicator_ms = 0;
          indicator_output    = 0;
          
          #ifdef INDICATOR_DEBUG
          Serial.print("delay4: "); Serial.print(totalElapsed_ms); Serial.print(" ind: ");  Serial.println(indicator_output);
          #endif
          
          digitalWrite(PIN_INDICATOR,indicator_output);          
        }
        break;          
  		case INDICATOR_STATE::IDLE:
        indicator_output = 1;
        digitalWrite(PIN_INDICATOR,indicator_output);
  			break;
  	}

    elapsedIndicator1_ms = 0;
  }
}

#endif
