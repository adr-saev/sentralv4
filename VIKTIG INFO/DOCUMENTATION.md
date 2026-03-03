PROJECT: sentralv4
===================
Kort dokumentasjon og oversikt over kildefiler under `src/`.

Formål
------
Dette dokumentet oppsummerer ansvar og nøkkelfunksjoner i hver fil. Kommentarene i kildefilene er skrevet på norsk; godkjennede kommentarer er allerede lagt inn. Bruk dette som en rask referanse.

Filer
-----
- [sentralv4.ino](sentralv4.ino)
  - Entrypoint: `setup()` initialiserer LCD, Serial og underliggende moduler.
  - `loop()` skal kalle `tegnMeny()` og periodiske oppgaver.

- [src/meny.h](src/meny.h)
  - Definerer alle meny-enum og deklarerer `settMeny()` og `tegnMeny()`.

- [src/meny.cpp](src/meny.cpp)
  - UI- og menylogikk. Hovedfunksjon: `tegnMeny()`.
  - Styrer navigasjon, LCD-utskrift og leser knappetrykk via `buttons`.
  - Viktige variabler: `aktivMeny`, `mainValg`, `tempValg`, `lysValg`, `farge_innst`.

- [src/funksjoner.h](src/funksjoner.h)
  - Generelle hjelpere og eksterne variabler brukt på tvers.

- [src/funksjoner.cpp](src/funksjoner.cpp)
  - `tempAvlesning(...)` — leser analoge temperatur-sensorer.
  - `serialTemp(...)` — printer temperaturer til Serial (debug).
  - `lesSerial()` — tolker Serial-kommandoer for hurtigkontroll/testing.

- [src/buttons.h](src/buttons.h)
  - Definerer knappepins og buzzer; deklarerer `initButtons()` osv.

- [src/buttons.cpp](src/buttons.cpp)
  - `initButtons()` — setter pinMode for knappene.
  - `buttonClick(pin)` — debouncet edge-detektering; returnerer true ved gyldig kort trykk.
  - `buttonHold(pin, holdTime)` — registrerer langt trykk.

- [src/lys.h](src/lys.h)
  - Enum `lys_gruppe` og deklarasjoner for lysstyrings-API (`initlys`, `lysFarge`, `lysStyrke`).

- [src/lys.cpp](src/lys.cpp)
  - Konfigurerer RGB-pin og skriver PWM-verdier.
  - `lysFarge(gruppe, r, g, b)` — setter farge for valgt gruppe (verdier 0-255).
  - Merk: nåværende implementasjon inneholder intern knappetoggle knyttet til `bOK`.

- [src/alarm.h](src/alarm.h)
  - Deklarasjoner for alarm-API (getters, oppdatering, start/stopp, reset).

- [src/alarm.cpp](src/alarm.cpp)
  - Implementerer alarm-tidtaking og state-maskin.
  - `oppdaterAlarm()` må kalles periodisk for å dekrementere tid.

- [src/testfunk.ino](src/testfunk.ino)
  - Enkel testsketch for å verifisere RGB-funksjonalitet.

Notater og anbefalinger
----------------------
- For å gjøre `lysFarge()` rent ansvarlig, anbefales å flytte av/på-toggling (som i dagens implementasjon) til menylogikk i `meny.cpp`.
- Vær oppmerksom på en eksisterende bug hvor et `if` muligens bruker `=` i stedet for `==` ved meny-sjekk (søk etter `if (aktivMeny =`).

Neste steg
---------
- Gi beskjed hvis du vil at jeg skal: 1) generere en mer detaljert `DOCUMENTATION.md` med eksempler, 2) flytte toggle-logikk ut av `lys.cpp` og inn i `meny.cpp`, eller 3) kjøre en kompilering og rapportere eventuelle linker/kompilasjonsfeil.

---
Dokument laget automatisk av assistenten. Si ifra for endringer i formulering eller språk.