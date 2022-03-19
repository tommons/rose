
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int PIN_SERVO1      = 2;
int PIN_SERVO2      = 3;
int PIN_SERVO3      = 4;
int PIN_SERVO4      = 5;
int SERVO_MAX_ANGLE = 120;
int SERVO_MIN_ANGLE = 0;
int SERVO_PW_MIN    = 700;
int SERVO_PW_MAX    = 1900;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  servo1.attach(PIN_SERVO1,SERVO_PW_MIN,SERVO_PW_MAX);
  servo2.attach(PIN_SERVO2,SERVO_PW_MIN,SERVO_PW_MAX);
  servo3.attach(PIN_SERVO3,SERVO_PW_MIN,SERVO_PW_MAX);
  servo4.attach(PIN_SERVO4,SERVO_PW_MIN,SERVO_PW_MAX);

}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
  
    // look for the next valid integer in the incoming serial stream:
    int angle = Serial.parseInt();
    if (Serial.read() == '\n')
    {
      if( angle >= SERVO_MIN_ANGLE && angle  <= SERVO_MAX_ANGLE )
      {
        Serial.print("angle: "); Serial.println(angle);
        int angle_map = map( angle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 180 );
        servo1.write(angle_map);
        servo2.write(angle_map);
        servo3.write(angle_map);
        servo4.write(angle_map);
      }
    }
  }
}
