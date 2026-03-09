
/*
 sentralv4.ino

 Entrypunkt for systemet.
 - `setup()` initialiserer LCD, Serial og underliggende moduler (knapper, lys,
   osv.).
 - `loop()` bør kalle `tegnMeny()` og andre periodiske oppgaver som
   `oppdaterAlarm()` og sensoravlesning.
 - Hold hovedlogikk lett for å sikre jevn UI-oppdatering.

 Funksjoner/ansvarsfordeling:
  - LCD/meny: `meny.cpp`/`meny.h`
  - Hardware-helpers: `funksjoner.cpp`
  - Knappelogikk: `buttons.cpp`/`buttons.h`
  - Lys: `lys.cpp`/`lys.h`
  - Alarm: `alarm.cpp`/`alarm.h`
*/

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "src/meny.h"
#include "src/funksjoner.h"
#include "src/buttons.h"
#include "src/lys.h"
#include <DHT.h>

#define DHT11pin 45
#define DHTTYPE DHT11
LiquidCrystal lcd(53, 52, 51, 50, 49, 48);
DHT dht(DHT11pin, DHTTYPE);
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//--------------------------------------------------
// Vaiabler:
float tempTMP[tlength];
float fuktDHT[flength];
float tempDHT[flength];

//--------------------------------------------------

void setup() {
  // Starter LCD skjerm
  lcd.begin(53, 43);
  //lcd.begin(12, 2);
  Serial.begin(9600);
  dht.begin();

  pinMode(buzzer, OUTPUT);
  pinMode(foto_transistor_STUE, INPUT);
  initButtons();
  initlys();
  delay(1500);
  //settMeny(LYS_MNY_BAD);
}

void loop() {
  //buttonHolding(33) ? Serial.println("TRUE") : Serial.println("FALSE");
  //isDimmed == true ? Serial.println("TRUE") : Serial.println("FALSE");
  
  tegnMeny();  // Oppdaterer LCD-meny basert på aktiv menytilstand
  
  readDHT11(fuktDHT, tempDHT, flength);  // Leser fuktighet og temperatur fra DHT11-sensor
  readTMP36(TMPpin, tempTMP, tlength);   // Leser temperatur fra TMP36-sensor
  
  oppdaterPhoto();  // Oppdaterer lysstyrke for stue basert på fototransistor
  oppdaterLys();    // Aktiverer/deaktiverer lys for stue og bad basert på flagg
  lesSerial();      // Behandler kommandoer mottatt via Serial for debugging/testing
}