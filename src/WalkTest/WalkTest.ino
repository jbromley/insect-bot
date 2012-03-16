/*
 * WalkForward.ino
 * Walk the insect-bot forward.
 *
 * Connections
 * D2 - front servo control
 * D3 - rear servo control
 */
#include <Servo.h>
#include <IRremote.h>

// The number of steps in a gait. There are four steps: movement 1, 
// center, movement 2, and center. Each step has to move both the 
// front and rear servos.
const int GAIT_STEPS = 8;

// The number of delays in the gait.
const int GAIT_DELAYS = 4;

// Scale the pauses in the gait.
const float TIME_SCALE = 1.0f;

// Types of movements. These index into the gaits array.
enum WalkType {
  FORWARD, BACKWARD, LEFT, RIGHT, BACK_LEFT, BACK_RIGHT, NUM_GAITS, STOP
};

// The gait positions.
char gaits[][GAIT_STEPS] = {
  { -18, 15, 0, 0, 18, -15, 0, 0 },
  { -18, -15, 0, 0, 18, 15, 0, 0 },
  { -27, 6, -9, -9, 27, -24, -9, -9 },
  { 27, -6, 9, 9, -27, 24, 9, 9 },
  { -18, -21, 0, -6, 27, 9, 0, 0 },
  { 18, 21, 0, 6, -27, -9, 0, 0 }
};

// Walking mode.
WalkType walkMode = STOP;

// Delays between each of the four gait steps and the delay between
// each gait cycle.
int delays[] = { 125, 65, 125, 65 };
// int delays[] = { 250, 130, 250, 130 };

// The delay between each gait cycle.
const int STRIDE_DELAY = 130;

// Center positions
const char FRONT_CENTER = 86;
const char REAR_CENTER = 82;

// Servo controllers
const int FRONT_SERVO_PIN = 2;
const int REAR_SERVO_PIN = 3;
Servo frontServo;
Servo rearServo;

// IR Receiver
const int IR_RECV_PIN = 4; 
IRrecv irrecv(IR_RECV_PIN);
decode_results results;


void setup()
{
  // Set up the gait table.
  for (int i = 0; i < NUM_GAITS; ++i) {
    for (int j = 0; j < GAIT_STEPS; ++j) {
      if (i % 2 == 0) {
	gaits[i][j] = gaits[i][j] + FRONT_CENTER;
      } else {
	gaits[i][j] = gaits[i][j] + REAR_CENTER;
      }
    }
  }

  // Scale the gait delay times.
  for (int i = 0; i < GAIT_DELAYS; ++i) {
    delays[i] = static_cast<int>(TIME_SCALE * delays[i]);
  }

  // Set up servos.
  frontServo.attach(FRONT_SERVO_PIN);
  rearServo.attach(REAR_SERVO_PIN);

  // Set up IR receiver.
  irrecv.enableIRIn();
}

void move(WalkType walk)
{
  const int walkIndex = int(walk);
  for (int i = 0; i < 4; ++i) {
    frontServo.write(gaits[walkIndex][2 * i]);
    rearServo.write(gaits[walkIndex][2 * i + 1]);
    delay(delays[i]);
  }
}

void loop()
{
  if (irrecv.decode(&results)) {
    switch (results.value) {
    case 0xFF18E7:
      // Forward
      walkMode = FORWARD;
      break;
    case 0xFF30CF:
      walkMode = LEFT;
      break;
    case 0xFF7A85:
      walkMode = RIGHT;
      break;
    case 0xFF38C7:
      walkMode = STOP;
      break;
    case 0xFF42BD:
      walkMode = BACK_LEFT;
      break;
    case 0xFF4AB5:
      walkMode = BACKWARD;
      break;
    case 0xFF52AD:
      walkMode = BACK_RIGHT;
      break;
    default:
      // Do nothing.
      break;
    }
    
    irrecv.resume();
  }

  if (walkMode != STOP) {
    move(walkMode);
  }
}
