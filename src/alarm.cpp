/*
Beskrivelse: Alarm funksjoner for hjemmesentaralen. 
            Styrere kun Logikken i alarm systemet. Ingen Visuell endringer.
            
----------------------------------------------------
Funksjoner:
    - Endre tid
    - oppdatere Alarm
    - Styre start og Stopp av Alarm
    - Reset alarm
    - Fortelle om alarm er ferdig

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato:20/01/2026
Version: 1.0


----------------------------------------------------
POTENSIELLE FEIL:
    - 
    -


--------------------------------------------------*/


#include <Arduino.h>
#include "alarm.h"

static int actSec, actMin, actHour;
static int targetSec, targetMin, targetHour;
static int alarmState = 0;
static unsigned long targetTime;

int getSec() {
  return actSec;
}
int getMin() {
  return actMin;
}
int getHour() {
  return actHour;
}

int getAlarmState() {
  return alarmState;
}

void oppdaterAlarm() {
  unsigned long actTime = millis();

  if (alarmState == 1 && actTime - targetTime >= 1000) {
    targetTime = actTime;
    actSec--;

    if (actSec < 0) {
      actSec = 59;
      actMin--;

      if (actMin < 0) {
        actMin = 59;
        actHour--;
      }
    }
  }
}

bool alarmFinished() {
  if (alarmState == 1 && actSec == 0 && actMin == 0 && actHour == 0) {
    alarmState = 0;
    Serial.print("Alarm er ferdig");
    return true;
  }
  return false;
}

void startStoppAlarm() {
  if (alarmState == 0 &&
      (targetHour != 0 || targetMin != 0 || targetSec != 0)) {
    actSec = targetSec;
    actMin = targetMin;
    actHour = targetHour;
    alarmState = 1;
  } else if (alarmState == 1) {
    alarmState = 0;
  }
}

void endreTid(int type, int verdi) {
  if (alarmState == 0) {
    switch (type) {
    case 0:
      targetSec += verdi;
      if (targetSec >= 60) { targetSec = 0; }
      if (targetSec < 0) { targetSec = 59; }
      break;

    case 1:
      targetMin += verdi;
      if (targetMin >= 60) { targetMin = 0; }
      if (targetMin < 0) { targetMin = 59; }
      break;
    case 2:
      targetHour += verdi;
      if (targetHour >= 24) { targetHour = 0; }
      if (targetHour < 0) { targetHour = 23; }
      break;
    }
  }
}
void resetAlarm() {
  targetSec = 0;
  targetMin = 0;
  targetHour = 0;
}
