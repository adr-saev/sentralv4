/*
Beskrivelse: Alarm funksjoner for hjemmesentralen.
            Styrer kun logikken i alarm-systemet. Ingen visuelle endringer.

----------------------------------------------------
Funksjoner:
    - getSec()/getMin()/getHour(): Returnerer gjeldende nedtellingstid
    - getAlarmState(): Returnerer alarmstatus (aktiv/inaktiv)
    - oppdaterAlarm(): Oppdaterer nedtelling hver 1000 ms
    - alarmFinished(): Sjekker om alarmen har nådd 0
    - startStoppAlarm(): Starter eller stopper alarmen
    - endreTid(): Endrer mål-tid for alarmen
    - resetAlarm(): Tilbakestiller mål-tid til 0

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato: 09/03/2026
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

// oppdaterAlarm()
// Oppdaterer alarmnedtellingen hver 1000 ms hvis alarmen er aktiv.
// Dekrementerer sekunder, minutter og timer etter behov.
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

// alarmFinished()
// Sjekker om alarmen har nådd 0. Returnerer true og stopper alarmen hvis ferdig.
bool alarmFinished() {
  if (alarmState == 1 && actSec == 0 && actMin == 0 && actHour == 0) {
    alarmState = 0;
    Serial.print("Alarm er ferdig");
    return true;
  }
  return false;
}

// startStoppAlarm()
// Starter alarmen hvis den er stoppet og tid er satt, eller stopper den hvis den kjører.
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

// endreTid(type, verdi)
// Endrer mål-tid for alarmen (sekunder, minutter eller timer) med gitt verdi.
// Kan bare endres når alarmen ikke kjører. Wraps rundt ved grenser.
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

// resetAlarm()
// Tilbakestiller mål-tid for alarmen til 0 sekunder, minutter og timer.
void resetAlarm() {
  targetSec = 0;
  targetMin = 0;
  targetHour = 0;
}
