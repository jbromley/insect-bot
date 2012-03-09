/*
 * Character LCD Example Code
 * 
 * Connections
 * LCD -> Arduino
 * 4 (RS) -> D12
 * 5 (RW) -> D11
 * 6 (ENABLE) -> D10
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

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
const int backLight = 13;
const int leftSensor = A0;
const int rightSensor = A1;

void setup()
{
  pinMode(backLight, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

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
}

String padWithSpaces(const String& s, int len)
{
  String result(s);
  while (result.length() < len) {
    result += " ";
  }

  return result;
}

void display(int left, int right)
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
}

void loop()
{
  int leftLight = analogRead(A1);
  int rightLight = analogRead(A0);
  display(leftLight, rightLight);
  delay(250);
}
