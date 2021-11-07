#ifndef SETUP_H
#define SETUP_H

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define SERVO_MAX_ANGLE 120
#define SERVO_MIN_ANGLE 0
#define SERVO_INITIAL_ANGLE SERVO_MIN_ANGLE
#define SERVO_PW_MIN_US   878 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_PW_MAX_US   1900 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define SERVO_FREQ_HZ     50 // Analog servos run at ~50/20 Hz updates

#define NUM_SERVOS 4
// Mapping of petal number to servo number
#define SERVO_PETAL_1 0
#define SERVO_PETAL_2 1
#define SERVO_PETAL_3 2
#define SERVO_PETAL_4 3
const uint32_t SERVO_PERIOD_US  = 1e6*1/SERVO_FREQ_HZ; // Analog servos run at ~50/20 Hz updates
const uint16_t SERVO_TICK_MIN   = 4096 * (float)SERVO_PW_MIN_US/(float)SERVO_PERIOD_US;
const uint16_t SERVO_TICK_MAX   = 4096 * (float)SERVO_PW_MAX_US/(float)SERVO_PERIOD_US;

#define DELAY_MS 10

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

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
