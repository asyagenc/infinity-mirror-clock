#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <RTClib.h>
#include "globals.h"
#include "app_state.h"
#include "menu.h"
#include "led.h"


extern void waitForRelease();
extern void updateMainMenu();
extern void showCurrentTime();



void updateMainMenu() {
  lcd.clear();
  switch (mainMenu) {
    case 1:
      lcd.print(">Alarm");
      lcd.setCursor(0, 1);
      lcd.print(" Parlaklik");
      break;
    case 2:
      lcd.print(">Parlaklik");
      lcd.setCursor(0, 1);
      lcd.print(" Renk Degistir");
      break;
    case 3:
      lcd.print(">Renk Degistir");
      lcd.setCursor(0, 1);
      lcd.print(" Animasyon");
      break;
    case 4:
      lcd.print(">Animasyon");
      lcd.setCursor(0, 1);
      lcd.print(" Saat");
      break;
    case 5:
      lcd.print(">Saat");
      break;
  }
}

void showAlarmSubMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alarm Menu:");
  lcd.setCursor(0, 1);
  lcd.print("Kur / Listele");
}

void showBrightnessMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Parlaklik:");
  lcd.setCursor(0, 1);
  lcd.print(">1  2  3"); 
}

void showAnimationSubMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Animasyon:");
  lcd.setCursor(0, 1);
  lcd.print("Disko / Chill / Spring");
}

void handleTemperatureMenu(DateTime now) {
  if (temperatureColorMenu) {
    showTemperatureColor(now);

    if (!digitalRead(swPin)) {
      waitForRelease();
      temperatureColorMenu = false;
      updateMainMenu();
    }

    delay(500);
  }
}

void handleInactivityTimeout(DateTime now){
   if (currentState == STATE_MAIN_MENU && millis() - lastInteractionTime > inactivityTimeout) {
  currentState = STATE_SHOW_TIME;
  showCurrentTime();  // LCD'de saat göster
  }


}
 