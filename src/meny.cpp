/*
Hovedkilde til menyene, styrer UI ( User Interface )



Skrevet av: Adrian Nesse
Dato: 25/01/2026
Version: 1.0
*/

//--------------------------------------------------
// Variabler:
extern float tempTMP[];
extern float tempDHT[];
extern float fuktDHT[];

//--------------------------------------------------

#include <Arduino.h>

#include "meny.h"
#include "funksjoner.h"
#include "lys.h"
#include "buttons.h"

extern LiquidCrystal lcd;

static MENY aktivMeny = HOVEDMENY;
static AKTIV_HOVED_MENY mainValg = TEMP;
static AKTIV_TEMP_MENY tempValg = SENSOR1;
static AKTIV_LYS_MENY lysValg = HOVEDROM;
static LYS_HOVEDROM l_hvd = ACTIVE_HVDROM;
static LYS_BAD l_bad = ACTIVE_BAD;

bool isStueAktiv = false;
bool isBadAktiv = false;
bool manuelLys = false; // false = fototransistor styrer

// settMeny()
// Funksjon brukt til og endre menyen til ønsket meny.
void settMeny(MENY nyMeny) { aktivMeny = nyMeny; }

// tegnMeny()
// Håndtere menyene.
void tegnMeny() {
  switch (aktivMeny) {
  case HOVEDMENY:
    tegn_HOVEDMENY();
    break;
  case TEMPRATUR_MENY:
    tegn_TEMPMENY();
    break;
  case LYS_MENY:
    tegn_LYSMENY();
    break;
  case LYS_MNY_HOVEDROM:
    tegn_LYSHOVED();
    break;
  case LYS_MNY_BAD:
    tegn_LYSBAD();
    break;
  }
}

// tegn_HOVEDMENY()
// Rendrer hovedmenyen med navigasjon mellom modus(TEMP, LYS, INSTILLINGER)
// Håndtere UP/DOWN for navigasjon og OK for å endre meny
void tegn_HOVEDMENY() {
  // tegn menyen
  lcd.setCursor(0, 0);
  lcd.print("Velg Modus       ");

  if (buttonClick(bUP)) {
    lcd.clear();
    mainValg = (AKTIV_HOVED_MENY)((int)mainValg + 1);
    if (mainValg > HJEM_MENY_INSTILLINGER) {
      mainValg = TEMP;
    }

    Serial.print("Sitter meny til: ");
    Serial.print(mainValg);
    Serial.print("\n");

    delay(200);
  }

  if (buttonClick(bDown)) {
    lcd.clear();
    mainValg = (AKTIV_HOVED_MENY)((int)mainValg - 1);

    if (mainValg < TEMP) {
      mainValg = HJEM_MENY_INSTILLINGER;
    }

    Serial.print("Sitter meny til: ");
    Serial.print(mainValg);
    Serial.print("\n");

    delay(200);
  }

  switch (mainValg) {
  case TEMP:
    lcd.setCursor(0, 1);
    lcd.print("> Tempratur     ");
    if (buttonClick(bOK)) {
      settMeny(TEMPRATUR_MENY);
      delay(200);
    }
    break;
  case DOOR_WINDOW:
    lcd.setCursor(0, 1);
    lcd.print("> DOOR & WINDOW");
    if (buttonClick(bOK)) { /*settMeny();*/
    }
    break;
  case LYS_HVDMNY:
    lcd.setCursor(0, 1);
    lcd.print("> Belysning     ");
    if (buttonClick(bOK)) {
      settMeny(LYS_MENY);
      delay(200);
    }
    break;

  case HJEM_MENY_INSTILLINGER:
    lcd.setCursor(0, 1);
    lcd.print("> Instillinger ");
    if (buttonClick(bOK)) { /*settMeny();*/
      delay(200);
    }
  }
}

// tegn_TEMPMENY()
// Rendrer tempratur menyen som viser tempratur, og ettervært luftfuktighet på
// bad Håndterer UP/DOWN inni temp menyene
void tegn_TEMPMENY() {

  serialTempFukt(tempTMP, tempDHT, fuktDHT, tlength, flength);

  if (buttonClick(bUP)) {
    lcd.clear();
    tempValg = (AKTIV_TEMP_MENY)((int)tempValg + 1);

    if (tempValg > SENSOR2) {
      tempValg = SENSOR1;
    }

    Serial.print("Viser temp sensor: ");
    Serial.print(tempValg);
    Serial.print("\n");
    delay(200);
  }
  if (buttonClick(bDown)) {
    lcd.clear();
    tempValg = (AKTIV_TEMP_MENY)((int)tempValg - 1);

    if (tempValg < SENSOR1) {
      tempValg = SENSOR2;
    }

    Serial.print("Viser temp sensor: ");
    Serial.print(tempValg);
    Serial.print("\n");
    delay(200);
  }
  if (buttonClick(bHome)) {
    lcd.clear();
    settMeny(HOVEDMENY);
  }

  switch (tempValg) {
  case SENSOR1:

    lcd.setCursor(0, 0);
    lcd.print("< TEMP TMP36   ");
    lcd.setCursor(0, 1);

    lcd.print("> ");
    lcd.print(tempTMP[0]);
    lcd.print("                ");

    break;

  case SENSOR2:
  
  lcd.setCursor(0, 0);
  lcd.print("< TEMP DHT     ");
  lcd.setCursor(0, 1);
    lcd.setCursor(0, 1);
    lcd.print("> ");
    lcd.print(tempDHT[0]);
    lcd.setCursor(8, 1);
    // lcd.print("Fukt: ");
    lcd.print(fuktDHT[0]);
    lcd.print("%");
    break;
  }
}

