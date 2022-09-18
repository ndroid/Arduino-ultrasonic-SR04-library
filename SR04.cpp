/**
 * SR04.cpp
 * Arduino\Energia library for SR04 ultrasonic distance sensor.
 * Forked from mrRobot62/Arduino-ultrasonic-SR04-library
 * 
 * modified-by: miller
 * last-modified: 9/18/2022
 */
#include "SR04.h"

#define FETCH_TIMEOUT       5       // re-attempts allowed when averaging
#define TRIG_PULSE_WIDTH    20      // 20 us
#define SETTLE_TIME         4       // 4 us
#define FAILURE_DELAY       10000   // 10 ms

SR04::SR04(int echoPin, int triggerPin) {
    _echoPin = echoPin;
    _triggerPin = triggerPin;
    _autoMode = false;
    _distance = MAX_DISTANCE;
}

SR04::SR04(int echoTrigPin) {
    _echoPin = echoTrigPin;
    _triggerPin = echoTrigPin;
    _autoMode = true;
    _distance = MAX_DISTANCE;
}

void SR04::init() {
    if (_autoMode) {
        pinMode(_echoPin, INPUT);
    } else {
        pinMode(_echoPin, INPUT);
        pinMode(_triggerPin, OUTPUT);
    }
}

long SR04::Distance() {
    long d = 0;
    _duration = 0;
    if (_autoMode) {
        pinMode(_triggerPin, OUTPUT);
    }
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(SETTLE_TIME);
    // check if echo signal already high
    if (digitalRead(_echoPin) == HIGH) { 
        // reduce frequency of failed attempts (for looping operations)
        delayMicroseconds(FAILURE_DELAY);
        return MAX_DISTANCE;
    }
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(TRIG_PULSE_WIDTH);
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(SETTLE_TIME);
    if (_autoMode) {
        pinMode(_echoPin, INPUT);
    }
    _duration = pulseIn(_echoPin, HIGH, PULSE_TIMEOUT);
    d = MicrosecondsToCentimeter(_duration);
    return d;
}

long SR04::DistanceAvg(int wait, int count) {
    long min, max, avg, d;
    int fetch;
    min = 999;
    max = 0;
    avg = d = 0;

    if (wait < 1) {
        wait = 1;
    }

    if (count < 1) {
        count = 1;
    }

    for (int x = 0; x < count + 2; x++) {
        fetch = 0;
        d = Distance();
        while (d == MAX_DISTANCE) {
            if ((++fetch) > FETCH_TIMEOUT) {
                return MAX_DISTANCE;
            }
            delayMicroseconds(FAILURE_DELAY);
            d = Distance();
        } 

        if (d < min) {
            min = d;
        }

        if (d > max) {
            max = d;
        }

        avg += d;
        delay(wait);
    }

    // substract highest and lowest value
    avg -= (max + min);
    // calculate average
    avg /= count;
    return avg;
}

void SR04::Ping() {
    _distance = Distance();
}

long SR04::getDistance() {
    return _distance;
}

long SR04::MicrosecondsToCentimeter(long duration) {
    long d = (duration * 100) / 5882;
    if ((d == 0) || (d > MAX_DISTANCE)) {
        d = MAX_DISTANCE;
    }
    return d;
}
