
#include "SR04.h"

#define PULSE_TIMEOUT   150000L	// 100ms
#define FETCH_TIMEOUT       10
#define TRIG_PULSE_WIDTH    12

SR04::SR04(int echoPin, int triggerPin) {
    _echoPin = echoPin;
    _triggerPin = triggerPin;
    _autoMode = false;
    _distance = MAX_DISTANCE;
}

void SR04::init() {
    pinMode(_echoPin, INPUT);
    pinMode(_triggerPin, OUTPUT);
}

long SR04::Distance() {
    long d = 0;
    _duration = 0;
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(25000);
    if (digitalRead(_echoPin) == HIGH) { // check if echo signal already high
        return MAX_DISTANCE;
    }
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(TRIG_PULSE_WIDTH);
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
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
        do {
            if ((++fetch) > FETCH_TIMEOUT) {
                return MAX_DISTANCE;
            }
            d = Distance();
        } while (d == MAX_DISTANCE);

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
