
int servoMap( int angle )
{
  return map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180 );
}

void state0()
{
	state_reset();
}

void state1()
{
  servo1.write( servoMap( SERVO_MAX_ANGLE ) );
}

void state2()
{
  servo2.write( servoMap( SERVO_MAX_ANGLE ) );
}

void state3()
{
  servo3.write( servoMap( SERVO_MAX_ANGLE ) );
}

void state4()
{
  servo4.write( servoMap( SERVO_MAX_ANGLE ) );
}

void state_reset()
{
  servo1.write(SERVO_INITIAL_ANGLE);
  servo2.write(SERVO_INITIAL_ANGLE);
  servo3.write(SERVO_INITIAL_ANGLE);
  servo4.write(SERVO_INITIAL_ANGLE);
}

