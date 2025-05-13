#include <Arduino.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "globals.h"
#include "app_state.h"
#include "led.h"
#include "menu.h"  // updateMainMenu() için gerekli

// Harici tanımlı değişken ve fonksiyonlar:

#define NUM_LEDS 119

extern void waitForRelease();

// getClockwiseIndex fonksiyonu başka yerde tanımlıysa bildir:
extern int getClockwiseIndex(float value, float maxValue);


void showTimeOnLED(DateTime now) {
  int hour = now.hour() % 12;
  int minute = now.minute();
  int second = now.second();

  float hourWithMinute = hour + (minute / 60.0);

  int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
  int minuteIndex = getClockwiseIndex(minute, 60.0);
  int secondIndex = getClockwiseIndex(second, 60.0);

  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(30, 30, 30);
  leds[hourIndex]   = CRGB::Red;
  leds[minuteIndex] = CRGB::Green;
  leds[secondIndex] = CRGB::Blue;
  FastLED.show();
}


// Ortam sıcaklığına göre LED ayarlama fonksiyonu
void showTemperatureColor() {
  float temperature = rtc.getTemperature();
  DateTime now = rtc.now();

  // Sabit renkler (örnek)
  CRGB bgColor ;
  CRGB hourColor = CRGB::Purple;
  CRGB minColor = CRGB::White;
  CRGB secColor= CRGB::HotPink;

  // Sıcaklığa göre sadece saniye rengi değişir
  if (temperature >= 35.0) {
    bgColor = CRGB::Red;  
  } else if (temperature >= 18.0 && temperature <= 25.0) {
    bgColor = CRGB::Green;
  }  else {
    bgColor = CRGB::Blue;  // soğuksa mavi 
  }

  // Saat verisi
  int hour = now.hour() % 12;
  int minute = now.minute();
  int second = now.second();
  float hourWithMinute = hour + (minute / 60.0);

  // LED index hesapları
  int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
  int minuteIndex = getClockwiseIndex(minute, 60.0);
  int secondIndex = getClockwiseIndex(second, 60.0);

  // LED'leri doldur
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = bgColor;
  }

  leds[hourIndex]   = hourColor;
  leds[minuteIndex] = minColor;
  leds[secondIndex] = secColor;

  FastLED.show();
}


void DiskoMode() {
  static bool ledsOn = true;
  static unsigned long lastBlinkTime = 0;
  const unsigned long blinkInterval = 500;

  while (true) {
    DateTime now = rtc.now();

    int hour = now.hour() % 12;
    int minute = now.minute();
    int second = now.second();
    float hourWithMinute = hour + (minute / 60.0);

    int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
    int minuteIndex = getClockwiseIndex(minute, 60.0);
    int secondIndex = getClockwiseIndex(second, 60.0);

    // Blink kontrolü
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      ledsOn = !ledsOn;
      lastBlinkTime = currentMillis;
    }

    // Tüm LED'leri ayarla
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ledsOn ? CRGB(30, 30, 30) : CRGB::Black;
    }

    // Saat LED'lerini sabit renkle üzerine yaz
    leds[hourIndex]   = CRGB::Red;
    leds[minuteIndex] = CRGB::Green;
    leds[secondIndex] = CRGB::Blue;

    FastLED.show();

    // Buton basımı kontrolü (menüye dön)
    if (!digitalRead(swPin)) {
      waitForRelease();
      updateMainMenu();
      break;
    }

    delay(50); // çok kısa gecikme, animasyonu pürüzsüz yapar
  }
}



void ChillMode() {
  static bool ledsOn = true;
  static unsigned long lastBlinkTime = 0;
  const unsigned long blinkInterval = 1000;

  while (true) {
    DateTime now = rtc.now();

    int hour = now.hour() % 12;
    int minute = now.minute();
    int second = now.second();
    float hourWithMinute = hour + (minute / 60.0);

    int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
    int minuteIndex = getClockwiseIndex(minute, 60.0);
    int secondIndex = getClockwiseIndex(second, 60.0);

    // Blink kontrolü
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      ledsOn = !ledsOn;
      lastBlinkTime = currentMillis;
    }

    // Tüm LED'leri ayarla
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = ledsOn ? CRGB(30, 30, 30) : CRGB::Black;
    }

    // Saat LED'lerini sabit renkle üzerine yaz
    leds[hourIndex]   = CRGB::Red;
    leds[minuteIndex] = CRGB::Green;
    leds[secondIndex] = CRGB::Blue;

    FastLED.show();

    // Buton basımı kontrolü (menüye dön)
    if (!digitalRead(swPin)) {
      waitForRelease();
      updateMainMenu();
      break;
    }

    delay(50); // çok kısa gecikme, animasyonu pürüzsüz yapar
  }
}



void SpringMode() {
  lcd.clear();
  lcd.print("Spring Mode!");
  delay(500);

  int waveCenter = 0;
  const int waveWidth = 110;

  while (true) {
    DateTime now = rtc.now();

    int hour = now.hour() % 12;
    int minute = now.minute();
    int second = now.second();
    float hourWithMinute = hour + (minute / 60.0);
    int hourIndex   = getClockwiseIndex(hourWithMinute, 12.0);
    int minuteIndex = getClockwiseIndex(minute, 60.0);
    int secondIndex = getClockwiseIndex(second, 60.0);

    for (int i = 0; i < NUM_LEDS; i++) {
      if (i == hourIndex) {
        leds[i] = CRGB::Red;
      } else if (i == minuteIndex) {
        leds[i] = CRGB::Green;
      } else if (i == secondIndex) {
        leds[i] = CRGB::Blue;
      } else {
  int dist = (waveCenter - i + NUM_LEDS) % NUM_LEDS;

  if (dist <= waveWidth) {
    float ratio = (float)dist / waveWidth;
    float wave = 0.5 * (cos(ratio * PI) + 1.0); // Smooth geçiş
    uint8_t brightness = wave * 200 + 30;      // 30–230 arası parlaklık
    leds[i] = CRGB(brightness, brightness, brightness);
  } else {
    leds[i] = CRGB::Black;
  }
}
    }

    FastLED.show();
    delay(30);

    // Saat yönünün tersine kaydır
    waveCenter = (waveCenter - 1 + NUM_LEDS) % NUM_LEDS;

    if (!digitalRead(swPin)) {
      waitForRelease();
      updateMainMenu();
      return;
    }
  }
}
