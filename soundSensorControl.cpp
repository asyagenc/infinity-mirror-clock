#include <Arduino.h>
#include <FastLED.h>
#include "globals.h"
#include "soundSensorControl.h"

void handleSoundSensor() {
  int soundVal = analogRead(A0);  // Ses sensörü pinin buysa
  unsigned long currentTime = millis();

  if (soundVal > soundThreshold) {
    if (currentTime - lastSoundTime > debounceDuration) {
      lastSoundTime = currentTime;
      ledsOn = !ledsOn;

      Serial.print("Yüksek ses algılandı - Yeni durum: ");
      Serial.println(ledsOn ? "AÇIK" : "KAPALI");

      if (ledsOn) {
        fill_solid(leds, NUM_LEDS, CRGB::Blue);  // Açıkken renk
        FastLED.setBrightness(100);
        FastLED.show();
      } else {
        FastLED.clear();  // Tüm LED’leri kapat
        FastLED.setBrightness(0); // Parlaklığı sıfırla (garanti)
        FastLED.show();
      }
    }
  }
}
