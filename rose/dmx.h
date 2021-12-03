#include "setup.h"
#include "led_functions.h"

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
		
		// Inner LED Ring
		led_inner_ring_new_color = Adafruit_NeoPixel::Color(dmx_led1_r_value, dmx_led1_g_value, dmx_led1_b_value);
		// Outer LED Ring
		led_outer_ring_new_color = Adafruit_NeoPixel::Color(dmx_led2_r_value, dmx_led2_g_value, dmx_led2_b_value);

		led_state = dmx_control_value;

		//colorFill(led_inner_ring_neo,led_inner_ring_new_color);
		//colorFill(led_outer_ring_neo,led_outer_ring_new_color);
	}
}