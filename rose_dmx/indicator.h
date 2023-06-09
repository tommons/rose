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

#ifndef INDICATOR_H
#define INDICATOR_H

#include <elapsedMillis.h>

//#include "dmx.h"

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
	ERROR_SERIAL,
	DMX_ACTIVITY,
	IDLE
};

uint8_t indicator_state   = 0;
uint8_t indicator_output  = 0;
uint8_t digit_counter     = 0;
uint8_t init_counter      = 0;

uint8_t dmx_base_address_digit_1     = 0;      
uint8_t dmx_base_address_digit_2    = 0;
uint8_t dmx_base_address_digit_3    = 0;

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

void indicatorDmxActivity()
{
	if( indicator_state == INDICATOR_STATE::IDLE )
	{
		indicator_state = DMX_ACTIVITY;
	}
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
        if( indicator_output != 1 )
        {
          indicator_output = 1;
          digitalWrite(PIN_INDICATOR,indicator_output);
        }
  			break;
      case INDICATOR_STATE::ERROR_SERIAL:
        if( elapsedIndicator_ms > 200 )
        {
          elapsedIndicator_ms = 0;
          indicator_output    = !indicator_output;
                    
          digitalWrite(PIN_INDICATOR,indicator_output);          
        }
        break;
      case INDICATOR_STATE::DMX_ACTIVITY:
        if( elapsedIndicator_ms > 50 )
        {
          elapsedIndicator_ms = 0;
          if( indicator_output == 1 )
          {
            indicator_output = 0;
            digitalWrite(PIN_INDICATOR,indicator_output);
          }
          else
          {
            indicator_state = INDICATOR_STATE::IDLE;
          }
        }
        break;
  	}

    elapsedIndicator1_ms = 0;
  }
}

#endif
