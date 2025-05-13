#include "globals.h"

// ---- Donanım Pinleri ----
const int clkPin = 2;
const int dtPin = 3;
const int swPin = 4;
const int buzzerPin = 8;
const int soundPin = A0;

// ---- LED ----
CRGB leds[NUM_LEDS];
int brightnessLevel = 60;
int brightnessOption = 2;
int brightnessSubMenu = 1;
bool ledsOn = true;

// ---- LCD ve RTC ----
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// ---- Menü ----
AppState currentState = STATE_MAIN_MENU;
int mainMenu = 1;
unsigned long lastInteractionTime = 0;
const unsigned long inactivityTimeout = 15000;

bool temperatureColor = false;
bool waitForTempExit = false;
bool animationMenu = false;
int animationSubMenu = 1;
bool brightnessMenu = false;

int alarmSubMenu = 1;
const int MAX_ALARMS = 5;
Alarm alarms[MAX_ALARMS];
int alarmCount = 0;


// ---- Rotary Encoder ----
int lastClkState = 0;

// ---- Ses Sensörü ----
const int soundThreshold = 300;
const unsigned long debounceDuration = 500;
unsigned long lastSoundTime = 0;