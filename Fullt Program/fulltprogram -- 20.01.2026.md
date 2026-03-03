/*
Beskrivelse:

----------------------------------------------------
Funksjoner:
    - Tempratur oversikt
    - Alarmer
    - Kontroller om dører / vinduer er åpne
        - Åpne lukke evt dører / vinduer
    - Lys dimming

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato:20/01/2026
Version: 1.0

----------------------------------------------------
KOMMENTAR


----------------------------------------------------
POTENSIELLE FEIL:
    - 
    -


--------------------------------------------------*/

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "src/meny.h"
#include "src/funksjoner.h"
#include "src/buttons.h"
#include "src/alarm.h"

LiquidCrystal lcd(53, 51, 49, 47, 45, 43);

//--------------------------------------------------
//Vaiabler:
float tempVal[tlength];
//--------------------------------------------------

void setup() {
    //Starter LCD skjerm
    lcd.begin(53, 43);
    Serial.begin(9600);
    initButtons();
    

}

void loop(){
    tempAvlesning(tempPin, tempVal, tlength);
    tegnMeny();
    lesSerial();

}
/*
Hovedkilde til menyene, styrer UI ( User Interface )


----------------------------------------------------
Funksjoner:
    -
    -

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato: 20/01/2026
Version: 1.0
*/

/*--------------------------------------------------
POTENSIELLE FEIL:


--------------------------------------------------*/
//--------------------------------------------------
// Vaiabler:
extern float tempVal[];
//--------------------------------------------------

#include <Arduino.h>

#include "meny.h"
#include "funksjoner.h"
#include "buttons.h"



extern LiquidCrystal lcd;

static MENY aktivMeny = HOVEDMENY;
static AKTIV_HOVED_MENY mainValg = TEMP;
static AKTIV_TEMP_MENY tempValg = SENSOR1;

void settMeny(MENY nyMeny) {
  aktivMeny = nyMeny;
}

void tegnMeny() {
  switch (aktivMeny) {
  case HOVEDMENY:
    // tegn menyen
    lcd.setCursor(0, 0);
    lcd.print("Velg Modus");
    lcd.setCursor(0, 1);

    if (aktivMeny == HOVEDMENY) {
      if (buttonClick(bUp)) {  }
      if (buttonClick(bDown)) { mainValg = INSTILLINGER; }
      if (buttonClick(bOK)) { settMeny(TEMPRATUR_MENY); }
    }
    switch (mainValg) {
    case TEMP:
      lcd.print("> Tempratur    ");
      break;
    case DOOR_WINDOW:
      lcd.print("> DOOR & WINDOW");
      break;
    case LYS:
      lcd.print("> Belysning     ");
      break;
    case TIMER:
      lcd.print("> TIMER         ");
      break;
    case INSTILLINGER:
      lcd.print("> Instillinger ");
    }
    break;
  case TEMPRATUR_MENY:

    serialTemp(tempVal, 3);

    lcd.setCursor(0, 0);
    lcd.print("< Tempratur    ");
    lcd.setCursor(0, 1);

    switch (tempValg) {
    case SENSOR1:

      lcd.print(">Sensor 1: ");
      lcd.print(tempVal[0]);
      lcd.print("            ");

      break;

    case SENSOR2:

      lcd.print(">Sensor 2: ");
      lcd.print(tempVal[1]);
      lcd.print("            ");

      break;
    case SENSOR3:

      lcd.print(">Sensor 3: ");
      lcd.print(tempVal[2]);
      lcd.print("            ");

      break;
    }
  }
}
/*
Forskjellige bakgrunns funksjoner som gjennomføres på
forskjellige menyer

----------------------------------------------------
Funksjoner:
    -
    -

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato: 20/01/2026
Version: 1.0
*/

/*--------------------------------------------------
POTENSIELLE FEIL:


--------------------------------------------------*/
#include <Arduino.h>

#include "meny.h"
#include "funksjoner.h"
#include "buttons.h"


void tempAvlesning(const int tempPin[], float tempVal[], int tlength) {
  for (int t = 0; t < tlength; t++) {
    int raw = analogRead(tempPin[t]);
    /*-------------------------------------------------------------------------------------*/
    // DERSOM BRUK AV TMP36
    // tempVal[t] = ((raw * (5000/1024)) - 500) /10;

    // VED BRUK AV POTENISOMETER
    tempVal[t] = 100 - (raw / 1023.0) * 100;
    tempVal[t] = map(tempVal[t], 0, 100, -25, 40);
  }
}

