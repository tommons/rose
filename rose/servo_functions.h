#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include <Servo.h>
#include "setup.h"

uint32_t servoMap( const uint32_t angle )
{
  return map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180 );
}


#endif