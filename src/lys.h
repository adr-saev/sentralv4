/*
 lys.h

 Definisjoner for lys-modulen.
 - `lys_gruppe` enum beskriver hvilke fysiske RGB-grupper som finnes.
 - `initlys()` setter opp pinMode for alle RGB-pins.
 - `lysStyrke()` er en placeholder for dimme-logikk.
*/

#ifndef LYS_H
#define LYS_H

enum lys_gruppe { _LYS_BAD = 5, _LYS_STUE = 6 };

const int foto_transistor_STUE = A5;
// const int foto_transistor_STUE = A7;


extern bool isStueAktiv;
extern bool isBadAktiv;
extern bool manuelLys; // false = fototransistor styrer


bool autoLysStyring(bool manuelLys);
void aktivLys(lys_gruppe gruppe, bool isAktiv, int *_STYRKE_STUE = nullptr,
              bool *dimStatus = nullptr);
void lysDimming(lys_gruppe gruppe, int &_STYRKE_BAD, int &dimme_retning);
void initlys();
void oppdaterPhoto();
void oppdaterLys();


#endif