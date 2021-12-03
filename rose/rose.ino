#include "setup.h"
//#include "states.h"
#include "led_functions.h"
#include "servo_functions.h"
#include <elapsedMillis.h>
#include "dmx.h"

uint32_t ch6_cur, ch6_prev, ch6_filt;
uint32_t buttonState 							= LOW;

uint32_t inputEvent             = LOW;
uint32_t inputEventPrev         = LOW;
elapsedMillis inputEventElapsed_ms;

const uint32_t SWITCH_ON_HIGH_THRESHOLD    		= 1800;
const uint32_t SWITCH_OFF_LOW_THRESHOLD    		= 1100;
const uint32_t RC_CONNECTED_THRESHOLD      		= 900;
boolean drop_petal              				= false;
uint32_t drop_on_count               			= 0;
uint32_t drop_off_count              			= 0;
const uint32_t DROP_OFF_COUNT_THRESHOLD1_ms   	= 1000;
const uint32_t DROP_ON_COUNT_THRESHOLD1_ms    	= 500;
const uint32_t DROP_OFF_COUNT_THRESHOLD2_ms   	= 100;
// 0=waiting for very long off
// 1=waiting for long on
// 2=waiting for long off
uint32_t drop_state                  			= 0; 
elapsedMillis drop_off_elapsed_ms;
elapsedMillis drop_on_elapsed_ms;
elapsedMillis blink_ms;
elapsedMillis checkInput_ms;
int blinkState 									= LOW;

#define INPUT_EVENT_HISTORY_SIZE 10
unsigned long inputEventHistoryDuration_ms[INPUT_EVENT_HISTORY_SIZE];
uint8_t inputEventHistoryState[INPUT_EVENT_HISTORY_SIZE];
void shiftInputEventHistory()
{
  for( int i=INPUT_EVENT_HISTORY_SIZE;  i > 0; i-- )
  {
    inputEventHistoryDuration_ms[i] = inputEventHistoryDuration_ms[i-1];
    inputEventHistoryState[i]       = inputEventHistoryState[i-1];
  }
}
void printInputEventHistory()
{
  for( int i=1; i < INPUT_EVENT_HISTORY_SIZE; i++ )
  {
    Serial.print("inputEventHistory "); Serial.print(i); 
    Serial.print(" state: "); Serial.print(inputEventHistoryState[i]);
    Serial.print(" duration: "); Serial.println(inputEventHistoryDuration_ms[i]);
  }
}
uint32_t counter = 0;

void setup() {
  DMXSerial.init(DMXReceiver);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PIN_RC_CH6, INPUT);
	pinMode(PIN_SWITCH, INPUT_PULLUP);

    setupServos();
	
	led_outer_ring_neo.setBrightness(LED_MAX_BRIGHTNESS); // Set BRIGHTNESS (max = 255)
  led_outer_ring_neo.clear();
  led_outer_ring_neo.show();
	led_inner_ring_neo.setBrightness(LED_MAX_BRIGHTNESS); // Set BRIGHTNESS (max = 255)
  led_inner_ring_neo.clear();
  led_inner_ring_neo.show();
  
  #define BUTTON_HISTORY_SIZE 10
  for( int i=0; i < BUTTON_HISTORY_SIZE; i++ )
  {
    inputEventHistoryDuration_ms[i] = 0;
    inputEventHistoryState[i]       = 0;
  }

	//state_reset();

	Serial.begin(9600); // Pour a bowl of Serial
}

boolean inputTransitionReaction = false;

uint32_t zeroCount = 0;

