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

extern int _STYRKE_BAD;
extern bool isDimmed;

bool isStueAktiv = false;
bool isBadAktiv = false;
bool manuelLys = false;

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
static RST_INST rst = RST_ALL;
static RST_YN rst_yn = RST_NO;

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
  case RESET_INSTILLINGER:
    tegn_RST_INST();
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
    if (mainValg > RESET_ALL) {
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
      mainValg = RESET_ALL;
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
  case LYS_HVDMNY:
    lcd.setCursor(0, 1);
    lcd.print("> Belysning     ");
    if (buttonClick(bOK)) {
      settMeny(LYS_MENY);
      delay(200);
    }
    break;

  case RESET_ALL:
    lcd.setCursor(0, 1);
    lcd.print("> RESET ALL    ");
    if (buttonClick(bOK)) {
      settMeny(RESET_INSTILLINGER);
      rst = RST_ALL;
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
    delay(200);
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
    lcd.print("%   ");
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
    if (lysValg > AKTIV_ALL) {
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
      lysValg = AKTIV_ALL;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(lysValg);
    Serial.print("\n");
    delay(200);
  }
  if (buttonClick(bHome)) {
    lcd.clear();
    settMeny(HOVEDMENY);
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("Lys Meny       ");
  switch (lysValg) {
  case HOVEDROM:
    lcd.setCursor(0, 1);
    lcd.print("> Hovedrom     ");

    if (buttonClick(bOK)) {
      settMeny(LYS_MNY_HOVEDROM);
      delay(200);
    }
    break;
  case BAD:
    lcd.setCursor(0, 1);
    lcd.print("> BAD         ");
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
  }
}

// tegn_LYSHOVED()
// Rendrer meny for lys på Stuen, med navigasjon til instillinger (aktiver stue,
// dimming) Navigering med UP/DOWN og OK For meny endring.

void tegn_LYSHOVED() {
  if (buttonClick(bUP)) {
    lcd.clear();
    l_hvd = (LYS_HOVEDROM)((int)l_hvd + 1);

    if (l_hvd > STUE_RESET) {
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
      l_hvd = STUE_RESET;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(l_hvd);
    Serial.print("\n");

    delay(200);
  }
  if (buttonClick(bHome)) {
    lcd.clear();
    settMeny(LYS_MENY);
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("HVDROM - INST  ");

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
  case STUE_RESET:
    lcd.setCursor(0, 1);
    lcd.print("> Reset Inst   ");

    if (buttonClick(bOK)) {
      lcd.clear();
      settMeny(RESET_INSTILLINGER);
      rst = RST_LYS_STUE;
      rst_yn = RST_NO;

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

  int dimming_prosent;
  if (buttonClick(bUP)) {
    l_bad = (LYS_BAD)((int)l_bad + 1);
    if (l_bad > BAD_RESET) {
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
      l_bad = BAD_RESET;
    }
    Serial.print("Sitter meny til: ");
    Serial.print(l_bad);
    Serial.print("\n");

    delay(200);
  }
  if (buttonClick(bHome)) {
    lcd.clear();
    settMeny(LYS_MENY);
    delay(200);
  }

  lcd.setCursor(0, 0);
  lcd.print("BAD - INNST    ");

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

    dimming_prosent = map(_STYRKE_BAD, 0, 255, 0, 100);

    lcd.setCursor(0, 1);
    lcd.print("> DIM LYS: ");
    lcd.print(dimming_prosent);
    lcd.print("%    ");

    if (isBadAktiv && buttonHolding(bOK)) {
      isDimmed = true;
      lysDimming(_LYS_BAD, _STYRKE_BAD, dimme_retning);
    }
    break;
  case BAD_RESET:
    lcd.setCursor(0, 1);
    lcd.print("> Reset Inst   ");

    if (buttonClick(bOK)) {
      lcd.clear();
      settMeny(RESET_INSTILLINGER);
      rst = RST_LYS_BAD;
      rst_yn = RST_NO;

      delay(200);
    }
    break;
  }
}
void tegn_RST_INST() {

  if (buttonClick(bUP)) {
    lcd.clear();
    rst_yn = (RST_YN)((int)rst_yn + 1);
    if (rst_yn > RST_YES) {
      rst_yn = RST_NO;
    }
    delay(200);
    Serial.print("Bytter Meny");
  }
  if (buttonClick(bDown)) {
    lcd.clear();
    rst_yn = (RST_YN)((int)rst_yn - 1);
    if (rst_yn < RST_NO) {
      rst_yn = RST_YES;
    }
    delay(200);
    Serial.print("Bytter Meny");
  }

  switch (rst) {
  case RST_ALL:
    lcd.setCursor(0, 0);
    lcd.print("Reset alt?     ");

    switch (rst_yn) {
    case RST_NO:

      if (buttonClick(bOK)) {
        lcd.clear();
        settMeny(HOVEDMENY);
        delay(200);
      }

      lcd.setCursor(0, 1);
      lcd.print("> NO      YES  ");
      break;
    case RST_YES:
      if (buttonClick(bOK)) {
        lcd.clear();
        isBadAktiv = false;
        isStueAktiv = false;
        manuelLys = true;
        isDimmed = false;

        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("ALT RESETTET");
        delay(1500);

        settMeny(HOVEDMENY);
        mainValg = TEMP;

        delay(200);
      }

      lcd.setCursor(0, 1);
      lcd.print(" NO     > YES  ");
      break;
    }
    break;
  case RST_LYS_STUE:
    lcd.setCursor(0, 0);
    lcd.print("Reset lys stue?");

    switch (rst_yn) {
    case RST_NO:

      if (buttonClick(bOK)) {
        lcd.clear();
        settMeny(LYS_MNY_HOVEDROM);
        delay(200);
      }

      lcd.setCursor(0, 1);
      lcd.print("> NO      YES  ");
      break;
    case RST_YES:

      if (buttonClick(bOK)) {
        isStueAktiv = false;
        manuelLys = false;
        _STYRKE_STUE = 255;

        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("LYS STUE RESATT");
        delay(1500);

        settMeny(HOVEDMENY);
        mainValg = TEMP;

        delay(200);
      }

      lcd.setCursor(0, 1);
      lcd.print(" NO     > YES  ");
      break;
    }
    break;
  case RST_LYS_BAD:
    lcd.setCursor(0, 0);
    lcd.print("Reset lys bad? ");

    switch (rst_yn) {
    case RST_NO:

      if (buttonClick(bOK)) {
        lcd.clear();
        settMeny(LYS_MNY_BAD);
      }

      lcd.setCursor(0, 1);
      lcd.print("> NO      YES  ");
      break;
    case RST_YES:

      if (buttonClick(bOK)) {
        lcd.clear();
        isBadAktiv = false;
        isDimmed = false;
        _STYRKE_BAD = 255;

        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("LYS BAD RESATT");
        delay(1500);

        settMeny(HOVEDMENY);
        mainValg = TEMP;

        delay(200);
      }
      lcd.setCursor(0, 1);
      lcd.print(" NO     > YES  ");
      break;
    }
    break;
  }
}