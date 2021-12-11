#include <Wire.h>

#undef ROSE_DEBUG
//#define ROSE_DEBUG 

#include "dmx.h"
#include "button.h"
#include "blink.h"
#include "servo_functions.h"
#include "indicator.h"

void setup() {
  #ifdef ROSE_DEBUG
  Serial.begin(9600);
  while(!Serial);
  #endif

  #ifdef ROSE_DEBUG
  Serial.println("start setup");
  #endif

  setupDMX();
  setupButton();
  setupBlink();
  setupServos();
  setupIndicator();

  #ifdef ROSE_DEBUG
  Serial.println("done setup");  
  #endif
  
  
}

void loop() {
  handleDMX();
  handleButton();
  handleBlink();
  handleServos();
  handleIndicator();
}
