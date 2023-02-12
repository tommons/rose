#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(LED_BUILTIN, OUTPUT);
}

#define DMX_I2C_ID         4
#define DMX_MSGID         2

uint8_t blinkState = 0;
int x = 0;

void loop() {
  // put your main code here, to run repeatedly:

    // Send the DMX values to the slave arduino
    Wire.beginTransmission(DMX_I2C_ID); // begin transmitting
    Wire.write(DMX_MSGID);
    Wire.write(x);
    Wire.write(x+1);
    Wire.write(x);
    Wire.write(x+1);
    Wire.write(x);
    Wire.write(x+1);
    Wire.write(x);
    Wire.write(x+1);
    Wire.write(x);
    Wire.endTransmission();    // stop transmitting
    x++;

    if( blinkState == HIGH )
    {
      blinkState = LOW;
    }
    else
    {
      blinkState = HIGH;
    }
    digitalWrite(LED_BUILTIN, blinkState);
    
    delay(1000);

    
}
