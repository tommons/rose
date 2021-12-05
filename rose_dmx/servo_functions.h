#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include <Adafruit_PWMServoDriver.h>
#include <elapsedMillis.h>

#define SERVO_MAX_ANGLE 120
#define SERVO_MIN_ANGLE 0
#define SERVO_INITIAL_ANGLE SERVO_MIN_ANGLE
#define SERVO_PW_MIN_US   878 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_PW_MAX_US   1900 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_FREQ_HZ     50 // Analog servos run at ~50/20 Hz updates
#define SERVO_OSC_FREQ_HZ 27000000

#define SERVO_PETAL_HOLD_ANGLE 120
#define SERVO_PETAL_DROP_ANGLE 50

#define NUM_SERVOS 4
// Mapping of petal number to servo number
#define SERVO_PETAL_1 0
#define SERVO_PETAL_2 1
#define SERVO_PETAL_3 2
#define SERVO_PETAL_4 3
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
uint8_t servoState_prev         = 0;
uint32_t servoCounter 			= 0;
elapsedMillis servoElapsed1_ms 	= 0;
elapsedMillis servoElapsed2_ms 	= 0;
bool servoStateChange           = false;
uint8_t petalDropState          = PETAL_HOLD_STATE;

void setServo( const uint8_t servoNumber, const uint8_t angle )
{
	const uint16_t pwm = map( angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_TICK_MIN, SERVO_TICK_MAX );

	Serial.print("setServo number: "); Serial.print(servoNumber);
	Serial.print(" angle: "); Serial.print(angle);
	Serial.print(" pwm: "); Serial.print(pwm);
	Serial.println("");
	
	servos.setPWM(servoNumber, 0, pwm );
}

void servoHoldPetal( const uint8_t servoNumber, const uint32_t & state_counter )
{
	if( state_counter == 0 )
	{
		setServo(servoNumber, SERVO_PETAL_HOLD_ANGLE );
		petalDropState = PETAL_HOLD_STATE;
	}
}

void setupServos()
{
	servos.begin();
	servos.setOscillatorFrequency(SERVO_OSC_FREQ_HZ);
	servos.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
	
	for( uint8_t servoIdx=0; servoIdx < NUM_SERVOS; servoIdx++ )
		servoHoldPetal( servoIdx, 0 );
	
	servoState 			= 0;
	servoState_prev 	= 0;
	servoStateChange 	= false;
}

void servoDropPetal( const uint8_t servoNumber, const uint32_t & state_counter )
{
	// drop the petal and then return servo to hold angle to allow for reset 
	// and to minimize stress on mechanism
    if( state_counter == 0 )
    {
    	setServo(servoNumber, SERVO_PETAL_DROP_ANGLE );
        servoElapsed2_ms  	= 0;
		petalDropState 		= PETAL_DROP_STATE;
    }
    else if( servoElapsed2_ms > 1000 && petalDropState == PETAL_DROP_STATE)
    {
        setServo(servoNumber, SERVO_PETAL_HOLD_ANGLE );
		petalDropState 		= PETAL_HOLD_STATE;
    }
}

void handleServos()
{
	// register state change
	if( servoState != servoState_prev )
	{
		servoElapsed1_ms = 0;
		servoStateChange = true;
		servoCounter     = 0;
	}
	
	// Require state to exist for some time
	if( servoElapsed1_ms > SERVO_MIN_STATE_MS )
	{
		switch( servoState )
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
	
	servoState_prev = servoState;
}

#endif
