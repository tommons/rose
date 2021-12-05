#include <DMXSerial.h>
#include <Wire.h>
#include <elapsedMillis.h>

#include "servo_functions.h"

// DMX Interface info
// Channels are 1-based
#define DMX_START_CHANNEL 		1
#define DMX_I2C_ID 				4
#define DMX_MSGID 				2
#define DMX_CONTROL1_CHANNEL 	DMX_START_CHANNEL + 0
#define DMX_LED1_R_CHANNEL 		DMX_START_CHANNEL + 1
#define DMX_LED1_G_CHANNEL 		DMX_START_CHANNEL + 2
#define DMX_LED1_B_CHANNEL 		DMX_START_CHANNEL + 3
#define DMX_CONTROL2_CHANNEL 	DMX_START_CHANNEL + 4
#define DMX_LED2_R_CHANNEL 		DMX_START_CHANNEL + 5
#define DMX_LED2_G_CHANNEL 		DMX_START_CHANNEL + 6
#define DMX_LED2_B_CHANNEL 		DMX_START_CHANNEL + 7
#define DMX_SERVO_CHANNEL  		DMX_START_CHANNEL + 8

elapsedMillis dmx_elapsed_ms;
#define DMX_READ_ELAPSED_MS 50

void setupDMX()
{
	Wire.begin(); // join i2c bus (address optional for master)
	DMXSerial.init(DMXReceiver);
	dmx_elapsed_ms = 0;
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
			const uint8_t dmx_control1_value 	= DMXSerial.read(DMX_CONTROL1_CHANNEL);		
			const uint8_t dmx_led1_r_value 		= DMXSerial.read(DMX_LED1_R_CHANNEL);
			const uint8_t dmx_led1_g_value 		= DMXSerial.read(DMX_LED1_G_CHANNEL);
			const uint8_t dmx_led1_b_value 		= DMXSerial.read(DMX_LED1_B_CHANNEL);
			const uint8_t dmx_control2_value 	= DMXSerial.read(DMX_CONTROL2_CHANNEL);				
			const uint8_t dmx_led2_r_value 		= DMXSerial.read(DMX_LED2_R_CHANNEL);
			const uint8_t dmx_led2_g_value 		= DMXSerial.read(DMX_LED2_G_CHANNEL);
			const uint8_t dmx_led2_b_value 		= DMXSerial.read(DMX_LED2_B_CHANNEL);
			const uint8_t dmx_servo_value  		= DMXSerial.read(DMX_SERVO_CHANNEL);
			
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
		  
			// set servo state
			servoState = dmx_servo_value;
			
			// Print for debug
			#ifdef ROSE_DEBUG
			Serial.println(F("DMX Update:"));
			Serial.print(F("DMX Control1: ")); 		Serial.println(dmx_control1_value);
			Serial.print(F("DMX LED1 R: ")); 		Serial.println(dmx_led1_r_value);
			Serial.print(F("DMX LED1 G: ")); 		Serial.println(dmx_led1_g_value);
			Serial.print(F("DMX LED1 B: ")); 		Serial.println(dmx_led1_b_value);
			Serial.print(F("DMX Control2: ")); 		Serial.println(dmx_control2_value);			
			Serial.print(F("DMX LED2 R: ")); 		Serial.println(dmx_led2_r_value);
			Serial.print(F("DMX LED2 G: ")); 		Serial.println(dmx_led2_g_value);
			Serial.print(F("DMX LED2 B: ")); 		Serial.println(dmx_led2_b_value);
			Serial.print(F("DMX Servo: ")); 		Serial.println(dmx_servo_value);
			#endif
		}
	}
}