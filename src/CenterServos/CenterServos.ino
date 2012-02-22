/*
 * CenterServos.ino
 * Center both servos.
 *
 * Connections
 * D2 - front servo control
 * D3 - rear servo control
 */
#include <Servo.h>

Servo frontServo;
Servo rearServo;

int oldPosition = 90;
int position = 90;

void setup()
{
  Serial.begin(9600);

  frontServo.attach(2);
  rearServo.attach(3);

  frontServo.write(90);
  rearServo.write(90);
}

void loop()
{
  oldPosition = position;
  int rawValue = analogRead(A0);
  position = map(rawValue, 0, 1023, 0, 180);
  if (position != oldPosition) {
    Serial.println(position);
    // frontServo.write(position);
    rearServo.write(position);
  }
}
