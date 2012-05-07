// UltrasonicSR04
// Library for use with SR04 ultrasonic sensor.
#ifndef ULTRASONICSR04_H
#define ULTRASONICSR04_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include <inttypes.h>


class UltrasonicSR04 {
public:
    
  UltrasonicSR04(int triggerPin, int echoPin);

  long distance();
  long distanceAvg(int wait = DEFAULT_DELAY, int count = DEFAULT_PINGS);
  void ping();
  long getDistance();

private:
  static const long PULSE_TIMEOUT;
  static const int DEFAULT_DELAY;
  static const int DEFAULT_PINGS;
  
  long microsecondsToCentimeter(long duration);
    
  int _triggerPin;
  int _echoPin;
  long _duration;
  long _distance;
  bool _autoMode;
};

#endif // ULTRASONICSR04_H
