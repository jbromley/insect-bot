/*
 * WalkForward.ino
 * Walk the insect-bot forward.
 *
 * Connections
 * D2 - front servo control
 * D3 - rear servo control
 */
#include <Servo.h>

enum WalkType {
  FORWARD, BACKWARD, LEFT, RIGHT, BACK_LEFT, BACK_RIGHT
};

enum {
  STRIDE_DELAY = 5
};

const char frontCenter = 86;
const char rearCenter = 84;

char gaits[][8] = {
  { -18, 15, 0, 0, 18, -15, 0, 0 },
  { -18, -15, 0, 0, 18, 15, 0, 0 },
  { -27, 6, -9, -9, 27, -24, -9, -9 },
  { 27, -6, 9, 9, -27, 24, 9, 9},
  { -18, -21, 0, -6, 27, 9, 0, 0},
  { 18, 21, 0, 6, -27, -9, 0, 0}
};

int delays[] = {125, 65, 125, 65, 65};

Servo frontServo;
Servo rearServo;

void setup()
{
  // Set up the gait table.
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (i % 2 == 0) {
	gaits[i][j] = gaits[i][j] + frontCenter;
      } else {
	gaits[i][j] = gaits[i][j] + rearCenter;
      }
    }
  }

  frontServo.attach(2);
  rearServo.attach(3);
}

void move(WalkType walk)
{
  for (int i = 0; i < 4; ++i) {
    frontServo.write(gaits[int(walk)][2 * i]);
    rearServo.write(gaits[int(walk)][2 * i + 1]);
    delay(delays[i]);
  }
}

void loop()
{
  // for (int i = 0; i < 10; ++i) {
  moveForward();
  delay(65);
  // }
  // for (int i = 0; i < 10; ++i) {
  //   moveTurnLeft();
  // }
  // for (int i = 0; i < 10; ++i) {
  //   moveBackward();
  // }
  // for (int i = 0; i < 10; ++i) {
  //   moveBackRight();
  // }
}