void serialTemp(float tempVal[], int templength) {
  static unsigned long lastPrint = 0;
  unsigned long now = millis();

  if (now - lastPrint >= 500) {
    lastPrint = now;

    // Viser: Sensor X:     Sensor Y    Sensor Z
    for (int t = 0; t < templength; t++) {
      Serial.print("Sensor ");
      Serial.print(t);
      Serial.print("\t");
    }
    Serial.println();

    // Viser: TempX:    Temp Y  Temp Z
    for (int tn = 0; tn < templength; tn++) {
      Serial.print("Temp: ");
      Serial.print(tempVal[tn]);
      Serial.print("\t");
    }
  }
}
void lesSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    Serial.print("Mottatt kommando: " + cmd + "\n");

    if (cmd == "STATUS") { Serial.print("System OK\n"); }
    if (cmd == "TEMP") {
      Serial.print("Viser TEMP MENY");
      settMeny(TEMPRATUR_MENY);
    }
    /*--------------------------------------------------*/
    if (cmd == "HOVEDMENY") {
      Serial.print("Viser HOVEDMENY");
      settMeny(HOVEDMENY);
    }
    if (cmd == "TEMP_MENY") {
      Serial.print("Viser TEMPRATUR_MENY");
      settMeny(TEMPRATUR_MENY);
    }
    /*--------------------------------------------------*/
    if (cmd == "HOME_TEMP") {
      Serial.print("Viser Home TEMP meny");
      settMeny(HOVEDMENY);
      mainValg = TEMP;
    }
    if (cmd == "HOME_DOOR_WIND") {
      Serial.print("Viser Home Door Window meny");
      settMeny(HOVEDMENY);
      mainValg = DOOR_WINDOW;
    }
    if (cmd == "HOME_LYS") {
      Serial.print("Viser Home LYS meny");
      settMeny(HOVEDMENY);
      mainValg = LYS;
    }
    if (cmd == "HOME_TIMER") {
      Serial.print("Viser Home TIMER meny");
      settMeny(HOVEDMENY);
      mainValg = TIMER;
    }
    if (cmd == "HOME_INSTILLINGER") {
      Serial.print("Viser Home INSTILLINGER");
      settMeny(HOVEDMENY);
      mainValg = INSTILLINGER;
    }

    /*--------------------------------------------------*/
    if (cmd == "TEMP_MENY_S1") {
      Serial.print("Viser TEMP_MENY SENSOR 1");
      settMeny(TEMPRATUR_MENY);
      tempValg = SENSOR1;
    }
    if (cmd == "TEMP_MENY_S2") {
      Serial.print("Viser TEMP_MENY SENSOR 2");
      settMeny(TEMPRATUR_MENY);
      tempValg = SENSOR2;
    }
    if (cmd == "TEMP_MENY_S3") {

      Serial.print("Viser TEMP_MENY SENSOR 3");
      settMeny(TEMPRATUR_MENY);
      tempValg = SENSOR3;
    }
    /*--------------------------------------------------*/

    if (cmd == "PRESS_OK") { simulateClick(bOK); }
    if (cmd == "PRESS_NXT") { simulateClick(bUp); }
    if (cmd == "PRESS_BCK") { simulateClick(bDown); }

    /*--------------------------------------------------*/
  }
}/*


----------------------------------------------------
Funksjoner:
    -
    -

----------------------------------------------------

Skrevet av: Adrian Nesse
Dato: 20/01/2026
Version: 1.0
*/

/*--------------------------------------------------
POTENSIELLE FEIL:
  - Knapp vil fortsatt være wasPressed = true vis meny byttes mens knapp er
innholdt.
    - Løsning: resetButton state
  -

--------------------------------------------------*/

#include <Arduino.h>
#include <HardwareSerial.h>
#include "buttons.h"
/*
Funksjon for å sjekke om knappen er holdt lenge nok,
holdTime bestemmer hvor lenge den skal holdes for at det skal registreres som
true holdTime måles i millis, så 1 sek = 1000

Eksempel: if(buttonHold(bOk, 1000){
    settMeny(ALARM_FERDIG_MENY);

})
*/
// Verdier for knapper


const int b[] = {bOK, bUp, bDown, bHome};
int blenght = sizeof(b) / sizeof(b[0]);

// Funksjon for å initiallere knappene.
void initButtons() {
  for (int bn = 0; bn < blenght; bn++) {
    pinMode(b[bn], INPUT_PULLUP);
    Serial.print("Initiallerer pin: " + String(b[bn]) +
                 "\t Fortsetter til neste\n\n");
  }
}

void simulateClick(int pin) {

}
bool buttonClick(int pin) {
  static unsigned long lastChange = 0;
  static unsigned long pressStart = 0;
  static bool wasPressed = 0;
  static bool lastState = HIGH;
  bool result = 0;

  int state = digitalRead(pin);

  if (state != lastState) { lastChange = millis(); }

  if (millis() - lastChange > 30) { lastState = state; }

  // Sjekker om knapp status er lav og om knappen var tidligere trykket
  if (lastState == LOW && !wasPressed) {
    wasPressed = true;
    pressStart = millis();
  }
  // Sjekker om knappen er høy og om den var presset, dersom den var vil
  // wasPressed satt til false
  // sjekker duration om det har gått under 200 ms, for da registrers trykket,
  // vis ikke returner false.
  if (lastState == HIGH && wasPressed) {
    wasPressed = false;

    unsigned long duration = millis() - pressStart;

    if (duration <= 200) {
      result = true;
      Serial.print("Knapp Trykket.");
    } else {
      result = false;
      Serial.print("Knapp Holdt for lenge.");
    }
  }
  return result;
}

bool buttonHold(int pin, unsigned long holdTime) {
  static unsigned long pressStart = 0;
  static unsigned long lastChange = 0;
  static bool isPressed = false;
  static bool holdState = HIGH;
  bool result = false;

  int state = digitalRead(pin);

  if (state != holdState) { lastChange = millis(); }
  if (millis() - lastChange > 30) { holdState = state; }

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

