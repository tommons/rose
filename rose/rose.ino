#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "states.h"
#include "led_functions.h"

uint32_t ch6_cur, ch6_prev, ch6_filt;
uint32_t buttonState 						= LOW;

const uint32_t SWITCH_ON_HIGH_THRESHOLD    	= 1800;
const uint32_t SWITCH_OFF_LOW_THRESHOLD    	= 1100;
const uint32_t RC_CONNECTED_THRESHOLD      	= 900;
boolean drop_petal              			= false;
uint32_t drop_on_count               		= 0;
uint32_t drop_off_count              		= 0;
const uint32_t DROP_OFF_COUNT_THRESHOLD1   	= 1000 / DELAY_MS; // 1 sec
const uint32_t DROP_ON_COUNT_THRESHOLD1    	= 500 / DELAY_MS; // .5 sec
const uint32_t DROP_OFF_COUNT_THRESHOLD2   	= 500 / DELAY_MS; // .5 sec
// 0=waiting for very long off
// 1=waiting for long on
// 2=waiting for long off
uint32_t drop_state                  = 0; 

uint32_t counter = 0;


void setup() {

  pinMode(PIN_RC_CH6, INPUT);
  pinMode(PIN_SWITCH, INPUT);

  led_outer_ring.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  led_outer_ring.show();            // Turn OFF all pixels ASAP
  led_outer_ring.setBrightness(127); // Set BRIGHTNESS (max = 255)
  
  led_inner_ring.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  led_inner_ring.show();            // Turn OFF all pixels ASAP
  led_inner_ring.setBrightness(127); // Set BRIGHTNESS (max = 255)
  
  servo1.attach(PIN_SERVO1,SERVO_PW_MIN,SERVO_PW_MAX);
  servo2.attach(PIN_SERVO2,SERVO_PW_MIN,SERVO_PW_MAX);
  servo3.attach(PIN_SERVO3,SERVO_PW_MIN,SERVO_PW_MAX);
  servo4.attach(PIN_SERVO4,SERVO_PW_MIN,SERVO_PW_MAX);

  state_reset();
  
  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

	// read RC states
	ch6_cur 		= pulseIn(PIN_RC_CH6, HIGH, 25000); // CH6 SWB
	buttonState 	= digitalRead(PIN_SWITCH); // Manual button

	// filter the PWM inputs
	//ch6_filt = (ch6_cur + ch6_prev) >> 1; // 2point average
	ch6_filt = (ch6_cur * 3 >> 2) + (ch6_filt >> 2); // alpha filter 3/4, 1/4

	if( false && counter % 10 == 0 )
	{
		Serial.print("Ch6: "); Serial.print(ch6_cur);
		Serial.print(" Ch6filt: "); Serial.print(ch6_filt);
		Serial.println("");
	}

	ch6_prev = ch6_cur;

	// Petal drop state machine
	// require low for 1 sec
	// then high for 1/2 sec
	// then low for 1/2 sec
	if( ch6_filt > SWITCH_ON_HIGH_THRESHOLD || buttonState == HIGH )
	{
		drop_on_count   = drop_on_count + 1;
		drop_off_count  = 0;

		if( drop_state == 1 )
		{
			if( drop_on_count >= DROP_ON_COUNT_THRESHOLD1 )
			{
				drop_state = 2; // waiting for long off
			}
		}
	}
	else if( ( ch6_filt > RC_CONNECTED_THRESHOLD && ch6_filt < SWITCH_OFF_LOW_THRESHOLD ) ||
			 buttonState == LOW )
	{
		drop_on_count   = 0;
		drop_off_count  = drop_off_count + 1;

		if( drop_state == 0 ) // waiting for very long off
		{
			if( drop_off_count >= DROP_OFF_COUNT_THRESHOLD1 )
			{
			  drop_state = 1; // waiting for long on
			}
		}
		else if( drop_state == 1 ) // waiting for long on
		{
			// NO-OP
		}
		else if( drop_state == 2 ) // waiting for long off
		{
			if( drop_off_count >= DROP_OFF_COUNT_THRESHOLD2 )
			{

				// reset counters and state machine
				drop_state      = 0;
				drop_off_count  = 0;

				incrementState();
				
				Serial.print("DROP PETAL!!! State is now: ");
				Serial.println(state);
			}
		}
	}

	stateMachine(state);
  
	counter = counter + 1;

	delay(DELAY_MS);
}
