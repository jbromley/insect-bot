/*
 * BoardTest.ino
 * Test all components on the board.
 *
 * Connections
 * 
 * A0 -> left light sensor
 * A1 - right light sensor
 */
#include "Pitches.h"
#include "UltrasonicSR04.h"
#include "Bounce.h"

// Constants
const byte L_LIGHT_SENSOR = A3;
const byte R_LIGHT_SENSOR = A2;

const byte TONE_PIN = A1;
const int scale[] = {
  NOTE_D4, NOTE_F4, NOTE_A4, NOTE_G4, NOTE_B4, NOTE_D5, NOTE_E5, NOTE_C5
}; 

const byte US_TRIGGER_PIN = 3;
const byte US_ECHO_PIN = 2;

const byte TILT_PIN = A0;
const byte TILT_LED = 13;

// State variables
int ledState = LOW;
int lightPosition = 0;
UltrasonicSR04 usSensor(US_TRIGGER_PIN, US_ECHO_PIN);
int distance = 0;
Bounce tiltDebouncer = Bounce(TILT_PIN, 5);
int upright = HIGH;


void playScale()
{
  const int NUM_NOTES = sizeof(scale) / sizeof(scale[0]);
  for (int i = 0; i < NUM_NOTES; ++i) {
    tone(TONE_PIN, scale[i], 120);
    delay(125);
  }
}

void setup()
{
  // Light sensor pins.
  pinMode(L_LIGHT_SENSOR, INPUT);
  pinMode(R_LIGHT_SENSOR, INPUT);

  // Buzzer control pin.
  pinMode(TONE_PIN, OUTPUT);

  // Tilt switch and LED.
  pinMode(TILT_PIN, INPUT);
  digitalWrite(TILT_PIN, HIGH);
  pinMode(TILT_LED, OUTPUT);

  Serial.begin(9600);

  playScale();
}

void loop()
{
  // Light direction detection.
  lightPosition = analogRead(R_LIGHT_SENSOR) - analogRead(L_LIGHT_SENSOR);

  // Ultrasonic sensor
  distance = usSensor.distance();

  // Tilt switch.
  tiltDebouncer.update();
  upright = tiltDebouncer.read();
  // upright = digitalRead(TILT_LED);
  digitalWrite(TILT_LED, upright == HIGH ? LOW : HIGH);

  // Print status.
  Serial.print("l=");
  Serial.print(lightPosition);
  Serial.print(", d=");
  Serial.print(distance);
  Serial.print("cm, ");
  Serial.print(upright == HIGH ? "M" : "W");
  Serial.println("");

  // LED heartbeat
  // ledState = (ledState == HIGH ? LOW : HIGH);
  // digitalWrite(13, ledState);
}
