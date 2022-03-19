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

#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include <Adafruit_PWMServoDriver.h>
#include <elapsedMillis.h>

#define SERVO_MAX_ANGLE 		120
#define SERVO_MIN_ANGLE 		0
#define SERVO_INITIAL_ANGLE 	SERVO_MIN_ANGLE
#define SERVO_PW_MIN_US   		878 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_PW_MAX_US   		1900 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_FREQ_HZ     		50 // Analog servos run at ~50/20 Hz updates
#define SERVO_OSC_FREQ_HZ 		27000000

#define SERVO_PETAL_DROP_ANGLE_DELTA 	40  // how far the servo travels to pull the magnet
#define SERVO_PETAL_HOLD_ANGLE_CCW  	120
#define SERVO_PETAL_HOLD_ANGLE_CW  		50

#define NUM_SERVOS 4
// Setup servo hold and drop angles based on whether it rotates CW or CCW
const uint8_t SERVO_PETAL_HOLD_ANGLES[NUM_SERVOS] = { SERVO_PETAL_HOLD_ANGLE_CW, 
													  SERVO_PETAL_HOLD_ANGLE_CW, 
												   	  SERVO_PETAL_HOLD_ANGLE_CCW, 
													  SERVO_PETAL_HOLD_ANGLE_CCW };
const uint8_t SERVO_PETAL_DROP_ANGLES[NUM_SERVOS] = { SERVO_PETAL_HOLD_ANGLE_CW  + SERVO_PETAL_DROP_ANGLE_DELTA, 
													  SERVO_PETAL_HOLD_ANGLE_CW  + SERVO_PETAL_DROP_ANGLE_DELTA, 
													  SERVO_PETAL_HOLD_ANGLE_CCW - SERVO_PETAL_DROP_ANGLE_DELTA, 
													  SERVO_PETAL_HOLD_ANGLE_CCW - SERVO_PETAL_DROP_ANGLE_DELTA };

// Mapping of petal number to servo number
#define SERVO_PETAL_1 0
#define SERVO_PETAL_2 1
#define SERVO_PETAL_3 2
#define SERVO_PETAL_4 3

// DMX values to trigger a petal servo to drop
#define SERVO_PETAL_1_DMX 32
#define SERVO_PETAL_2_DMX 64
#define SERVO_PETAL_3_DMX 96
#define SERVO_PETAL_4_DMX 128

const uint32_t SERVO_PERIOD_US  = 1e6*1/SERVO_FREQ_HZ; // Analog servos run at ~50/20 Hz updates
const uint16_t SERVO_TICK_MIN   = 4096 * (float)SERVO_PW_MIN_US/(float)SERVO_PERIOD_US;
const uint16_t SERVO_TICK_MAX   = 4096 * (float)SERVO_PW_MAX_US/(float)SERVO_PERIOD_US;

#define PETAL_HOLD_STATE 0
#define PETAL_DROP_STATE 1

#define SERVO_MIN_STATE_MS      500
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

uint8_t servoState              = 0;
uint8_t servoState_cur  		= 0;
uint8_t servoState_prev         = 0;
uint32_t servoCounter 			= 0;
elapsedMillis servoElapsed1_ms 	= 0;
elapsedMillis servoElapsed2_ms 	= 0;
bool servoStateChange           = false;
uint8_t petalDropState          = PETAL_HOLD_STATE;

void setServo( const uint8_t servoNumber, const uint8_t angle )
{
	const uint16_t pwm = map( angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_TICK_MIN, SERVO_TICK_MAX );

	#ifdef ROSE_DEBUG
	Serial.print("setServo number: "); Serial.print(servoNumber);
	Serial.print(" angle: "); Serial.print(angle);
	Serial.print(" pwm: "); Serial.print(pwm);
	Serial.println("");
	#endif 

	#ifndef NO_SERVOS
	servos.setPWM(servoNumber, 0, pwm );
	#endif
}

void servoHoldPetal( const uint8_t servoNumber, const uint32_t & state_counter )
{
	if( state_counter == 0 )
	{
		setServo(servoNumber, SERVO_PETAL_HOLD_ANGLES[servoNumber] );
		petalDropState = PETAL_HOLD_STATE;
	}
}

void setupServos()
{
	#ifndef NO_SERVOS
	servos.begin();
	servos.setOscillatorFrequency(SERVO_OSC_FREQ_HZ);
	servos.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
	#endif

	for( uint8_t servoIdx=0; servoIdx < NUM_SERVOS; servoIdx++ )
		servoHoldPetal( servoIdx, 0 );
	
	servoState 			= 0;
	servoState_prev 	= 0;
	servoState_cur 		= 0;
	servoStateChange 	= false;
}

void servoDropPetal( const uint8_t servoNumber, const uint32_t & state_counter )
{
	// drop the petal and then return servo to hold angle to allow for reset 
	// and to minimize stress on mechanism
	if( state_counter == 0 )
	{
		#ifdef ROSE_DEBUG
		Serial.println("sservoDropPetal. PETAL_DROP_STATE: ");
		#endif 
		
		setServo(servoNumber, SERVO_PETAL_DROP_ANGLES[servoNumber] );
		servoElapsed2_ms  	= 0;
		petalDropState 		  = PETAL_DROP_STATE;
	}
	else if( servoElapsed2_ms > 1000 && petalDropState == PETAL_DROP_STATE)
	{
		#ifdef ROSE_DEBUG
		Serial.println("sservoDropPetal. PETAL_HOLD_STATE: ");
		#endif 
		
		setServo(servoNumber, SERVO_PETAL_HOLD_ANGLES[servoNumber] );
		petalDropState 		  = PETAL_HOLD_STATE;
	}
}

void handleServos()
{
	// only process state changes if we are not in the middle of dropping a petal
	if( petalDropState == PETAL_HOLD_STATE )
	{
		// register state change
		if( servoState != servoState_prev )
		{
			#ifdef ROSE_DEBUG
			Serial.print("servo state change. state: "); Serial.println(servoState);
			#endif 
			
			// lock in the new state
			servoState_cur   = servoState;
			servoElapsed1_ms = 0;
			servoStateChange = true;
			servoCounter     = 0;
		}
	}
	
	// Require state to exist for some time
	if( servoElapsed1_ms > SERVO_MIN_STATE_MS )
	{
		switch( servoState_cur )
		{
			case SERVO_PETAL_1_DMX:
				servoDropPetal(SERVO_PETAL_1, servoCounter );
				break;
			case SERVO_PETAL_2_DMX:
				servoDropPetal(SERVO_PETAL_2, servoCounter );
				break;
			case SERVO_PETAL_3_DMX:
				servoDropPetal(SERVO_PETAL_3, servoCounter );
				break;
			case SERVO_PETAL_4_DMX:
				servoDropPetal(SERVO_PETAL_4, servoCounter );
				break;
			default:
				break;
		}
		servoCounter++;
	}
	
	servoState_prev = servoState_cur;
	
}

#endif
