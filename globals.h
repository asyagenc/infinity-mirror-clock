// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H
#define LED_PIN 6
#define NUM_LEDS 119



#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <FastLED.h>
#include "app_state.h"
#include "alarm.h"

// ---- Donanım Pinleri ----
extern const int clkPin;
extern const int dtPin;
extern const int swPin;
extern const int buzzerPin;
extern const int soundPin;

// ---- LED ve Parlaklık ----
extern CRGB leds[];
extern int brightnessLevel;
extern int brightnessOption;
extern int brightnessSubMenu;
extern bool ledsOn;

// ---- LCD ----
extern LiquidCrystal_I2C lcd;

// ---- Saat ----
extern RTC_DS3231 rtc;

// ---- Menü ----
extern AppState currentState;
extern int mainMenu;
extern unsigned long lastInteractionTime;
extern const unsigned long inactivityTimeout;
extern bool alarmMenu;

extern bool temperatureColor;
extern bool waitForTempExit;
extern bool animationMenu;
extern int animationSubMenu;
extern bool brightnessMenu;



extern int alarmSubMenu;
extern const int MAX_ALARMS;
extern Alarm alarms[];
extern int alarmCount;

// ---- Rotary Encoder ----
extern int lastClkState;

// ---- Ses Sensörü ----
extern const int soundThreshold;
extern const unsigned long debounceDuration;
extern unsigned long lastSoundTime;


#endif
