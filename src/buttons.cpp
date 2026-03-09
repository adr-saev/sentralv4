/*
Beskrivelse: Knapp-håndteringsfunksjoner for Arduino-systemet.
            Håndterer knappetrykk med debouncing og hold-detektering.

----------------------------------------------------
Funksjoner:
    - initButtons(): Initialiserer knapp-pinner som input
    - buttonClick(pin): Detekterer kort knappetrykk med debouncing
    - buttonHolding(pin): Detekterer om knapp holdes nede

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
#include <HardwareSerial.h>
#include "buttons.h"

const int b[] = {bOK, bUP, bDown, bHome};
int blength = sizeof(b) / sizeof(b[0]);

// Funksjon for å initiallere knappene.
// Setter `pinMode` for hver knapp-pin definert i `buttons.h`.
void initButtons() {
  for (int bn = 0; bn < blength; bn++) {
    pinMode(b[bn], INPUT);
    Serial.print("Initiallerer pin: " + String(b[bn]) +
                 "\t Fortsetter til neste\n\n");
  }
}

// buttonClick(pin)
// Debouncet detektert knappetrykk. Returnerer true ved gyldig kort trykk
// (<=200 ms). Gir også lydfeedback via `buzzer` ved aksept.
bool buttonClick(int pin) {
  static unsigned long lastChange = 0;
  static unsigned long pressStart = 0;
  static bool wasPressed = false;
  static bool lastState = LOW;
  bool result = false;

  int state = digitalRead(pin);

  if (state != lastState) {
    lastChange = millis();
  }

  if (millis() - lastChange > 10) {
    lastState = state;
  }

  // Sjekker om knapp status er lav og om knappen var tidligere trykket
  if (state == HIGH && !wasPressed) {
    wasPressed = true;
    pressStart = millis();
  }
  // Sjekker om knappen er høy og om den var presset, dersom den var vil
  // wasPressed satt til false
  // sjekker duration om det har gått under 200 ms, for da registrers trykket,
  // vis ikke returner false.
  if (lastState == LOW && wasPressed) {
    wasPressed = false;

    unsigned long duration = millis() - pressStart;

    if (duration <= 200) {
      result = true;
      Serial.print("Knapp trykket: ");
      Serial.print(pin);
      Serial.print("");

      tone(buzzer, 900);
      delay(50);
      noTone(buzzer);
    } else {
      result = false;
      Serial.print("Knapp trykket: ");
      Serial.print(pin);
      Serial.print("\t IKKE GODKJENT");

      noTone(buzzer);
    }
    Serial.print("\n");
  }
  return result;
}

// buttonHolding(pin)
// Detekterer om en knapp holdes nede. Returnerer true så lenge knappen er trykket,
// med debouncing for å unngå falske signaler.
bool buttonHolding(int pin) {
  static bool stableState = HIGH;
  static unsigned long lastChange = 0;
  bool result = false;

  bool state = digitalRead(pin);

  if (state != stableState) {
    lastChange = millis();
  }

  if (millis() - lastChange > 10) {
    stableState = state;

    if (stableState == HIGH) {
      result = true;
    }
  }
  return result;
}
