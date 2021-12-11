#ifndef DMX_H
#define DMX_H

#include <DMXSerial.h>
#include <Wire.h>
#include <elapsedMillis.h>

#include "servo_functions.h"

#define DMX_DIP_0_PIN 12
#define DMX_DIP_1_PIN 11
#define DMX_DIP_2_PIN 10
#define DMX_DIP_3_PIN 9
#define DMX_DIP_4_PIN 8
#define DMX_DIP_5_PIN 7
#define DMX_DIP_6_PIN 6

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

uint8_t dmx_base_address       		  = 0;
uint8_t dmx_base_address_digit_1 	  = 0;			
uint8_t dmx_base_address_digit_2 	  = 0;
uint8_t dmx_base_address_digit_3 	  = 0;

uint8_t dmx_control1_channel_num    = 0;
uint8_t dmx_led1_r_channel_num    	= 0;
uint8_t dmx_led1_g_channel_num    	= 0;
uint8_t dmx_led1_b_channel_num    	= 0;
uint8_t dmx_control2_channel_num    = 0;
uint8_t dmx_led2_r_channel_num    	= 0;
uint8_t dmx_led2_g_channel_num    	= 0;
uint8_t dmx_led2_b_channel_num    	= 0;
uint8_t dmx_servo_channel_num    	  = 0;

bool allow_dmxI2C = false;

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
  
	pinMode(DMX_DIP_0_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_1_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_2_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_3_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_4_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_5_PIN, INPUT_PULLUP);
	pinMode(DMX_DIP_6_PIN, INPUT_PULLUP);
	
	const uint8_t dip_0 = !digitalRead(DMX_DIP_0_PIN);
	const uint8_t dip_1 = !digitalRead(DMX_DIP_1_PIN);
	const uint8_t dip_2 = !digitalRead(DMX_DIP_2_PIN);
	const uint8_t dip_3 = !digitalRead(DMX_DIP_3_PIN);
	const uint8_t dip_4 = !digitalRead(DMX_DIP_4_PIN);
	const uint8_t dip_5 = !digitalRead(DMX_DIP_5_PIN);
	const uint8_t dip_6 = !digitalRead(DMX_DIP_6_PIN);

	// build decimal value from dip switch bits
	const uint8_t value = dip_0 + (dip_1 << 1) + (dip_2 << 2) + (dip_3 << 3) + (dip_4 << 4);
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
	uint8_t value_tmp  			= dmx_base_address;
	dmx_base_address_digit_3 	= value_tmp / 100;
	value_tmp          			= value_tmp - dmx_base_address_digit_3*100;
	dmx_base_address_digit_2 	= value_tmp / 10;
	value_tmp          			= value_tmp - dmx_base_address_digit_2*10;
	dmx_base_address_digit_1 	= value_tmp;

  #ifdef ROSE_DEBUG
  Serial.print("dmx address: "); Serial.println(dmx_base_address);
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
		// check for new dmx instruction
		if( DMXSerial.dataUpdated() ) 
		{
			DMXSerial.resetUpdated();
		
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
      sendDMXI2C();
      
		}
	}
}

#endif
