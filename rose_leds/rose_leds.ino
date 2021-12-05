
#define ROSE_DEBUG 
#undef ROSE_DEBUG

#include "dmx_i2c.h"
#include "led_functions.h"
#include "blink.h"

void setup() {
  #ifdef ROSE_DEBUG
  Serial.begin(9600);           // start serial for output
  while(!Serial);
  #endif
  
  setupDMXI2C();
  setupLeds();
  setupBlink();
}

void loop() {
  handleLeds();
  handleBlink();
}
