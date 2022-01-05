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

#ifndef DMX_H
#define DMX_H

#define DMX_DEBUG
#undef DMX_DEBUG // uncomment to enable DMX debug

#include <DMXSerial.h>
#include <Wire.h>
#include <elapsedMillis.h>

#include "servo_functions.h"
#include "indicator.h"

#define DMX_DIP_1_PIN 6
#define DMX_DIP_2_PIN 7
#define DMX_DIP_3_PIN 8
#define DMX_DIP_4_PIN 9
#define DMX_DIP_5_PIN 10
#define DMX_DIP_6_PIN 11
#define DMX_DIP_7_PIN 12

// DMX Interface info
// Channels are 1-based
#define DMX_START_CHANNEL 		1
#define DMX_I2C_ID 				4
#define DMX_MSGID 				2
#define DMX_CONTROL1_CHANNEL_OFFSET 	0
#define DMX_LED1_R_CHANNEL_OFFSET 		1
#define DMX_LED1_G_CHANNEL_OFFSET 		2
#define DMX_LED1_B_CHANNEL_OFFSET 		3
#define DMX_CONTROL2_CHANNEL_OFFSET 	4
#define DMX_LED2_R_CHANNEL_OFFSET 		5
#define DMX_LED2_G_CHANNEL_OFFSET 		6
#define DMX_LED2_B_CHANNEL_OFFSET 		7
#define DMX_SERVO_CHANNEL_OFFSET 		  8

elapsedMillis dmx_elapsed_ms;
elapsedMillis dmx_debug_elapsed_ms;

#define DMX_READ_ELAPSED_MS 50

uint8_t dmx_control1_value 	= 0;
uint8_t dmx_led1_r_value 	= 0;
uint8_t dmx_led1_g_value 	= 0;
uint8_t dmx_led1_b_value 	= 0;
uint8_t dmx_control2_value 	= 0;
uint8_t dmx_led2_r_value 	= 0;
uint8_t dmx_led2_g_value 	= 0;
uint8_t dmx_led2_b_value 	= 0;
uint8_t dmx_servo_value  	= 0;

uint16_t dmx_base_address       	  = 0;

uint16_t dmx_control1_channel_num    = 0;
uint16_t dmx_led1_r_channel_num    	= 0;
uint16_t dmx_led1_g_channel_num    	= 0;
uint16_t dmx_led1_b_channel_num    	= 0;
uint16_t dmx_control2_channel_num    = 0;
uint16_t dmx_led2_r_channel_num    	= 0;
uint16_t dmx_led2_g_channel_num    	= 0;
uint16_t dmx_led2_b_channel_num    	= 0;
uint16_t dmx_servo_channel_num    	= 0;

uint32_t dmx_debug_counter = 0;

bool allow_dmxI2C 					= false;
bool suspend_dmxI2C 				= false;

void sendDMXI2C()
{
#ifdef ROSE_DEBUG
	Serial.println(F("call sendDMXI2C"));
#endif
	
	if( allow_dmxI2C )
	{
		// Print for debug
		#ifdef ROSE_DEBUG
		Serial.println(F("DMX Update:"));
		Serial.print(F("DMX Control1: "));    Serial.println(dmx_control1_value);
		Serial.print(F("DMX LED1 R: "));    Serial.println(dmx_led1_r_value);
		Serial.print(F("DMX LED1 G: "));    Serial.println(dmx_led1_g_value);
		Serial.print(F("DMX LED1 B: "));    Serial.println(dmx_led1_b_value);
		Serial.print(F("DMX Control2: "));    Serial.println(dmx_control2_value);     
		Serial.print(F("DMX LED2 R: "));    Serial.println(dmx_led2_r_value);
		Serial.print(F("DMX LED2 G: "));    Serial.println(dmx_led2_g_value);
		Serial.print(F("DMX LED2 B: "));    Serial.println(dmx_led2_b_value);
		Serial.print(F("DMX Servo: "));     Serial.println(dmx_servo_value);
		#endif
		
		// Send the DMX values to the slave arduino
		Wire.beginTransmission(DMX_I2C_ID); // begin transmitting
		Wire.write(DMX_MSGID);
		Wire.write(dmx_control1_value);
		Wire.write(dmx_led1_r_value);
		Wire.write(dmx_led1_g_value);
		Wire.write(dmx_led1_b_value);
		Wire.write(dmx_control2_value);		
		Wire.write(dmx_led2_r_value);
		Wire.write(dmx_led2_g_value);
		Wire.write(dmx_led2_b_value);
		Wire.write(dmx_servo_value); 
		Wire.endTransmission();    // stop transmitting

		// servo state happens in this arduino
		// set servo state
		servoState = dmx_servo_value;      
	}
}

