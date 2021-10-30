
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Servos
int PIN_SERVO1      = 2;
int PIN_SERVO2      = 3;
int PIN_SERVO3      = 4;
int PIN_SERVO4      = 5;
int SERVO_MAX_ANGLE = 120;
int SERVO_MIN_ANGLE = 0;
int SERVO_PW_MIN    = 700;
int SERVO_PW_MAX    = 1900;
int SERVO_INITIAL_ANGLE = SERVO_MIN_ANGLE;

// Inputs
int PIN_RC_CH6 = 7;

int DELAY_ms = 100;
 
unsigned int ch6, ch6a, ch6b;

int SWITCH_ON_HIGH_THRESHOLD    = 1800;
int SWITCH_OFF_LOW_THRESHOLD    = 1100;
int RC_CONNECTED_THRESHOLD      = 900;
boolean drop_petal              = false;
int drop_on_count               = 0;
int drop_off_count              = 0;
int DROP_ON_COUNT_THRESHOLD1    = 5;
int DROP_OFF_COUNT_THRESHOLD1   = 10;
int DROP_OFF_COUNT_THRESHOLD2   = 5;
// 0=waiting for very long off
// 1=waiting for long on
// 2=waiting for long off
int drop_state                  = 0; 

int counter = 0;
int state   = 0;

int servoMap( int angle )
{
  return map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180 );
}

void state_reset()
{
  servo1.write(SERVO_INITIAL_ANGLE);
  servo2.write(SERVO_INITIAL_ANGLE);
  servo3.write(SERVO_INITIAL_ANGLE);
  servo4.write(SERVO_INITIAL_ANGLE);
  state = 0;
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

void setup() {

  pinMode(PIN_RC_CH6, INPUT);
 
  servo1.attach(PIN_SERVO1,SERVO_PW_MIN,SERVO_PW_MAX);
  servo2.attach(PIN_SERVO2,SERVO_PW_MIN,SERVO_PW_MAX);
  servo3.attach(PIN_SERVO3,SERVO_PW_MIN,SERVO_PW_MAX);
  servo4.attach(PIN_SERVO4,SERVO_PW_MIN,SERVO_PW_MAX);

  state_reset();
  
  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {
    
  ch6 = pulseIn(PIN_RC_CH6, HIGH, 25000); // CH6 SWB

  // filter the PWM inputs
  ch6b = (ch6 + ch6a) >> 1;

  if( false && counter % 10 == 0 )
  {
    Serial.print("Ch6: "); Serial.print(ch6);
    Serial.print(" Ch6b: "); Serial.print(ch6b);
    Serial.println("");
  }
  
  ch6a = ch6;

  // Petal drop state machine
  // require low for 1 sec
  // then high for 1/2 sec
  // then low for 1/2 sec
  if( ch6b > RC_CONNECTED_THRESHOLD )
  {
    if( ch6b > SWITCH_ON_HIGH_THRESHOLD )
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
    else if( ch6b < SWITCH_OFF_LOW_THRESHOLD )
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
          state = state + 1;
          Serial.print("DROP PETAL!!! State is now: ");
          Serial.println(state);
        }
      }
    }
  }

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
            
    default:
      state_reset();
      break;
  }
  
  counter = counter + 1;
 
  delay(DELAY_ms);
}