void loop() {

    handleDMX();

    led_state_machine(led_state);
    //servo_state_machine();
    
    /*
	if( checkInput_ms > 50 )
	{
		// read RC states
		ch6_cur 		    = pulseIn(PIN_RC_CH6, HIGH, 25000); // CH6 SWB
		buttonState 	  = digitalRead(PIN_SWITCH); // Manual button
    inputEventPrev  = inputEvent;

		// filter the PWM inputs
		//ch6_filt = (ch6_cur + ch6_prev) >> 1; // 2point average
		//ch6_filt = (ch6_cur * 3 >> 2) + (ch6_filt >> 2); // alpha filter 3/4, 1/4
    
    if( ch6_cur == 0 )
      zeroCount++;
    else
      zeroCount = 0;  
      
    if( zeroCount > 20 || ch6_cur > 0 )
		  ch6_filt    = (ch6_cur >> 1) + (ch6_filt >> 1); // alpha filter 1/2, 1/2
    
		if( counter % 10 == 0 )
		{
			Serial.print("Ch6: "); Serial.print(ch6_cur);
			Serial.print(" Ch6filt: "); Serial.print(ch6_filt);
			Serial.print(" buttonState: "); Serial.print(buttonState);
			Serial.println("");
		}
		
		ch6_prev = ch6_cur;

		// Petal drop state machine
		// require low for 1 sec
		// then high for 1/2 sec
		// then low for 1/2 sec
   
		if( ch6_filt > SWITCH_ON_HIGH_THRESHOLD || buttonState == LOW )
		{
      inputEvent = HIGH;
		}
    else if( ( ch6_filt > RC_CONNECTED_THRESHOLD && ch6_filt < SWITCH_OFF_LOW_THRESHOLD ) ||
              buttonState == HIGH )
    {
      inputEvent = LOW;
    }

    boolean inputTransition = false;
    // input transitioned
    if( inputEvent != inputEventPrev )
    {
      Serial.print("Input Transition.  Was: "); Serial.print(inputEventPrev); 
      Serial.print(" for ms: "); Serial.print(inputEventElapsed_ms); 
      Serial.print(" is now: "); Serial.println(inputEvent);
      inputEventElapsed_ms = 0;
      shiftInputEventHistory();
      //printInputEventHistory();
      inputTransition = true;
      inputTransitionReaction = false;
    }

    // track the current input state in the history
    inputEventHistoryDuration_ms[0] = inputEventElapsed_ms;
    inputEventHistoryState[0]       = inputEvent;


    // Test for state change
    if( inputTransitionReaction == false &&
        inputEventHistoryState[0]       == LOW &&
        inputEventHistoryDuration_ms[0] > DROP_OFF_COUNT_THRESHOLD2_ms &&
        inputEventHistoryState[1]       == HIGH &&
        inputEventHistoryDuration_ms[1] > DROP_ON_COUNT_THRESHOLD1_ms &&
        inputEventHistoryState[2]       == LOW &&
        inputEventHistoryDuration_ms[2] > DROP_OFF_COUNT_THRESHOLD1_ms )
      {
          inputTransitionReaction = true;
          incrementState();
          
          Serial.print("INCREMENT STATE!!! State is now: ");
          Serial.println(state);
      }
      else if(  inputTransitionReaction == false &&
                inputEventHistoryState[0]       == LOW && // any off
                inputEventHistoryDuration_ms[0] > 100 &&
                
                inputEventHistoryState[1]       == HIGH &&
                inputEventHistoryDuration_ms[1] > 100 && inputEventHistoryDuration_ms[1] < 1000 && // short on
                
                inputEventHistoryState[2]       == LOW && // any off
                inputEventHistoryDuration_ms[2] > 100 &&
                
                inputEventHistoryState[3]       == HIGH &&
                inputEventHistoryDuration_ms[3] > 1000 && // long on
                
                inputEventHistoryState[4]       == LOW &&  // any off
                inputEventHistoryDuration_ms[4] > 100 &&
                
                inputEventHistoryState[5]       == HIGH &&
                inputEventHistoryDuration_ms[5] > 100 && inputEventHistoryDuration_ms[5] < 1000 ) // short on
      {
        Serial.print("SERVO TENSION MODE!!!");
          inputTransitionReaction = true;
        //clearLeds();
        for( uint8_t servoNumber=0; servoNumber < NUM_SERVOS; servoNumber++ )
          setServo(servoNumber, SERVO_PETAL_DROP_ANGLE );
      }
    	
		checkInput_ms = 0;
		counter = counter + 1;

	}

	stateMachine(state);

  */
	// Blink the built-in LED
	if( blink_ms > 1000 )
	{
		blink_ms = 0;
		if( blinkState == HIGH )
		{
			blinkState = LOW;
		}
		else
		{
			blinkState = HIGH;
		}
		digitalWrite(LED_BUILTIN, blinkState);
	}
		
	delay(DELAY_MS);
}