void setupDMX()
{
#ifdef ROSE_DEBUG
	Serial.println("start setupDMX");
#endif

	pinMode(DMX_DIP_1_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_2_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_3_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_4_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_5_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_6_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_7_PIN, INPUT_PULLUP);
	
	const uint8_t dip_1 = !digitalRead(DMX_DIP_1_PIN);
	const uint8_t dip_2 = !digitalRead(DMX_DIP_2_PIN);
	const uint8_t dip_3 = !digitalRead(DMX_DIP_3_PIN);
	const uint8_t dip_4 = !digitalRead(DMX_DIP_4_PIN);
	const uint8_t dip_5 = !digitalRead(DMX_DIP_5_PIN);
	const uint8_t dip_6 = !digitalRead(DMX_DIP_6_PIN);
  const uint8_t dip_7 = !digitalRead(DMX_DIP_7_PIN);

	// build decimal value from dip switch bits
  // MSB is DIP1, LSB is DIP5
	const uint16_t value = (dip_1 << 4) + (dip_2 << 3) + (dip_3 << 2) + (dip_4 << 1) + (dip_5 << 0);
	dmx_base_address = (value * 16) + 1;

	// compute dmx channel addresses from base
	dmx_control1_channel_num    = dmx_base_address + DMX_CONTROL1_CHANNEL_OFFSET;
	dmx_led1_r_channel_num    	= dmx_base_address + DMX_LED1_R_CHANNEL_OFFSET;
	dmx_led1_g_channel_num    	= dmx_base_address + DMX_LED1_G_CHANNEL_OFFSET;
	dmx_led1_b_channel_num    	= dmx_base_address + DMX_LED1_B_CHANNEL_OFFSET;
	dmx_control2_channel_num    = dmx_base_address + DMX_CONTROL2_CHANNEL_OFFSET;
	dmx_led2_r_channel_num    	= dmx_base_address + DMX_LED2_R_CHANNEL_OFFSET;
	dmx_led2_g_channel_num    	= dmx_base_address + DMX_LED2_G_CHANNEL_OFFSET;
	dmx_led2_b_channel_num    	= dmx_base_address + DMX_LED2_B_CHANNEL_OFFSET;
	dmx_servo_channel_num    	  = dmx_base_address + DMX_SERVO_CHANNEL_OFFSET;

	// compute the value of each digit
	uint16_t value_tmp  			= dmx_base_address;
	dmx_base_address_digit_3 	= value_tmp / 100;
	value_tmp          			= value_tmp - dmx_base_address_digit_3*100;
	dmx_base_address_digit_2 	= value_tmp / 10;
	value_tmp          			= value_tmp - dmx_base_address_digit_2*10;
	dmx_base_address_digit_1 	= value_tmp;

#ifdef ROSE_DEBUG
	Serial.print("dmx dip1: "); Serial.println(dip_1);
  Serial.print("dmx dip2: "); Serial.println(dip_2);
  Serial.print("dmx dip3: "); Serial.println(dip_3);
  Serial.print("dmx dip4: "); Serial.println(dip_4);
  Serial.print("dmx dip5: "); Serial.println(dip_5);
  Serial.print("value: "); Serial.println(value);
	Serial.print("digits: "); Serial.print(dmx_base_address_digit_3);
	Serial.print(" "); Serial.print(dmx_base_address_digit_2);
	Serial.print(" "); Serial.println(dmx_base_address_digit_1);
#endif

	Wire.begin(); // join i2c bus (address optional for master)
	
	// initialize the dmx receiver
	DMXSerial.init(DMXReceiver);
	
	dmx_elapsed_ms = 0;

	allow_dmxI2C = true;

#ifdef ROSE_DEBUG
	Serial.println("done setupDMX");
#endif 
}

void handleDMX()
{
	// Only process updates so often
	if( dmx_elapsed_ms > DMX_READ_ELAPSED_MS )
	{
    #if defined(ROSE_DEBUG) && defined(DMX_DEBUG)
    // dump the DMX bus data periodically
    if( dmx_debug_elapsed_ms > 1000 )
    {
      dmx_debug_counter++;
      Serial.println(dmx_debug_counter);
      for( uint16_t i=1; i<=512; i+=16 )
      {
        // print channel number
        if( i < 100 )
          Serial.print("0");
        if( i < 10 )
          Serial.print("0");
        Serial.print(i); Serial.print(" ");

        // print next 16 addresses
        for( uint8_t j=0; j < 16; j++ )
        {
          uint16_t ch = i+j;
          uint8_t val = DMXSerial.read(ch);
          if( val < 100 )
            Serial.print("0");
          if( val < 10 )
            Serial.print("0");
          Serial.print(val); 
          Serial.print(" "); 

          // break the words into groups of 8
          if( j==7 )
            Serial.print("  ");
        }
        Serial.println("");
      }
   
      dmx_debug_elapsed_ms = 0;
    }
    #endif 
    
		// check for new dmx instruction
		if( DMXSerial.dataUpdated() ) 
		{
			DMXSerial.resetUpdated();

      // indicate bus activity
      indicatorDmxActivity();
      
			dmx_elapsed_ms = 0;
			
			// Read our values of interest from the DMX bus
			dmx_control1_value 		= DMXSerial.read(dmx_control1_channel_num);		
			dmx_led1_r_value 		= DMXSerial.read(dmx_led1_r_channel_num);
			dmx_led1_g_value 		= DMXSerial.read(dmx_led1_g_channel_num);
			dmx_led1_b_value 		= DMXSerial.read(dmx_led1_b_channel_num);
			dmx_control2_value 		= DMXSerial.read(dmx_control2_channel_num);				
			dmx_led2_r_value 		= DMXSerial.read(dmx_led2_r_channel_num);
			dmx_led2_g_value 		= DMXSerial.read(dmx_led2_g_channel_num);
			dmx_led2_b_value 		= DMXSerial.read(dmx_led2_b_channel_num);
			dmx_servo_value  		= DMXSerial.read(dmx_servo_channel_num);


			#ifdef ROSE_DEBUG
			Serial.println(F("Got DMX Update:"));
			#endif
			
			// send the dmx data over i2c
      if( !suspend_dmxI2C )
			{
			  sendDMXI2C();
			}
			
		}
	}
}

#endif
