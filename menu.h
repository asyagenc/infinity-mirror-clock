#ifndef MENU_H
#define MENU_H
#include <RTClib.h> 

void updateMainMenu();          // Ana menüyü ekranda gösterir
void showAlarmSubMenu();        // Alarm alt menüsünü gösterir
void showBrightnessMenu();      // Parlaklık alt menüsünü gösterir
void showAnimationSubMenu();    // Animasyon alt menüsünü gösterir
void handleInactivityTimeout(DateTime now);
#endif
