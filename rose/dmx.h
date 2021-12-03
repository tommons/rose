#include "setup.h"

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
		
		const uint32_t dmx_led1_color = led_outer_ring.gamma32( led_outer_ring.Color(dmx_led1_r_value, dmx_led1_g_value, dmx_led1_b_value) );
		const uint32_t dmx_led2_color = led_outer_ring.gamma32( led_outer_ring.Color(dmx_led2_r_value, dmx_led2_g_value, dmx_led2_b_value) );
		
		Serial.println("DMX Update:");
		Serial.print("DMX Control: "); 	Serial.println(dmx_control_value);
		Serial.print("DMX LED1 R: "); 	Serial.println(dmx_led1_r_value);
		Serial.print("DMX LED1 G: "); 	Serial.println(dmx_led1_g_value);
		Serial.print("DMX LED1 B: "); 	Serial.println(dmx_led1_b_value);
		Serial.print("DMX LED2 R: "); 	Serial.println(dmx_led2_r_value);
		Serial.print("DMX LED2 G: "); 	Serial.println(dmx_led2_g_value);
		Serial.print("DMX LED2 B: "); 	Serial.println(dmx_led2_b_value);
		Serial.print("DMX Servo: "); 	Serial.println(dmx_servo_value);
	}
}