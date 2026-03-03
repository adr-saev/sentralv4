/*
 buttons.h

 Grensesnitt for knapp- og buzzer-modulen.

 Eksporterte symboler:
 - knapp-pin-konstanter: `bOK`, `bDown`, `bUP`, `bHome`.
 - `buzzer`: pin for lydfeedback.
 - initButtons(): konfigurerer knappene som input.
 - simulateClick(pin): test-helper for å simulere knappetrykk.
 - buttonClick(pin): edge-detektert kort trykk (debounced).
 - buttonHold(pin, holdTime): returnerer true hvis knapp holdes i angitt tid.
*/

#ifndef BUTTONS_H
#define BUTTONS_H
#include <Arduino.h>

const int bOK = 33;
const int bDown = 32;
const int bUP = 31;
const int bHome = 30;


//const int bOK = 6;
//const int bDown = 7;
//const int bUP = 8;
//const int bHome;

const int buzzer = 29;

void initButtons();
bool buttonClick(int pin);
bool buttonHold(int pin, unsigned long holdTime);

#endif