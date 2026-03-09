
/*
Hovedkilde til menyene, styrer UI ( User Interface )




Skrevet av: Adrian Nesse
Dato: 25/01/2026
Version: 1.0
*/

//--------------------------------------------------
// INKLUDE:
#include <Arduino.h>

#include "meny.h"
#include "buttons.h"
#include "lys.h"
//--------------------------------------------------
// VARIABLER:
int _STYRKE_STUE = 0;
int fast_styrke = 255;
int _STYRKE_BAD = 255;
int dimme_retning = 5;

bool isDimmed = false;
//--------------------------------------------------

// initlys()
// Konfigurerer RGB-pin som OUTPUT for alle definerte lys.
void initlys() {
  pinMode(_LYS_BAD, OUTPUT);
  pinMode(_LYS_STUE, OUTPUT);
  Serial.println("LED PINs initialisert");
}

void oppdaterPhoto() {
  static unsigned long lastRead = 0;
  unsigned long now = millis();

  if (now - lastRead >= 100) {
    lastRead = now;

    int _LES_LYS_STUE = analogRead(foto_transistor_STUE);
    _STYRKE_STUE = map(_LES_LYS_STUE, 200, 900, 255, 0);
    _STYRKE_STUE = constrain(_STYRKE_STUE, 50, 255);
  }
}
void oppdaterLys() {
  aktivLys(_LYS_STUE, isStueAktiv, &_STYRKE_STUE, nullptr);
  aktivLys(_LYS_BAD, isBadAktiv,nullptr, &isDimmed);
}

bool autoLysStyring(bool manuelLys) { return !manuelLys; } // SITTER MANUEL STYR TIL SANT.
// aktivLys(gruppe, isAktiv,
// Skrur av og på lysene basert på isAktiv parameter
void aktivLys(lys_gruppe gruppe, bool isAktiv, int *_STYRKE_STUE = nullptr,
              bool *dimStatus = nullptr) {
  const int FAST_STYRKE = 255;

  if (!isAktiv) {
    
    if(gruppe == _LYS_BAD) {
      isDimmed = false;
      _STYRKE_BAD = 255;
    }

    analogWrite(gruppe, 0);
    return;
  }

  if (gruppe == _LYS_STUE) {
    if (manuelLys) { // MANUELL LYS = TRUE
      analogWrite(gruppe, FAST_STYRKE);
    } else {
      if (_STYRKE_STUE != nullptr) {
        analogWrite(gruppe, *_STYRKE_STUE);
      }
    }
    return;
  }
  if (gruppe == _LYS_BAD) {
    if (dimStatus != nullptr) {
      if (*dimStatus) {
        analogWrite(gruppe, _STYRKE_BAD);

      } else {
        analogWrite(gruppe, FAST_STYRKE);
      }
      return;
    }
  }
}
void lysDimming(lys_gruppe gruppe, int &_STYRKE_BAD, int &dimme_retning) {

  _STYRKE_BAD += dimme_retning;

  if (_STYRKE_BAD > 255 || _STYRKE_BAD < 0) {
    dimme_retning = -dimme_retning;
  }

  _STYRKE_BAD = constrain(_STYRKE_BAD, 0, 255);
  analogWrite(gruppe, _STYRKE_BAD);

  // DEBUG
  Serial.print("Dimming Pin ");
  Serial.print(gruppe);
  Serial.print(" til: ");
  Serial.println(_STYRKE_BAD);

  delay(50);
}

/*Funksjon for å variere lysstyrken. I stuen skal man kunne dimme lysene.
Denne funskjonen skal dimme når bruker øsnker det,
det skal gjøres med å holde inne en knapp
Blir implementert
*/
void lysStyrke() {}