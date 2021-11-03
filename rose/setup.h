#ifndef SETUP_H
#define SETUP_H

#include <Adafruit_NeoPixel.h>

#define SERVO_MAX_ANGLE 120
#define SERVO_MIN_ANGLE 0
#define SERVO_PW_MIN    700
#define SERVO_PW_MAX    1900
#define SERVO_INITIAL_ANGLE SERVO_MIN_ANGLE

#define DELAY_MS 10

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Servos
#define PIN_SERVO1      2
#define PIN_SERVO2      3
#define PIN_SERVO3      4
#define PIN_SERVO4      5

// Inputs
#define PIN_RC_CH6 7
#define PIN_SWITCH 6

// LEDs
#define PIN_LED_OUTER_RING   8
#define PIN_LED_INNER_RING   9
#define LED_COUNT_OUTER_RING 24
#define LED_COUNT_INNER_RING 24
#define LED_MAX_BRIGHTNESS 91

Adafruit_NeoPixel led_outer_ring(LED_COUNT_OUTER_RING, PIN_LED_OUTER_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led_inner_ring(LED_COUNT_INNER_RING, PIN_LED_INNER_RING, NEO_GRB + NEO_KHZ800);

#endif