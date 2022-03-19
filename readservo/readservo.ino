
#include <Servo.h>

Servo servo1;
int PIN_SERVO1    = 2;
int PIN_SERVO2    = 3;
int PIN_SERVO3    = 4;
int PIN_SERVO4    = 5;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  servo1.attach(PIN_SERVO1,700,1900);

}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
  
    // look for the next valid integer in the incoming serial stream:
    int pw = Serial.parseInt();
    if( pw > 0 )
    {
      Serial.print("PW: "); Serial.println(pw);
          
      servo1.write(pw);
    }
  }
}
