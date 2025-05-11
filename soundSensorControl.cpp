#include <Arduino.h>
#include <FastLED.h>
#include "globals.h"
#include "app_state.h"
#include "soundSensorControl.h"
void handleSoundSensor() {

  int soundVal = analogRead(soundPin);
  if (soundVal > soundThreshold) {
    soundDetected = true;
    soundDetectedTime = millis();
  }

  if (soundDetected) {
    FastLED.clear();
    FastLED.show();
    if (millis() - soundDetectedTime >= soundHoldDuration) {
      soundDetected = false;
    } else {
      return;  // Ses kesilmediği sürece diğer loop işleri yapılmaz
    }
  }
}
