/*
==============================================================================
SENTRALV4 - FULLT PROGRAM (ENKELT FIL)
==============================================================================

Beskrivelse: Komplett hjemmesentralsystem for Arduino Mega.
            Håndterer LCD-meny, lysstyring, temperatursensorer og alarmer.

Integrasjon: Alle moduler kombinert i én .ino-fil
- Menylogikk (LCD-grensesnitt)
- Lysstyring (fototransistor + manuell dimming)
- Knappehåndtering (debouncing + hold-detektoring)
- Sensoravlesning (DHT11 + TMP36)
- Alarmlogikk
- Serial-kommandoer for debugging

Skrevet av: Adrian Nesse
Dato: 09/03/2026
Version: 1.0

==============================================================================
*/

//==============================================================================
// BIBLIOTEKER
//==============================================================================
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <HardwareSerial.h>

//==============================================================================
// HARDWARE-DEFINISJON
//==============================================================================

// LCD
LiquidCrystal lcd(53, 52, 51, 50, 49, 48);

// DHT11-SENSOR
#define DHT11pin 45
#define DHTTYPE DHT11
DHT dht(DHT11pin, DHTTYPE);

// KNAPP-PINNER
const int bOK = 33;
const int bDown = 32;
const int bUP = 31;
const int bHome = 30;

// BUZZER
const int buzzer = 29;

// LYS
enum lys_gruppe { _LYS_BAD = 5, _LYS_STUE = 6 };

// FOTOTRANSISTOR
const int foto_transistor_STUE = A5;

// TEMPERATURSENSOR (TMP36)
const int TMPpin[] = {A0};
const int tlength = sizeof(TMPpin) / sizeof(TMPpin[0]);
const int flength = 1;

//==============================================================================
// ENUM-DEFINISJONER (MENYER)
//==============================================================================

enum MENY {
  HOVEDMENY,
  TEMPRATUR_MENY,
  LYS_MENY,
  LYS_MNY_HOVEDROM,
  LYS_MNY_BAD,
  RESET_INSTILLINGER
};

enum AKTIV_HOVED_MENY {
  TEMP,
  LYS_HVDMNY,
  RESET_ALL
};

enum AKTIV_TEMP_MENY {
  SENSOR1,
  SENSOR2
};

enum AKTIV_LYS_MENY {
  HOVEDROM,
  BAD,
  AKTIV_ALL
};

enum LYS_HOVEDROM { ACTIVE_HVDROM, LYS_STYRING, STUE_RESET };

enum LYS_BAD { ACTIVE_BAD, DIMMING, BAD_RESET };

enum RST_INST { RST_ALL, RST_LYS_STUE, RST_LYS_BAD };
enum RST_YN { RST_NO, RST_YES };

//==============================================================================
// GLOBALE VARIABLER - SENSORER
//==============================================================================

float tempTMP[tlength];
float fuktDHT[flength];
float tempDHT[flength];

//==============================================================================
// GLOBALE VARIABLER - MENY
//==============================================================================

static MENY aktivMeny = HOVEDMENY;
static AKTIV_HOVED_MENY mainValg = TEMP;
static AKTIV_TEMP_MENY tempValg = SENSOR1;
static AKTIV_LYS_MENY lysValg = HOVEDROM;
static LYS_HOVEDROM l_hvd = ACTIVE_HVDROM;
static LYS_BAD l_bad = ACTIVE_BAD;
static RST_INST rst = RST_ALL;
static RST_YN rst_yn = RST_NO;

bool isStueAktiv = false;
bool isBadAktiv = false;
bool manuelLys = false;  // false = fototransistor styrer

//==============================================================================
// GLOBALE VARIABLER - LYS
//==============================================================================

int _STYRKE_STUE = 0;
int fast_styrke = 255;
int _STYRKE_BAD = 255;
int dimme_retning = 5;
bool isDimmed = false;



//==============================================================================
// FORWARD DECLARATIONS
//==============================================================================

void aktivLys(lys_gruppe gruppe, bool isAktiv, int *_STYRKE_STUE = nullptr,
              bool *dimStatus = nullptr);
void settMeny(MENY nyMeny);

