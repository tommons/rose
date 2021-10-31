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
#define NUM_STATES 7
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
	servo1.write(SERVO_INITIAL_ANGLE);
	servo2.write(SERVO_INITIAL_ANGLE);
	servo3.write(SERVO_INITIAL_ANGLE);
	servo4.write(SERVO_INITIAL_ANGLE);

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

	state_counter[stateIdx]++;
}

void state2()
{
	const uint8_t stateIdx = 2;
	
	fadeOutPurple( state_counter[stateIdx] );
  
	state_counter[stateIdx]++;
}

void state3()
{
	const uint8_t stateIdx = 3;
	
	chaseBackward(state_counter[stateIdx]);
			
	state_counter[stateIdx]++;
}

void state4()
{
	const uint8_t stateIdx = 4;
	
	wipeRed(state_counter[stateIdx], true); // forward
				
	state_counter[stateIdx]++;
}

void state5()
{
	const uint8_t stateIdx = 5;
	
	wipeRed(state_counter[stateIdx], false); // backward
			
	state_counter[stateIdx]++;
}

void state6()
{
	const uint8_t stateIdx = 6;
	
	if( state_counter[stateIdx] == 0 )
		servo4.write( servoMap( SERVO_MAX_ANGLE ) );
  
	state_counter[stateIdx]++;
}

void state7()
{
  
  state_counter[7]++;
}

void state8()
{
  
  state_counter[8]++;
}

void state9()
{
  
  state_counter[9]++;
}

void state10()
{
  
  state_counter[610]++;
}

void state11()
{
  
  state_counter[11]++;
}

void state12()
{
  
  state_counter[12]++;
}

void state13()
{
  
  state_counter[13]++;
}

void state14()
{
  
  state_counter[14]++;
}

void state15()
{
  
  state_counter[15]++;
}

void state16()
{
  
  state_counter[16]++;
}

void state17()
{
  
  state_counter[17]++;
}

void state18()
{
  
  state_counter[18]++;
}

void state19()
{
  
  state_counter[19]++;
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