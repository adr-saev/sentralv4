/*
Beskrivelse: Hjelpefunksjoner for sensoravlesning og Serial-kommunikasjon.
            Inkluderer temperatur/fuktighetsavlesning og debugging-kommandoer.

----------------------------------------------------
Funksjoner:
    - readTMP36(): Leser temperatur fra TMP36-sensor
    - readDHT11(): Leser temperatur og fuktighet fra DHT11-sensor
    - serialTempFukt(): Printer sensorverdier til Serial for debugging
    - lesSerial(): Behandler kommandoer mottatt via Serial

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

#include "meny.h"
#include "lys.h"
#include "funksjoner.h"
#include "buttons.h"
#include <DHT.h>

extern DHT dht;

// tempAvlesning(tempPin, tempVal, tlength)
// Leser analoge sensorer definert i `tempPin[]` og skriver kalibrerte
// temperaturverdier inn i `tempVal[]` (skalert/mapper for testpot).
void readTMP36(const int TMPpin[], float tempTMP[], int tlength) {
  static unsigned long lastRead = 0;
  unsigned long now = millis();
  if (now - lastRead >= 1000) {
    lastRead = now;
    // for (int t = 0; t < tlength; t++) {
    // int raw = analogRead(TMPpin[t]);
    /*-------------------------------------------------------------------------------------*/
    // DERSOM BRUK AV TMP36
    // tempTMP[t] = ((raw * (5000.0 / 1024.0)) - 500) / 10;

    // VED BRUK AV POTENISOMETER
    // tempVal[t] = 100 - (raw / 1023.0) * 100;
    // tempVal[t] = map(tempVal[t], 0, 100, -25, 40);
    //}
    int raw = analogRead(TMPpin[0]);

    tempTMP[0] = ((raw * (5000.0 / 1023.0)) - 500) / 10;
  }
}

// readDHT11(fuktDHT, tempDHT, flength)
// Leser fuktighet og temperatur fra DHT11-sensoren og skriver inn i arrays.
// Bruker millis() for å begrense oppdateringsfrekvens til hver 1000 ms.
void readDHT11(float fuktDHT[], float tempDHT[], int flength) {

  static unsigned long lastRead = 0;
  unsigned long now = millis();

  if (now - lastRead >= 1000) {
    lastRead = now;

    // Bare én sensor
    fuktDHT[0] = dht.readHumidity();
    tempDHT[0] = dht.readTemperature();
  }
}

void serialLys(lys_gruppe gruppe) {

  static bool isAktiv = false;
  if (gruppe == _LYS_STUE) {
    isAktiv = isStueAktiv;
  } else if (gruppe == _LYS_BAD) {
    isAktiv = isBadAktiv;
  }

  if (!isAktiv) {
    Serial.print("Lys gruppe ");
    Serial.print(gruppe);
    Serial.println(" skrudd Av");
    return;
  }
  if (isStueAktiv && autoLysStyring(manuelLys)) {
    Serial.print("Lys PÅ (automatisk), styrke: ");
    Serial.println(_STYRKE_STUE);
  } else {
    Serial.print("Lys gruppe ");
    Serial.print(gruppe);
    Serial.println(" skrudd PÅ");
  }
}

// serialTemp(tempVal, templength)
// Printer temperaturinformasjon til Serial hvert 500 ms for debugging.

void serialTempFukt(float tempTMP[], float tempDHT[], float fuktDHT[],
                    int tlength, int flength) {

  static unsigned long lastPrint = 0;
  unsigned long now = millis();

  if (now - lastPrint >= 1000) {
    lastPrint = now;

    Serial.println("---- SENSOR DATA ----");

    // TMP36
    for (int t = 0; t < tlength; t++) {
      Serial.print("TMP36 ");
      Serial.print(t);
      Serial.print(": ");
      Serial.print(tempTMP[t]);
      Serial.println(" C");
    }

    // DHT11
    for (int f = 0; f < flength; f++) {
      Serial.print("DHT11 ");
      Serial.print(f);
      Serial.print(": ");
      Serial.print(tempDHT[f]);
      Serial.print(" C  |  ");
      Serial.print(fuktDHT[f]);
      Serial.println(" %");
    }

    Serial.println();
  }
}

// lesSerial()
// Leser tekstkommandoer fra Serial og utfører hurtigkommandoer for meny
// navigasjon og innstillinger (brukes mest til debugging/test).
void lesSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    Serial.print("Mottatt kommando: " + cmd + "\n");

    if (cmd == "STATUS") {
      Serial.print("System OK\n");
    }
    if (cmd == "STATUS_SENSOR") {
      // INGEN ARBEID ENDA
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
    if (cmd == "LYS_MENY") {
      Serial.print("Viser LYS_MENY");
      settMeny(LYS_MENY);
    }
    /*--------------------------------------------------*/
    if (cmd == "RST_ALL") {
      Serial.print("Viser RST_ALL");
      settMeny(RESET_INSTILLINGER);
      rst = RST_ALL;
    }
    if (cmd == "RST_STUE") {
      Serial.print("Viser RST_LYS_STUE");
      settMeny(RESET_INSTILLINGER);
      rst = RST_LYS_STUE;
    }
    if (cmd == "RST_BAD") {
      Serial.print("Viser RST_LYS_BAD");
      settMeny(RESET_INSTILLINGER);
      rst = RST_LYS_BAD;
    }

    /*--------------------------------------------------*/
    if (cmd == "HOME_TEMP") {
      Serial.print("Viser Home TEMP meny");
      settMeny(HOVEDMENY);
      mainValg = TEMP;
    }
    if (cmd == "HOME_LYS") {
      Serial.print("Viser Home LYS meny");
      settMeny(HOVEDMENY);
      mainValg = LYS_HVDMNY;
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
    /*--------------------------------------------------*/
    if (cmd == "BAD_AKTIV") {
      isBadAktiv = !isBadAktiv;
      Serial.print(isBadAktiv ? "LYS STATUS: ON" : "LYS STATUS: OFF");
    }
    if (cmd == "BAD_DIM") {
      isDimmed = !isDimmed;
      Serial.print(isDimmed ? "Mode: Dimmed" : "Mode: FAST STYRKE");
    }
    if (cmd == "TEST_DIM_FUNC") {
      isDimmed = true;
      isBadAktiv = true;
      settMeny(LYS_MNY_BAD);

    } else if (cmd.startsWith("DIMMING_VERDI")) {
      int nyVerdi;
      if (sscanf(cmd.c_str(), "DIMMING_VERDI %d", &nyVerdi) == 1) {
        dimme_retning = nyVerdi;
        Serial.print("Ny dimmehastighet satt til: ");
        Serial.println(dimme_retning);
      } else {
        Serial.println("Feil format! Bruk: DIMMING_VERDI [tall]");
      }
    }
    /*--------------------------------------------------*/
    if (cmd == "STUE_AKTIV") {
      isStueAktiv = !isStueAktiv;
      Serial.print(isStueAktiv ? "LYS STATUS: ON" : "LYS STATUS: OFF");
    }
    if (cmd == "STUE_MANUEL") {
      manuelLys = !manuelLys;
      Serial.println(manuelLys ? "Modus: Manuell Lys Styring"
                               : "Modus: Autmoatisk Lys Styring");
    }
  }
}