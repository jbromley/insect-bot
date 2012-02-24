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
int frontPos = 90;
int rearPos = 90;

void setup()
{
  Serial.begin(9600);

  frontServo.attach(2);
  rearServo.attach(3);

  frontServo.write(frontPos);
  rearServo.write(rearPos);
}

void loop()
{
  oldPosition = frontPos;
  int rawValue = analogRead(A0);
  frontPos = map(rawValue, 0, 1023, 0, 180);
  rearPos = map(rawValue, 0, 1024, 180, 0);
  if (frontPos != oldPosition) {
    Serial.print(frontPos);
    Serial.print(", ");
    Serial.println(rearPos);
    frontServo.write(frontPos);
    rearServo.write(rearPos);
  }
}
