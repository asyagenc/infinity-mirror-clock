#ifndef ALARM_H
#define ALARM_H
#include <RTClib.h>


struct Alarm {
  int hour;
  int minute;
  bool active;
};

extern Alarm alarms[];
extern int alarmCount;

void alarmSetup();
void showAlarmList();
void confirmDelete(int index);
void checkAlarms(DateTime now);

#endif