// tegn_LYSMENY()
// Rendrer lysmenyer, med navigering mellom modus (Hovedmeny /Stue, Bad, AKTIVEr
// ALLE LYS) Navigering med UP/DOWN og OK for å sitte meny til ønskede.
void tegn_LYSMENY() {

  if (buttonClick(bUP)) {
    lcd.clear();
    lysValg = (AKTIV_LYS_MENY)((int)lysValg + 1);
    if (lysValg > LYS_INSTILLINGER) {
      lysValg = HOVEDROM;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(lysValg);
    Serial.print("\n");

    delay(200);
  }
  if (buttonClick(bDown)) {
    lcd.clear();
    lysValg = (AKTIV_LYS_MENY)((int)lysValg - 1);
    if (lysValg < HOVEDROM) {
      lysValg = LYS_INSTILLINGER;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(lysValg);
    Serial.print("\n");
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("Lys Meny       ");
  switch (lysValg) {
  case HOVEDROM:
    lcd.setCursor(0, 1);
    lcd.print("> Hovedrom");

    if (buttonClick(bOK)) {
      settMeny(LYS_MNY_HOVEDROM);
      delay(200);
    }
    break;
  case BAD:
    lcd.setCursor(0, 1);
    lcd.print("> BAD");
    if (buttonClick(bOK)) {
      settMeny(LYS_MNY_BAD);
      delay(200);
    }
    break;
  case AKTIV_ALL:
    lcd.setCursor(0, 1);
    lcd.print("> AKTIVER ALT  ");

    if (buttonClick(bOK)) {
      isStueAktiv = !isStueAktiv;
      isBadAktiv = !isBadAktiv;

      delay(200);
    }
    break;
  case LYS_INSTILLINGER:
    lcd.setCursor(0, 1);
    lcd.print("Lys Innst.     ");
    /*if (buttonClick(bOK)) {
      settMeny(LYS_INNST_MENY);
      delay(200);
    }*/

    break;
  }
}

// tegn_LYSHOVED()
// Rendrer meny for lys på Stuen, med navigasjon til instillinger (aktiver stue,
// dimming) Navigering med UP/DOWN og OK For meny endring.

void tegn_LYSHOVED() {
  if (buttonClick(bUP)) {
    lcd.clear();
    l_hvd = (LYS_HOVEDROM)((int)l_hvd + 1);

    if (l_hvd > LYS_STYRING) {
      l_hvd = ACTIVE_HVDROM;
    }

    Serial.print("Sitter meny til: ");
    Serial.print(l_hvd);
    Serial.print("\n");

    delay(200);
  }
  if (buttonClick(bDown)) {
    lcd.clear();
    l_hvd = (LYS_HOVEDROM)((int)l_hvd - 1);

    if (l_hvd < ACTIVE_HVDROM) {
      l_hvd = LYS_STYRING;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(l_hvd);
    Serial.print("\n");

    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("HVDROM - INNST");

  switch (l_hvd) {
  case ACTIVE_HVDROM:
    lcd.setCursor(0, 1);
    lcd.print("AKTIV / DEAKTIV");

    if (buttonClick(bOK)) {
      isStueAktiv = !isStueAktiv;
      delay(200);
    }

    break;

  case LYS_STYRING:
    lcd.setCursor(0, 1);
    lcd.print("> LYS STYRING   ");

    if (buttonClick(bOK)) {
      manuelLys = !manuelLys;
      delay(200);
    }

    break;
  }
}

// tegn_LYSBAD()
// Rendrer meny for lys på Stuen, med navigasjon til instillinger (aktiver stue,
// manuell overstyring) Skal styres med phototransistor dersom manuell
// overstyring er deaktivert. Navigering med UP/DOWN og OK For meny endring.

void tegn_LYSBAD() {
  if (buttonClick(bUP)) {
    l_bad = (LYS_BAD)((int)l_bad + 1);
    if (l_bad > DIMMING) {
      l_bad = ACTIVE_BAD;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(l_bad);
    Serial.print("\n");

    delay(200);
  }
  if (buttonClick(bDown)) {
    l_bad = (LYS_BAD)((int)l_bad - 1);
    if (l_bad < ACTIVE_BAD) {
      l_bad = DIMMING;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(l_bad);
    Serial.print("\n");

    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("BAD - INNST  ");

  switch (l_bad) {
  case ACTIVE_BAD:
    lcd.setCursor(0, 1);
    lcd.print("AKTIV / DEAKTIV");

    if (buttonClick(bOK)) {
      isBadAktiv = !isBadAktiv;
      delay(200);
    }
    break;
  case DIMMING:
    lcd.setCursor(0, 1);
    lcd.print("> Manuel Styring");
    break;
  }
}