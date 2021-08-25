#include "SR04.h"

/*!
* This example uses an SR04 ultrasonic devices and prints the distance 
*
* Have fun
* mrRobot@web.de
*/


#define TRIGGER_PIN 7

#define ECHO_PIN 15

SR04 sr04 = SR04(ECHO_PIN, TRIGGER_PIN);

#define LED_PIN LED1

void setup() {
    Serial.begin(57600);
    delay(1000);
    sr04.init();
    pinMode(LED_PIN, OUTPUT);
    Serial.println("******************************************");
    Serial.println("Ultraschall - TEST SR04");
    Serial.println("******************************************");
}

void loop() {
	doQuickPing();
    delay(250);
	doAvgDefaultPing();
    delay(250);
	doAvgMorePings(15,10);
    delay(250);
    toggleLED(1, 50);
}

void doQuickPing() {
    long distance;
	// get distance in cm
    distance = sr04.Distance();
    Serial.print("doQuickPing: ");
    Serial.print(distance);
    Serial.println("cm");
}

void doAvgDefaultPing() {
    long distance;
	// get average distance, use default parameter
    distance = sr04.DistanceAvg();
    Serial.print(" doAvgDefaultPing: ");
    Serial.print(distance);
    Serial.println("cm");
}

/*!
* use DistanceAvg for measurment. Do not use default values
* 
* use 15ms delay per ping
* use 10pings
*/
void doAvgMorePings(int wait, int pings) {
    long distance;
	// get left average distance
    distance = sr04.DistanceAvg(wait,pings);
    Serial.print(" doAvgMorePings: delay ");
    Serial.print(wait);
    Serial.print(", pings ");
    Serial.print(pings);
    Serial.print(", ");
    Serial.print(distance);
    Serial.println("cm");
}

/**
* Blink on board led (on PB2)
* \param count number of blinks
* \param ms delay between blink
* Return:
*/
void toggleLED(byte count, int ms) {
    for (int x = 0; x < count; x++) {
        digitalWrite(LED_PIN, LOW);
        delay(ms);
        digitalWrite(LED_PIN, HIGH);
        delay(ms);
    }
}