//==============================================================================
// KNAPP-FUNKSJONALITET
//==============================================================================

// Funksjon for å initialisere knappene.
// Setter `pinMode` for hver knapp-pin.
void initButtons() {
  const int b[] = {bOK, bUP, bDown, bHome};
  int blength = sizeof(b) / sizeof(b[0]);
  
  for (int bn = 0; bn < blength; bn++) {
    pinMode(b[bn], INPUT);
    Serial.print("Initiallerer pin: " + String(b[bn]) +
                 "\t Fortsetter til neste\n\n");
  }
}

// buttonClick(pin)
// Debouncet detektert knappetrykk. Returnerer true ved gyldig kort trykk (<=200 ms).
// Gir også lydfeedback via `buzzer` ved aksept.
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

  if (state == HIGH && !wasPressed) {
    wasPressed = true;
    pressStart = millis();
  }

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

//==============================================================================
// LYS-FUNKSJONALITET
//==============================================================================

// initlys()
// Konfigurerer lys-pinner som OUTPUT.
void initlys() {
  pinMode(_LYS_BAD, OUTPUT);
  pinMode(_LYS_STUE, OUTPUT);
  Serial.println("LED PINs initialisert");
}

// oppdaterPhoto()
// Leser fototransistor-verdien og mapper den til lysstyrke for stuen.
// Oppdateres hver 100 ms for responsiv lysstyring.
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

// oppdaterLys()
// Oppdaterer lysstatus for både stue og bad basert på aktive flagg og modus.
// Kaller aktivLys for hver gruppe med relevante parametere.
void oppdaterLys() {
  aktivLys(_LYS_STUE, isStueAktiv, &_STYRKE_STUE, nullptr);
  aktivLys(_LYS_BAD, isBadAktiv, nullptr, &isDimmed);
}

// autoLysStyring(manuelLys)
// Returnerer true hvis automatisk lysstyring er aktiv (manuelLys er false).
bool autoLysStyring(bool manuelLys) {
  return !manuelLys;
}

