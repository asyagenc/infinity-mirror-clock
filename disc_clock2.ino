#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <FastLED.h>
#define LED_PIN 6
#define NUM_LEDS 119


#include "globals.h"
#include "app_state.h"
#include "menu.h"
#include "alarm.h"
#include "led.h"
#include "soundSensorControl.h"

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  // Bunu bir defaya mahsus olarak setup() içinde yaz, sonra kaldır:
  //rtc.adjust(DateTime(2025, 5, 17, 20, 34, 0));  // Örnek doğru tarih/saat

  lcd.init();
  lcd.backlight();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightnessLevel);
  FastLED.clear();
  FastLED.show();

  pinMode(clkPin, INPUT);
  pinMode(dtPin, INPUT);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(soundPin, INPUT);
  digitalWrite(buzzerPin, LOW);

  lastClkState = digitalRead(clkPin);
  updateMainMenu();
}

// ---------------- LOOP ----------------
void loop() {
  DateTime now = rtc.now();

  checkAlarms(now);
  handleRotary();
  handleButton();
  handleSoundSensor();
  showTimeOnLED(now);  

  delay(200);  
}

// ---------------- LED Konum Hesabı ----------------
int getClockwiseIndex(float value, float maxValue) {
  float ledsPerUnit = NUM_LEDS / maxValue;
  int offset = round(value * ledsPerUnit);
  int index = (NUM_LEDS - offset) % NUM_LEDS;
  return index;
}

// ---------------- Buton Utility ----------------
void waitForRelease() {
  while (!digitalRead(swPin));
  delay(150);
}

void waitForNewPress() {
  while (!digitalRead(swPin));
  while (digitalRead(swPin));
  delay(150);
}

// ---------------- Ana Menü Execute ----------------
void execute() {
  lcd.clear();
  switch (mainMenu) {
    case 1: alarmMenu = true; showAlarmSubMenu(); break;
    case 2: brightnessMenu = true; showBrightnessMenu(); break;
    case 3: handleTemperature(); break;
    case 4: animationMenu = true; showAnimationSubMenu(); break;
    case 5: showCurrentTime(); break;
  }
}

// ---------------- Rotary ----------------
void handleRotary() {
  int newClk = digitalRead(clkPin);
  if (newClk != lastClkState && newClk == LOW) {
    bool clockwise = digitalRead(dtPin) != newClk;
    lastInteractionTime = millis();

    switch (currentState) {
      case STATE_MAIN_MENU:
        mainMenu += clockwise ? 1 : -1;
        if (mainMenu < 1) mainMenu = 5;
        if (mainMenu > 5) mainMenu = 1;
        updateMainMenu();
        break;

      case STATE_ALARM_MENU:
        alarmSubMenu += clockwise ? 1 : -1;
        if (alarmSubMenu < 1) alarmSubMenu = 2;
        if (alarmSubMenu > 2) alarmSubMenu = 1;
        showAlarmSubMenu();
        break;

      case STATE_ANIMATION_MENU:
        animationSubMenu += clockwise ? 1 : -1;
        if (animationSubMenu < 1) animationSubMenu = 3;
        if (animationSubMenu > 3) animationSubMenu = 1;
        showAnimationSubMenu();
        break;

      case STATE_BRIGHTNESS_MENU:
        brightnessSubMenu += clockwise ? 1 : -1;
        if (brightnessSubMenu < 1) brightnessSubMenu = 3;
        if (brightnessSubMenu > 3) brightnessSubMenu = 1;
        showBrightnessMenu();
        break;

      default:
        break;
    }

    delay(150);
  }
  lastClkState = newClk;
}

// ---------------- Buton ----------------
void handleButton() {
  if (!digitalRead(swPin)) {
    waitForRelease();
    lastInteractionTime = millis();

    switch (currentState) {
      case STATE_MAIN_MENU:
        switch (mainMenu) {
          case 1: currentState = STATE_ALARM_MENU; showAlarmSubMenu(); break;
          case 2: currentState = STATE_BRIGHTNESS_MENU; showBrightnessMenu(); break;
          case 3: currentState = STATE_TEMPERATURE_COLOR; handleTemperature(); break;
          case 4: currentState = STATE_ANIMATION_MENU; showAnimationSubMenu(); break;
          case 5: currentState = STATE_SHOW_TIME; showCurrentTime(); break;
        }
        break;

      case STATE_ALARM_MENU:
        if (alarmSubMenu == 1) alarmSetup();
        else showAlarmList();
        currentState = STATE_MAIN_MENU;
        updateMainMenu();
        break;

      case STATE_ANIMATION_MENU:
        if (animationSubMenu == 1) DiskoMode();
        else if (animationSubMenu == 2) ChillMode();
        else if (animationSubMenu == 3) SpringMode();
        currentState = STATE_MAIN_MENU;
        updateMainMenu();
        break;

      case STATE_BRIGHTNESS_MENU:
        brightnessLevel = (brightnessSubMenu == 1) ? 20 :
                          (brightnessSubMenu == 2) ? 60 : 255;
        FastLED.setBrightness(brightnessLevel);
        currentState = STATE_MAIN_MENU;
        updateMainMenu();
        break;

      case STATE_TEMPERATURE_COLOR:
        currentState = STATE_MAIN_MENU;
        updateMainMenu();
        break;

      case STATE_SHOW_TIME:
        currentState = STATE_MAIN_MENU;
        updateMainMenu();
        break;
    }
  }
}

// ---------------- Saat Göstergesi ----------------
void showCurrentTime() {
  while (true) {
    DateTime now = rtc.now();
    lcd.clear();
    lcd.setCursor(0, 0);
    if (now.day() < 10) lcd.print("0");
    lcd.print(now.day()); lcd.print("/");
    if (now.month() < 10) lcd.print("0");
    lcd.print(now.month()); lcd.print("/");
    lcd.print(now.year()); lcd.print(" ");
    if (now.hour() < 10) lcd.print("0");
    lcd.print(now.hour()); lcd.print(":");
    if (now.minute() < 10) lcd.print("0");
    lcd.print(now.minute());

    delay(500);
    if (!digitalRead(swPin)) {
      waitForRelease();
      break;
    }
  }
  updateMainMenu();
}
void handleTemperature() {
  while (true) {
    DateTime now = rtc.now();
    float temperature = rtc.getTemperature();

    // Saat verisi
    int hour = now.hour() % 12;
    int minute = now.minute();
    int second = now.second();
    float hourWithMinute = hour + (minute / 60.0);

    // LED renkleri
    CRGB bgColor;
    CRGB hourColor = CRGB::Purple;
    CRGB minColor = CRGB::White;
    CRGB secColor = CRGB::Purple;

    if (temperature >= 35.0) {
      bgColor = CRGB::Red;
    } else if (temperature >= 20.0 && temperature <= 30.0) {
      bgColor = CRGB::Green;
    } else if (temperature >= 10.0 && temperature < 20.0) {
      bgColor = CRGB::Blue;
    } else {
      bgColor = CRGB::Yellow;
    }

    // Saat LED index'leri
    int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
    int minuteIndex = getClockwiseIndex(minute, 60.0);
    int secondIndex = getClockwiseIndex(second, 60.0);

    // LED'leri güncelle
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = bgColor;
    leds[hourIndex] = hourColor;
    leds[minuteIndex] = minColor;
    leds[secondIndex] = secColor;
    FastLED.show();

    // Buton kontrolü
    if (!digitalRead(swPin)) {
      waitForRelease();
      updateMainMenu();
      break;
    }

    delay(200); // saniyelik akış için yeterli
  }
}

