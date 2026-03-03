/*
 funksjoner.h

 Generelle hjelpefunksjoner og eksterne variabler som brukes på tvers
 av meny- og hardware-moduler.

 Eksportert:
 - eksterne enum-variabler som styrer menyvalg og fargeinnstillinger.
 - tempAvlesning(): leser analoge temperatur-sensorer inn i `tempVal[]`.
 - serialTemp(): periodisk sending av temperaturer over Serial for debugging.
 - lesSerial(): tolker kommandoer mottatt via Serial.
*/

#ifndef FUNKSJONER_H
#define FUNKSJONER_H

#include <Arduino.h>
#include "lys.h"

extern AKTIV_TEMP_MENY tempValg;
extern AKTIV_HOVED_MENY mainValg;
extern AKTIV_LYS_MENY lysValg;

const int TMPpin[] = {A0 /*A2*/};
const int tlength = sizeof(TMPpin) / sizeof(TMPpin[0]);

const int flength = 1;

extern int* _STYRKE_STUE;


void readTMP36(const int TMPpin[], float tempTMP[], int tlength);
void readDHT11(float fuktDHT[], float tempDHT[], int flength);
void serialTempFukt(float tempTMP[], float tempDHT[], float fuktDHT[],
                    int tlength, int flength);
//void serialLys(lys_gruppe gruppe);
void lesSerial();
#endif