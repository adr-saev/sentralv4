
/*
 testfunk.ino

 Enkel testsketch for RGB-lyskontroll.
 - Brukes for manuell verifikasjon av `initlys()` og `lysFarge()`.
 - Ikke brukt i hovedprosjektet; behold som referanse/test.
*/

enum lys_gruppe { BAD, STUE };
void initLys();
void lysFarge(lys_gruppe gruppe, int r, int g, int b);
static lys_gruppe lys_g = BAD;
int rPin = 9;
int gPin = 10;
int bPin = 11;
int sR = 6;
int sG = 5;
int sB = 3;
int LYSBAD[] = {rPin, gPin, bPin, sR, sG, sB};

int badLengde = sizeof(LYSBAD) / sizeof(LYSBAD[0]);

void initlys() {
  for (int c = 0; c < badLengde; c++) {
    pinMode(LYSBAD[c], OUTPUT);
    Serial.print("Initiallerer RGB PIN: " +
                 String(LYSBAD[c] + "\t Går videre \n\n"));
  }
}

void lysFarge(lys_gruppe gruppe, int r, int g, int b) {

  switch (gruppe) {
  case BAD:
    analogWrite(rPin, r);
    analogWrite(gPin, g);
    analogWrite(bPin, b);
    break;
  case STUE:
    analogWrite(sR, r);
    analogWrite(sG, g);
    analogWrite(sB, b);
    break;
  }
}

void setup() {
  initlys();
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  int state = digitalRead(2);
  int bstate = digitalRead(4);

  switch (lys_g) {
  case BAD:

    if (state == LOW) {
      lysFarge(BAD, 255, 0, 0);
      delay(500);
      lysFarge(BAD, 0, 255, 0);
      delay(500);
      lysFarge(BAD, 0, 0, 255);
      delay(500);
      lysFarge(BAD, 0,0,0);
    }
    if (bstate == LOW) {
      lys_g = STUE;
    }
    break;
  case STUE:
    if (state == LOW) {
      lysFarge(STUE, 255, 0, 0);
      delay(500);
      lysFarge(STUE, 0, 255, 0);
      delay(500);
      lysFarge(STUE, 0, 0, 255);
      delay(500);
      lysFarge(STUE, 0,0,0);
    }
    if (bstate == LOW) {
      lys_g = BAD;
    }
    break;
  }
}