// aktivLys(gruppe, isAktiv, ...)
// Skrur av og på lysene basert på isAktiv parameter.
void aktivLys(lys_gruppe gruppe, bool isAktiv, int *_STYRKE_STUE = nullptr,
              bool *dimStatus = nullptr) {
  const int FAST_STYRKE = 255;

  if (!isAktiv) {
    if (gruppe == _LYS_BAD) {
      isDimmed = false;
      _STYRKE_BAD = 255;
    }
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

// lysDimming(gruppe, _STYRKE_BAD, dimme_retning)
// Dimmer lys ved å endre PWM-verdi over tid.
void lysDimming(lys_gruppe gruppe, int &_STYRKE_BAD, int &dimme_retning) {
  _STYRKE_BAD += dimme_retning;

  if (_STYRKE_BAD > 255 || _STYRKE_BAD < 0) {
    dimme_retning = -dimme_retning;
  }

  _STYRKE_BAD = constrain(_STYRKE_BAD, 0, 255);
  analogWrite(gruppe, _STYRKE_BAD);

  Serial.print("Dimming Pin ");
  Serial.print(gruppe);
  Serial.print(" til: ");
  Serial.println(_STYRKE_BAD);

  delay(50);
}

// lysStyrke()
// Placeholder for fremtidig lysstyrke-funksjonalitet.
void lysStyrke() {}

//==============================================================================
// SENSOR-FUNKSJONALITET
//==============================================================================

// readTMP36(TMPpin, tempTMP, tlength)
// Leser analoge sensorer og skriver kalibrerte temperaturverdier inn i tempTMP[].
void readTMP36(const int TMPpin[], float tempTMP[], int tlength) {
  static unsigned long lastRead = 0;
  unsigned long now = millis();
  if (now - lastRead >= 1000) {
    lastRead = now;
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
    fuktDHT[0] = dht.readHumidity();
    tempDHT[0] = dht.readTemperature();
  }
}

// serialTempFukt(...)
// Printer sensordata (TMP36 og DHT11) til Serial hver 1000 ms for debugging.
void serialTempFukt(float tempTMP[], float tempDHT[], float fuktDHT[],
                    int tlength, int flength) {
  static unsigned long lastPrint = 0;
  unsigned long now = millis();

  if (now - lastPrint >= 1000) {
    lastPrint = now;

    Serial.println("---- SENSOR DATA ----");

    for (int t = 0; t < tlength; t++) {
      Serial.print("TMP36 ");
      Serial.print(t);
      Serial.print(": ");
      Serial.print(tempTMP[t]);
      Serial.println(" C");
    }

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

//==============================================================================
// SERIAL-KOMMUNIKASJON (DEBUGGING)
//==============================================================================

// lesSerial()
// Leser tekstkommandoer fra Serial og utfører hurtigkommandoer for meny-navigasjon
// og innstillinger (brukes mest til debugging/test).
void lesSerial() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    Serial.print("Mottatt kommando: " + cmd + "\n");

    if (cmd == "STATUS") {
      Serial.print("System OK\n");
    }

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

    if (cmd == "STUE_AKTIV") {
      isStueAktiv = !isStueAktiv;
      Serial.print(isStueAktiv ? "LYS STATUS: ON" : "LYS STATUS: OFF");
    }
    if (cmd == "STUE_MANUEL") {
      manuelLys = !manuelLys;
      Serial.println(manuelLys ? "Modus: Manuell Lys Styring"
                               : "Modus: Automatisk Lys Styring");
    }
  }
}



//==============================================================================
// MENY-FUNKSJONALITET
//==============================================================================

// settMeny(nyMeny)
// Funksjon brukt til å endre menyen til ønsket meny.
void settMeny(MENY nyMeny) {
  aktivMeny = nyMeny;
}

// tegnMeny()
// Hovedfunksjon for meny-håndtering. Dispatcher til relevant tegnXXX()-funksjon.
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
// Rendrer hovedmenyen med navigasjon mellom modus (TEMP, LYS, RESET_ALL).
// Håndterer UP/DOWN for navigasjon og OK for å endre meny.
void tegn_HOVEDMENY() {
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
// Rendrer tempratur-menyen som viser temperatur og luftfuktighet.
// Håndterer UP/DOWN for sensor-valg og HOME for å gå tilbake.
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
    lcd.print("> ");
    lcd.print(tempDHT[0]);
    lcd.setCursor(8, 1);
    lcd.print(fuktDHT[0]);
    lcd.print("%   ");
    break;
  }
}

// tegn_LYSMENY()
// Rendrer lys-menyen med navigering mellom valg (Hovedrom, Bad, Aktiver alt).
// Navigering med UP/DOWN og OK for å sitte meny til ønsket undermeny.
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
// Rendrer meny for lys på Stuen, med navigasjon til instillinger.
// Håndterer aktiver/deaktiver og lys-styring (manuell/automatisk).
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
// Rendrer meny for lys på Bad, med navigasjon til instillinger og dimming.
// Håndterer aktiver/deaktiver, dimming (ved å holde OK) og reset.
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

// tegn_RST_INST()
// Rendrer reset instillinger meny, med valg mellom JA og NEI for å bekrefte reset.
// Håndterer reset av alt system, lys stue og lys bad.
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

//==============================================================================
// SETUP & LOOP
//==============================================================================

void setup() {
  // Initialiserer LCD-skjerm
  lcd.begin(16, 2);
  Serial.begin(9600);
  dht.begin();

  pinMode(buzzer, OUTPUT);
  pinMode(foto_transistor_STUE, INPUT);
  initButtons();
  initlys();
  delay(1500);
}

void loop() {
  // Oppdaterer LCD-meny basert på aktiv menytilstand
  tegnMeny();

  // Leser sensorer
  readDHT11(fuktDHT, tempDHT, flength);      // Leser fuktighet og temperatur fra DHT11-sensor
  readTMP36(TMPpin, tempTMP, tlength);       // Leser temperatur fra TMP36-sensor

  // Oppdaterer lys
  oppdaterPhoto();  // Oppdaterer lysstyrke for stue basert på fototransistor
  oppdaterLys();    // Aktiverer/deaktiverer lys for stue og bad basert på flagg

  // Håndterer Serial-kommandoer for debugging/testing
  lesSerial();
}
