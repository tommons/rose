#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include "setup.h"

#define SERVO_PETAL_HOLD_ANGLE 120
#define SERVO_PETAL_DROP_ANGLE 50

void setupServos()
{
	servos.begin();
	servos.setOscillatorFrequency(27000000);
	servos.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
}

uint32_t servoCounter = 0;
elapsedMillis servoElapsed1_ms = 0;

void setServo( const uint8_t servoNumber, const uint8_t angle )
{
	const uint16_t pwm = map( angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_TICK_MIN, SERVO_TICK_MAX );

	Serial.print("setServo number: "); Serial.print(servoNumber);
	Serial.print(" angle: "); Serial.print(angle);
	Serial.print(" pwm: "); Serial.print(pwm);
	Serial.println("");
	
	servos.setPWM(servoNumber, 0, pwm );
}

void servoHoldPetal( const uint8_t servoNumber, const uint32_t state_counter )
{
  if( state_counter == 0 )
	  setServo(servoNumber, SERVO_PETAL_HOLD_ANGLE );
}

void servoDropPetal( const uint8_t servoNumber, const uint32_t state_counter )
{
    if( state_counter == 0 )
    {
    	  setServo(servoNumber, SERVO_PETAL_DROP_ANGLE );
        servoElapsed1_ms  = 0;
        servoCounter      = 0;
    }
    else if( servoElapsed1_ms > 1000 && servoCounter == 0)
    {
        setServo(servoNumber, SERVO_PETAL_HOLD_ANGLE );
        servoCounter      = 1;
    }
}

#endif
