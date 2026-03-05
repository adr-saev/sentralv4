
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
  aktivLys(_LYS_STUE, isStueAktiv, &_STYRKE_STUE);
  aktivLys(_LYS_BAD, isBadAktiv);
}

bool autoLysStyring(bool manuelLys) { return !manuelLys; }
// aktivLys(gruppe, isAktiv,
// Skrur av og på lysene basert på isAktiv parameter
void aktivLys(lys_gruppe gruppe, bool isAktiv, int *_STYRKE_STUE = nullptr) {
  const int FAST_STYRKE = 255;

  if (!isAktiv) {
    analogWrite(gruppe, 0);
    return;
  }

  if (gruppe == _LYS_STUE) {
    if (manuelLys) {
      analogWrite(gruppe, FAST_STYRKE);
    } else {
      if (_STYRKE_STUE != nullptr) {
        analogWrite(gruppe, *_STYRKE_STUE);
      }
    }
    return;
  }
  if (gruppe == _LYS_BAD) {
    analogWrite(gruppe, FAST_STYRKE);
    return;
  }
}

/*Funksjon for å variere lysstyrken. I stuen skal man kunne dimme lysene.
Denne funskjonen skal dimme når bruker øsnker det,
det skal gjøres med å holde inne en knapp
Blir implementert
*/
void lysStyrke() {}