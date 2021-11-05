#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include "setup.h"

void setupServos()
{
	servos.begin();
	servos.setOscillatorFrequency(27000000);
	servos.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
}

void setServo( const uint8_t servoNumber, const uint8_t angle )
{
	const uint16_t pwm = map( angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, SERVO_TICK_MIN, SERVO_TICK_MAX );

	Serial.print("setServo number: "); Serial.print(servoNumber);
	Serial.print(" angle: "); Serial.print(angle);
	Serial.print(" pwm: "); Serial.print(pwm);
	Serial.println("");
	
	servos.setPWM(servoNumber, 0, pwm );
}

void servoHoldPetal( const uint8_t servoNumber )
{
	setServo(servoNumber, SERVO_PETAL_HOLD_ANGLE );
}

void servoDropPetal( const uint8_t servoNumber )
{
	setServo(servoNumber, SERVO_PETAL_DROP_ANGLE );
}

#endif