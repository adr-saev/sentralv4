/*


Funksjon for å sjekke om knappen er holdt lenge nok,
holdTime bestemmer hvor lenge den skal holdes for at det skal registreres som
true holdTime måles i millis, så 1 sek = 1000

Eksempel: if(buttonHold(bOk, 1000){
    settMeny(ALARM_FERDIG_MENY);

})



Skrevet av: Adrian Nesse
Dato: 20/01/2026
Version: 1.0
*/


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

  if (state != lastState) { lastChange = millis(); }

  
  if (millis() - lastChange > 10) { lastState = state; }

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

  // buttonHold(pin, holdTime)
  // Returnerer true dersom knapp holdes i minst `holdTime` millisekunder.
  bool buttonHold(int pin, unsigned long holdTime) {
    static unsigned long pressStart = 0;
    static unsigned long lastChange = 0;
    static bool isPressed = false;
    static bool holdState = HIGH;
    bool result = false;

    int state = digitalRead(pin);

    if (state != holdState) {
      lastChange = millis();
    }
    if (millis() - lastChange > 30) {
      holdState = state;
    }

    if (state == LOW && !isPressed) {
      isPressed = true;
      pressStart = millis();
    }

    if (state == HIGH && isPressed) {
      isPressed = false;

      unsigned long duration = millis() - pressStart;
      Serial.print("Tid holdt inne: ");
      Serial.print(duration);
      Serial.print("\n");

      if (duration >= holdTime) {
        result = true;
      } else {
        result = false;
      }
    }
    return result;
  }
