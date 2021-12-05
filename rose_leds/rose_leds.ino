
//#define ROSE_DEBUG 
#undef ROSE_DEBUG

#include "dmx_i2c.h"
#include "led_functions.h"
#include "blink.h"

void setup() {
  Serial.begin(9600);           // start serial for output

  setupDMXI2C();
  setupLeds();
  setupBlink();
}

void loop() {
  handleLeds();
  handleBlink();
}
