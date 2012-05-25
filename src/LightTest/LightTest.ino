/*
 * Character LCD Example Code
 * 
 * Connections
 * LCD -> Arduino
 * 4 (RS) -> D12
 * 5 (RW) -> D11
 * 6 (ENABLE) -> D10
 * Ultrasonic trigger -> D6
 * Ultrasonic echo -> D7
 * tilt switch -> D8
 * tilt indicator LED -> D9
 * red distance LED -> A2
 * yellow distance LED -> A3
 * 11 (D4) -> D5
 * 12 (D5) -> D4
 * 13 (D6) -> D3
 * 14 (D7) -> D2
 * 15 (BACKLIGHT+) -> D13
 *
 * Photoresistors
 * left -> A0
 * right -> A1

 */
#include <LiquidCrystal.h>

// LCD 
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
const int backLight = 13;

// Photoresistors
const int leftSensor = A0;
const int rightSensor = A1;

// Ultrasonic sensor
const int triggerPin = 6;
const int echoPin = 7;

// Tilt switch and indicator
const int tiltSwitchPin = 8;
const int tiltLED = 9;

// Distance indicator LEDs
const int dangerLED = A2;
const int warningLED = A3;

void triggerUltrasonic()
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
}

long readDistanceCm()
{
  long echoTime = pulseIn(echoPin, HIGH);
  return echoTime < 14500 ? echoTime / 59 : -1;
}

void setup()
{
  pinMode(backLight, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(tiltSwitchPin, INPUT);
  digitalWrite(tiltSwitchPin, HIGH);
  pinMode(tiltLED, OUTPUT);
  digitalWrite(tiltLED, LOW);

  pinMode(dangerLED, OUTPUT);
  digitalWrite(dangerLED, LOW);
  pinMode(warningLED, OUTPUT);
  digitalWrite(warningLED, LOW);

  // Turn on the backlight.
  digitalWrite(backLight, HIGH);

  // Configure the LCD interface.
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.setCursor(8, 0);
  lcd.print("R:");
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  lcd.setCursor(0, 2);
  lcd.print("Distance (cm):");
}

String padWithSpaces(const String& s, int len)
{
  String result(s);
  while (result.length() < len) {
    result += " ";
  }

  return result;
}

void display(int left, int right, long distance)
{
  String leftStr(left);
  String rightStr(right);

  leftStr = padWithSpaces(leftStr, 4);
  rightStr = padWithSpaces(rightStr, 4);

  int diff = left - right;
  lcd.setCursor(3, 0);
  lcd.print(leftStr);
  lcd.setCursor(11, 0);
  lcd.print(rightStr);

  String lightDirection;
  if (diff < -20) {
    lightDirection = "right       ";
  } else if (diff > 20) {
    lightDirection = "left        ";
  } else {
    lightDirection = "ahead/behind";
  }
  
  lcd.setCursor(7, 1);
  lcd.print(lightDirection);

  String distanceStr;
  if (distance > 0) {
    distanceStr = String(distance);
  } else {
    distanceStr = "----";
  }
  distanceStr = padWithSpaces(distanceStr, 4);
  lcd.setCursor(15, 2);
  lcd.print(distanceStr);
}

void loop()
{
  int leftLight = analogRead(A1);
  int rightLight = analogRead(A0);

  triggerUltrasonic();
  long distance = readDistanceCm();

  if (0 < distance && distance < 10) {
    digitalWrite(dangerLED, HIGH);
    digitalWrite(warningLED, LOW);
  } else if (10 < distance && distance  < 20) {
    digitalWrite(dangerLED, LOW);
    digitalWrite(warningLED, HIGH);
  } else {
    digitalWrite(dangerLED, LOW);
    digitalWrite(warningLED, LOW);
  }

    

  int tilt = digitalRead(tiltSwitchPin);
  digitalWrite(tiltLED, tilt);

  display(leftLight, rightLight, distance);
  delay(250);
}
