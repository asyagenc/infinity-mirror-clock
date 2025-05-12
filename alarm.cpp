
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "app_state.h"
#include "alarm.h"
#include "globals.h"
#include <RTClib.h>

extern void waitForRelease();
extern void waitForNewPress();


void alarmSetup() {
  int stage = 0;
  int currentHour = 0;
  int currentMinute = 0;
  int lastClk = digitalRead(clkPin);

  lcd.clear();
  lcd.print("Alarm Kur:");

  while (true) {
    lcd.setCursor(0, 1);
    if (stage == 0) {
      lcd.print("Saat: ");
      if (currentHour < 10) lcd.print("0");
      lcd.print(currentHour);
    } else {
      lcd.print("Dakika: ");
      if (currentMinute < 10) lcd.print("0");
      lcd.print(currentMinute);
    }

    int clkState = digitalRead(clkPin);
    if (clkState != lastClk && clkState == LOW) {
      if (digitalRead(dtPin) != clkState) {
        if (stage == 0)
          currentHour = (currentHour + 1) % 24;
        else
          currentMinute = (currentMinute + 1) % 60;
      } else {
        if (stage == 0)
          currentHour = (currentHour + 23) % 24;
        else
          currentMinute = (currentMinute + 59) % 60;
      }
      delay(150);
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    lastClk = clkState;

    if (!digitalRead(swPin)) {
      waitForRelease();
      stage++;
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }

    if (stage > 1) break;
  }

  if (alarmCount < MAX_ALARMS) {
    alarms[alarmCount].hour = currentHour;
    alarms[alarmCount].minute = currentMinute;
    alarms[alarmCount].active = true;
    alarmCount++;
  }

  lcd.clear();
  lcd.print("Alarm Kaydedildi");

  waitForNewPress(); // Çıkmak için basılmasını bekle
}

void checkAlarms(DateTime now) {
  for (int i = 0; i < alarmCount; i++) {
    if (alarms[i].active && alarms[i].hour == now.hour() && alarms[i].minute == now.minute()) {
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      alarms[i].active = false;
    }
  }
}

void confirmDelete(int index) {
  int option = 0; // 0 = Evet, 1 = Hayır
  int lastClk = digitalRead(clkPin);
    int lastOption = -1;

  while (true) {
    if(option != lastOption){
      lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Alarmi sil?");
    lcd.setCursor(0, 1);
    lcd.print(option == 0 ? ">Evet Hayir" : " Evet >Hayir");
    lastOption = option;
    }
    

    // Rotary kontrolü
    int clkState = digitalRead(clkPin);
    if (clkState != lastClk && clkState == LOW) {
      if (digitalRead(dtPin) != clkState)
        option = (option + 1) % 2;
      else
        option = (option + 1) % 2;

      delay(150);
    }
    lastClk = clkState;

    // Seçim yapılırsa alarm için
    if (!digitalRead(swPin)) {
      waitForRelease();

      if (option == 0) {
        // Evet: alarmı sil
        for (int i = index; i < alarmCount - 1; i++) {
          alarms[i] = alarms[i + 1];
        }
        alarmCount--;

        lcd.clear();
        lcd.print("Alarm silindi");
        delay(1000);
      } else {
        // Hayır: bir şey yapma
        lcd.clear();
        lcd.print("Iptal edildi");
        delay(1000);
      }
      break;
    }
  }
}

void showAlarmList() {
  if (alarmCount == 0) {
    lcd.clear();
    lcd.print("Alarm yok");
    waitForRelease();
    return;
  }

  int selected = 0;
  int viewStart = 0;
  int lastSelected = -1;
  int lastViewStart = -1;
  int lastClk = digitalRead(clkPin);

  while (true) {
    // Ekran sadece değiştiğinde güncellenir
    if (selected != lastSelected || viewStart != lastViewStart) {
      lcd.clear();
      for (int i = 0; i < 2 && (viewStart + i) < alarmCount; i++) {
        lcd.setCursor(0, i);
        if (viewStart + i == selected) lcd.print(">");
        else lcd.print(" ");
        lcd.print(viewStart + i + 1);
        lcd.print(". ");
        lcd.print(alarms[viewStart + i].hour);
        lcd.print(":");
        if (alarms[viewStart + i].minute < 10) lcd.print("0");
        lcd.print(alarms[viewStart + i].minute);
      }
      lastSelected = selected;
      lastViewStart = viewStart;
    }

    // Rotary encoder kontrolü
    int clkState = digitalRead(clkPin);
    if (clkState != lastClk && clkState == LOW) {
      if (digitalRead(dtPin) != clkState) {
        selected = (selected + 1) % alarmCount;
      } else {
        selected = (selected + alarmCount - 1) % alarmCount;
      }

      // Kaydırma kontrolü
      if (selected < viewStart) viewStart = selected;
      else if (selected >= viewStart + 2) viewStart = selected - 1;

      delay(150);
    }
    lastClk = clkState;

    // Buton basımı (kısa / uzun)
    if (!digitalRead(swPin)) {
      unsigned long pressStart = millis();
      while (!digitalRead(swPin));
      unsigned long pressDuration = millis() - pressStart;

      if (pressDuration < 600) {
        break;
      } else {
        confirmDelete(selected);
        if(alarmCount == 0 ){
          lcd.clear();
          lcd.print("Alarm Yok");
          delay(1000);
          break;

        }
        if (selected>= alarmCount) selected = alarmCount-1;
        if (viewStart > 0 && selected < viewStart) viewStart--;
       
          lastSelected = -1; // ekranı tekrar çiz
          lastViewStart = -1;
        }
      }
    }
  
}

