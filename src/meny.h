/*
 meny.h

 Deklarasjoner for meny-systemet og alle enum-typer brukt av UI.
 - Beskriver hierarkiet av menyer og undervalg som `tegnMeny()` bruker.
 - Alle enums skal gi tydelig mapping mellom meny-tilstand og visning.
*/

#ifndef MENY_H
#define MENY_H

#include <LiquidCrystal.h>
extern LiquidCrystal lcd;

enum MENY {
  HOVEDMENY,      // 0
  TEMPRATUR_MENY, // 1
  LYS_MENY,       // 2
  LYS_MNY_HOVEDROM,
  LYS_MNY_BAD,
  ALARM_FERDIG_MENY // 6

};

enum AKTIV_HOVED_MENY {
  TEMP,        // 0
  LYS_HVDMNY,  // 1
  DOOR_WINDOW, // 2
  HJEM_MENY_INSTILLINGER
};

enum AKTIV_TEMP_MENY {
  SENSOR1, // 0
  SENSOR2, // 1
};

enum AKTIV_LYS_MENY {
  HOVEDROM,
  BAD,
  AKTIV_ALL,
  LYS_INSTILLINGER,
};

enum LYS_HOVEDROM { ACTIVE_HVDROM, LYS_STYRING };

enum LYS_BAD { ACTIVE_BAD, DIMMING };

void settMeny(MENY nyMeny);
void tegnMeny();
void tegn_HOVEDMENY();
void tegn_TEMPMENY();
void tegn_LYSMENY();
void tegn_LYSHOVED();
void tegn_LYSBAD();

#endif