#ifndef STATES_H
#define STATES_H

#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <elapsedMillis.h>

#include "led_functions.h"
#include "setup.h"
#include "servo_functions.h"

uint32_t state   = 0; // state machine value for sequencing instructions

#define MAX_NUM_STATES 20
#define NUM_STATES 5
uint32_t state_counter[MAX_NUM_STATES] = {0};

void resetStateCounters()
{
	for( uint32_t i=0; i < MAX_NUM_STATES; i++ )
		state_counter[i] = 0;
}

void incrementState()
{	
	if( state > 0 )
		state_counter[state-1] = 0;
	state++;
	
	if( state == NUM_STATES )
	{
		state = 0;
	}
				
	state_counter[state] = 0;
}

void state_reset()
{
	for( uint8_t servoIdx=0; servoIdx < NUM_SERVOS; servoIdx++ )
		servoHoldPetal( servoIdx, 0 );

	state = 0;

	resetStateCounters();

	colorFill( led_outer_ring, led_outer_ring.Color(0, 0, 0) );
	colorFill( led_inner_ring, led_inner_ring.Color(0, 0, 0) );          
}

void state0()
{
	if( state_counter[0] == 0 )
		state_reset();
  
	state_counter[0]++;
}

void state1()
{
	const uint8_t stateIdx = 1;
	
	fadeInRed( state_counter[stateIdx] );
  
  servoDropPetal(SERVO_PETAL_1, state_counter[stateIdx]);
    
	state_counter[stateIdx]++;
}

void state2()
{
	const uint8_t stateIdx = 2;
	
	fadeOutPurple( state_counter[stateIdx] );

  servoDropPetal(SERVO_PETAL_2, state_counter[stateIdx]);
    
	state_counter[stateIdx]++;
}

void state3()
{
	const uint8_t stateIdx = 3;
	
	chaseBackward(state_counter[stateIdx]);
 
  servoDropPetal(SERVO_PETAL_3, state_counter[stateIdx]);
    
	state_counter[stateIdx]++;
}

void state4()
{
	const uint8_t stateIdx = 4;
	
	wipeRed(state_counter[stateIdx], 40, true); // forward
  
  servoDropPetal(SERVO_PETAL_4, state_counter[stateIdx]);
    			
	state_counter[stateIdx]++;
}

void state5()
{
	const uint8_t stateIdx = 5;
	
	wipeRed(state_counter[stateIdx], 20, false); // backward
			
	state_counter[stateIdx]++;
}

void state6()
{
	const uint8_t stateIdx = 6;

	clearLeds();
	
	if( state_counter[stateIdx] == 0 )
		servoDropPetal(SERVO_PETAL_1, state_counter[stateIdx]);
  
	state_counter[stateIdx]++;
}

void state7()
{
	const uint8_t stateIdx = 7;

  	if( state_counter[stateIdx] == 0 )
		servoDropPetal(SERVO_PETAL_2, state_counter[stateIdx]);
	
	state_counter[stateIdx]++;
}

void state8()
{
	const uint8_t stateIdx = 8;

  	if( state_counter[stateIdx] == 0 )
		servoDropPetal(SERVO_PETAL_3, state_counter[stateIdx]);
	
	state_counter[stateIdx]++;
}

void state9()
{
	const uint8_t stateIdx = 9;

  	if( state_counter[stateIdx] == 0 )
		servoDropPetal(SERVO_PETAL_4, state_counter[stateIdx]);
	
	state_counter[stateIdx]++;
}

void state10()
{
	const uint8_t stateIdx = 10;
  
	state_counter[stateIdx]++;
}

void state11()
{
	const uint8_t stateIdx = 11;
  
	state_counter[stateIdx]++;
}

void state12()
{
	const uint8_t stateIdx = 12;
  
	state_counter[stateIdx]++;
}

void state13()
{
	const uint8_t stateIdx = 13;
  
	state_counter[stateIdx]++;
}

void state14()
{
	const uint8_t stateIdx = 14;

	state_counter[stateIdx]++;
}

void state15()
{
	const uint8_t stateIdx = 15;

	state_counter[stateIdx]++;
}

void state16()
{
	const uint8_t stateIdx = 16;

	state_counter[stateIdx]++;
}

void state17()
{
	const uint8_t stateIdx = 17;

	state_counter[stateIdx]++;
}

void state18()
{
	const uint8_t stateIdx = 18;

	state_counter[stateIdx]++;
}

void state19()
{
	const uint8_t stateIdx = 19;

	state_counter[stateIdx]++;
}

void stateMachine(const uint32_t state)
{
	switch( state )
	{
		case 0:
			state0();
			break;

		case 1:
			state1();
			break;

		case 2:
			state2();
			break;

		case 3:
			state3();
			break;

		case 4:
			state4();
			break;
				
		case 5:
			state5();
			break;

		case 6:
			state6();
			break;

		case 7:
			state7();
			break;
		  
		case 8:
			state8();
			break;

		case 9:
			state9();
			break;

		case 10:
			state10();
			break;

		case 11:
			state11();
			break;

		case 12:
			state12();
			break;

		case 13:
			state13();
			break;

		case 14:
			state14();
			break;
				
		case 15:
			state15();
			break;

		case 16:
			state16();
			break;

		case 17:
			state17();
			break;
		  
		case 18:
			state18();
			break;

		case 19:
			state19();
			break;	
			
		default:
			state_reset();
			break;
	}
}

#endif
