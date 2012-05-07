// Ultrasonic SR04 sensor implementation
#include "UltrasonicSR04.h"

/**
 * @class UltrasonicSR04 
 * 
 * This class interfaces to the SR04 ultrasonic sensor.  
 * Generally, users will instantiate an instance of this class and then
 * call the @c distance method when they need to measure a
 * distance. This class also provides methods to measure an average
 * distance over a number of pings or explicitly emit pings and read the
 * distance.
 */

/** 
 * Maximum time to wait for a return pulse.
 * The SR04 data sheet claims a range of 5 meters (or 29411 microseconds).
 */
const long UltrasonicSR04::PULSE_TIMEOUT = 30000L;

/**
 * Time between pings in average mode.
 */
const int UltrasonicSR04::DEFAULT_DELAY = 10;

/**
 * Number of pings to do in average mode.
 */
const int UltrasonicSR04::DEFAULT_PINGS = 5;

/**
 * Create an ultrasonic sensor controlled by the given pins.
 * @param triggerPin the pin that triggers pulse emission.
 * @param echoPin the pin used to listen for return pulses.
 */ 
UltrasonicSR04::UltrasonicSR04(int triggerPin, int echoPin) 
  : _triggerPin(triggerPin), _echoPin(echoPin), _distance(-1), _autoMode(false)
{
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

/**
 * Measure the distance in centimeters to the nearest object.
 * @return The distance in centimeters to the nearest object. Note that if the
 *         echo times out, there is no object in range and 0 is returned.
 */
long UltrasonicSR04::distance() 
{
    _duration = 0;
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    _duration = pulseIn(_echoPin, HIGH, PULSE_TIMEOUT);
    return microsecondsToCentimeter(_duration);
}

/**
 * Measure the average distance in centimeters to the nearest object.
 * This method does multiple pings, eliminates the minimum and maximum
 * ping times and then returns the average distance according to the 
 * remaining pings.
 * @param wait The wait time in microseconds betweeen distance pings.
 * @param count The number of pings to average.
 * @return The distance in centimeters to the nearest object. Note that if the
 *         echo times out, there is no object in range and 0 is returned.
 */
long UltrasonicSR04::distanceAvg(int wait, int count) {
  long minDist = 999;
  long maxDist = 0;
  long avgDist = 0;
  long dist = 0;

  // Make sure parameters are sane.
  wait = max(25, wait);
  count = max(1, count);

  // Do multiple pings and average.
  for (int i = 0; i < count + 2; ++i) {
    dist = distance();
    minDist = min(minDist, dist);
    maxDist = max(maxDist, dist);
    avgDist += dist;
  }

  // Substract highest and lowest value from the average.
  avgDist -= (maxDist + minDist);
  avgDist /= count;
  return avgDist;
}

/**
 * Sends a single ping and measures the distance to the nearest object.
 * The caller must subsequently call @c getDistance to get the measured 
 * distance.
 */
void UltrasonicSR04::ping() 
{
  _distance = distance();
}

/**
 * Gets the distance measured by the last @c ping call.
 * @return The distance to the nearest object in centimeters from the last 
 *         @c ping call.
 */
long UltrasonicSR04::getDistance() {
  return _distance;
}

/**
 * Converts pulse length to distance.
 * The goes from the pulse length in microseconds to the distance in
 * centimeters.
 * @param duration The pulse time in microseconds.
 * @return The distance in centimeters corresponding to the given pulse 
 *         length.
 */
long UltrasonicSR04::microsecondsToCentimeter(long duration) 
{
  return duration * 100 / 5882;
}
