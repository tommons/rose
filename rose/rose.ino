
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Servos
#define PIN_SERVO1      2
#define PIN_SERVO2      3
#define PIN_SERVO3      4
#define PIN_SERVO4      5
#define SERVO_MAX_ANGLE 120
#define SERVO_MIN_ANGLE 0
#define SERVO_PW_MIN    700
#define SERVO_PW_MAX    1900
#define SERVO_INITIAL_ANGLE SERVO_MIN_ANGLE

// LEDs
#define PIN_LED_OUTER_RING   8
#define PIN_LED_INNER_RING   9
#define LED_COUNT_OUTER_RING 5
#define LED_COUNT_INNER_RING 24
uint32_t color = 0;

Adafruit_NeoPixel led_outer_ring(LED_COUNT_OUTER_RING, PIN_LED_OUTER_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_inner_ring(LED_COUNT_INNER_RING, PIN_LED_INNER_RING, NEO_GRB + NEO_KHZ800);

// Inputs
#define PIN_RC_CH6 7

#define DELAY_MS 50
 
unsigned int ch6_cur, ch6_prev, ch6_filt;

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
#define NUM_STATES 5
int state_counter[NUM_STATES] = {0};

int servoMap( int angle )
{
  return map(angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180 );
}

void state_reset()
{
  Serial.println("Doing state_reset!");
  
  servo1.write(SERVO_INITIAL_ANGLE);
  servo2.write(SERVO_INITIAL_ANGLE);
  servo3.write(SERVO_INITIAL_ANGLE);
  servo4.write(SERVO_INITIAL_ANGLE);
  
  state = 0;
  
  for( int i=0; i < NUM_STATES; i++ )
    state_counter[i] = 0;

  color = led_outer_ring.Color(0, 0, 0);
  for(int i=0; i < led_outer_ring.numPixels(); i++) // For each pixel in strip...
    led_outer_ring.setPixelColor(i, color); //  Set pixel's color (in RAM)
  led_outer_ring.show();
  
  color = led_inner_ring.Color(0, 0, 0);
  for(int i=0; i < led_inner_ring.numPixels(); i++) // For each pixel in strip...
    led_inner_ring.setPixelColor(i, color); //  Set pixel's color (in RAM)
  led_inner_ring.show();          
}

void state0()
{
  if( state_counter[0] > 100 )
  {
    Serial.println("State 0 call state1");
    state = 1;
  }
  
  state_counter[0]++;
}

void state1()
{
  // fade in to red and white
  // Outer Red
  uint16_t h1  = 0; 
  uint8_t s1   = 255; 
  uint8_t v1   = 255;
  // Inner White
  uint16_t h2  = 0; 
  uint8_t s2   = 0; 
  uint8_t v2   = 255;  
  const int fadeupcount = 5000 / DELAY_MS;

  if( state_counter[1] <= fadeupcount )
  {
    v1 = map( state_counter[1], 0, fadeupcount, 0, 255);
    v2 = map( state_counter[1], 0, fadeupcount, 0, 255);

    // Outer
    color = led_outer_ring.ColorHSV(h1, s1, v1);
    for(int i=0; i < led_outer_ring.numPixels(); i++) // For each pixel in strip...
      led_outer_ring.setPixelColor(i, color); //  Set pixel's color (in RAM)
    led_outer_ring.show(); 

    // Inner
    color = led_inner_ring.ColorHSV(h2, s2, v2);
    for(int i=0; i < led_inner_ring.numPixels(); i++) // For each pixel in strip...
      led_inner_ring.setPixelColor(i, color); //  Set pixel's color (in RAM)
    led_inner_ring.show();    
  }
  else
  {
    if( state_counter[1] <= 2*fadeupcount )
    {
      Serial.println("State 1 call state_reset");
      state_reset();
    }
  }
  
  state_counter[1]++;
}

void state2()
{
  servo2.write( servoMap( SERVO_MAX_ANGLE ) );
  state_counter[2]++;
}

void state3()
{
  servo3.write( servoMap( SERVO_MAX_ANGLE ) );
  state_counter[3]++;
}

void state4()
{
  servo4.write( servoMap( SERVO_MAX_ANGLE ) );
  state_counter[4]++;
}

void setup() {

  pinMode(PIN_RC_CH6, INPUT);

  led_outer_ring.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  led_outer_ring.show();            // Turn OFF all pixels ASAP
  led_outer_ring.setBrightness(127); // Set BRIGHTNESS (max = 255)
  
  led_inner_ring.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  led_inner_ring.show();            // Turn OFF all pixels ASAP
  led_inner_ring.setBrightness(127); // Set BRIGHTNESS (max = 255)
  
  servo1.attach(PIN_SERVO1,SERVO_PW_MIN,SERVO_PW_MAX);
  servo2.attach(PIN_SERVO2,SERVO_PW_MIN,SERVO_PW_MAX);
  servo3.attach(PIN_SERVO3,SERVO_PW_MIN,SERVO_PW_MAX);
  servo4.attach(PIN_SERVO4,SERVO_PW_MIN,SERVO_PW_MAX);

  state_reset();
  
  Serial.begin(9600); // Pour a bowl of Serial

}

void loop() {

  // read RC states
  ch6_cur = pulseIn(PIN_RC_CH6, HIGH, 25000); // CH6 SWB

  // filter the PWM inputs
  //ch6_filt = (ch6_cur + ch6_prev) >> 1; // 2point average
  ch6_filt = (ch6_cur >> 1) + (ch6_filt >> 1); // alpha filter

  if( false && counter % 10 == 0 )
  {
    Serial.print("Ch6: "); Serial.print(ch6_cur);
    Serial.print(" Ch6filt: "); Serial.print(ch6_filt);
    Serial.println("");
  }
  
  ch6_prev = ch6_cur;

  // Petal drop state machine
  // require low for 1 sec
  // then high for 1/2 sec
  // then low for 1/2 sec
  if( ch6_filt > RC_CONNECTED_THRESHOLD )
  {
    if( ch6_filt > SWITCH_ON_HIGH_THRESHOLD )
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
    else if( ch6_filt < SWITCH_OFF_LOW_THRESHOLD )
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
 
  delay(DELAY_MS);
}
