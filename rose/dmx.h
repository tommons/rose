#include "setup.h"
#include "led_functions.h"

#define DMX_HISTORY_DEPTH 4
uint8_t dmx_control_history[DMX_HISTORY_DEPTH];
uint8_t dmx_history_counter = 0;

void handleDMX()
{
	// check for new dmx instruction
	if( DMXSerial.dataUpdated() ) 
	{
		DMXSerial.resetUpdated();
		
		const uint8_t dmx_control_value = DMXSerial.read(DMX_CONTROL_CHANNEL);
		const uint8_t dmx_led1_r_value 	= DMXSerial.read(DMX_LED1_R_CHANNEL);
		const uint8_t dmx_led1_g_value 	= DMXSerial.read(DMX_LED1_G_CHANNEL);
		const uint8_t dmx_led1_b_value 	= DMXSerial.read(DMX_LED1_B_CHANNEL);
		const uint8_t dmx_led2_r_value 	= DMXSerial.read(DMX_LED2_R_CHANNEL);
		const uint8_t dmx_led2_g_value 	= DMXSerial.read(DMX_LED2_G_CHANNEL);
		const uint8_t dmx_led2_b_value 	= DMXSerial.read(DMX_LED2_B_CHANNEL);
		const uint8_t dmx_servo_value  	= DMXSerial.read(DMX_SERVO_CHANNEL);
		
		const uint32_t dmx_led1_color = Adafruit_NeoPixel::gamma32( Adafruit_NeoPixel::Color(dmx_led1_r_value, dmx_led1_g_value, dmx_led1_b_value) );
		const uint32_t dmx_led2_color = Adafruit_NeoPixel::gamma32( Adafruit_NeoPixel::Color(dmx_led2_r_value, dmx_led2_g_value, dmx_led2_b_value) );
		
		//colorFill( led_inner_ring, dmx_led1_color );
		//colorFill( led_outer_ring, dmx_led2_color );
		
		// to avoid glitches, look for a constent control value before we publish it
		//dmx_control_history[dmx_history_counter] = dmx_control_value;
		bool control_value_ok = true;
		// for( uint8_t i=0; i < DMX_HISTORY_DEPTH; i++ )
		// {
			// if( dmx_control_history[i] != dmx_control_history[0] )
				// control_value_ok = false;
		// }
		
		if( control_value_ok )
		{
			led_state 					= dmx_control_value;
			led_inner_ring_new_color 	= dmx_led1_color;
			led_outer_ring_new_color 	= dmx_led2_color;
		}	
		
		Serial.println(F("DMX Update:"));
		Serial.print(F("DMX Control: ")); 	Serial.println(dmx_control_value);
		//Serial.print("DMX LED1 R: "); 	Serial.println(dmx_led1_r_value);
		//Serial.print("DMX LED1 G: "); 	Serial.println(dmx_led1_g_value);
		//Serial.print("DMX LED1 B: "); 	Serial.println(dmx_led1_b_value);
		Serial.print(F("DMX LED2 R: ")); 	Serial.println(dmx_led2_r_value);
		Serial.print(F("DMX LED2 G: ")); 	Serial.println(dmx_led2_g_value);
		Serial.print(F("DMX LED2 B: ")); 	Serial.println(dmx_led2_b_value);
		//Serial.print("DMX Servo: "); 	Serial.println(dmx_servo_value);
		
		dmx_history_counter++;
	}
}