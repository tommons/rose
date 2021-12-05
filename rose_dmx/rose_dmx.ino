#include <Wire.h>

//#define ROSE_DEBUG 
#undef ROSE_DEBUG

#include "dmx.h"
#include "button.h"
#include "blink.h"
#include "servo_functions.h"

void setup() {
  Serial.begin(9600);
  
  setupDMX();
  setupButton();
  setupBlink();
  setupServos();
}

void loop() {
  handleDMX();
  handleButton();
  handleBlink();
  handleServos();
}