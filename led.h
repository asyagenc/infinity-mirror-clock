#ifndef LED_H
#define LED_H

#include <RTClib.h>

void showTimeOnLED(DateTime now);
void showTemperatureColor();
void DiskoMode();
void ChillMode();
void SpringMode();

#endif
