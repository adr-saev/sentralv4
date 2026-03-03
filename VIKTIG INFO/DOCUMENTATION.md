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
  - Viktige variabler: `aktivMeny`, `mainValg`, `tempValg`, `lysValg`, `isStueAktiv`, `isBadAktiv`, `manuelLys`.
  - Underfunksjoner: `tegn_HOVEDMENY()`, `tegn_TEMPMENY()`, `tegn_LYSMENY()`, `tegn_LYSHOVED()`, `tegn_LYSBAD()`.
  - Menyer for dør/vindu-sensorer (DOOR_WINDOW) og innstillinger (HJEM_MENY_INSTILLINGER) er delvis implementert.

- [src/funksjoner.h](src/funksjoner.h)
  - Generelle hjelpere og eksterne variabler brukt på tvers.

- [src/funksjoner.cpp](src/funksjoner.cpp)
  - `readTMP36(...)` — leser analoge TMP36-temperatursensorer og lagrer i `tempTMP[]`.
  - `readDHT11(...)` — leser DHT11-sensor for temperatur og fuktighet; lagrer i `tempDHT[]` og `fuktDHT[]`.
  - `serialTempFukt(...)` — printer TMP36 og DHT11 data periodisk til Serial (debug-utskrift hvert 1000 ms).
  - `serialLys(...)` — printer lysstatus til Serial (aktiv/inaktiv, automatisk/manuell).
  - `lesSerial()` — tolker Serial-kommandoer for hurtigkontroll/testing av menyer.

- [src/buttons.h](src/buttons.h)
  - Definerer knappepins og buzzer; deklarerer `initButtons()` osv.

- [src/buttons.cpp](src/buttons.cpp)
  - `initButtons()` — setter pinMode for knappene.
  - `buttonClick(pin)` — debouncet edge-detektering; returnerer true ved gyldig kort trykk.
  - `buttonHold(pin, holdTime)` — registrerer langt trykk.

- [src/lys.h](src/lys.h)
  - Enum `lys_gruppe` og deklarasjoner for lysstyrings-API (`initlys`, `lysFarge`, `lysStyrke`).
s for stue (_LYS_STUE) og bad (_LYS_BAD); skriver PWM-verdier.
  - `lysFarge(gruppe, r, g, b)` — setter farge for valgt gruppe (verdier 0-255).
  - `aktivLys(gruppe, isAktiv, ...)` — slår lys av/på for en gruppe med valgfri styrkepointer.
  - `oppdaterPhoto()` — leser fototransistor (A5) og oppdaterer automatisk lysstyrke.
  - `oppdaterLys()` — oppdaterer lyskontroll basert på `manuelLys`-flag (automatisk vs manuell).
  - `autoLysStyring(...)` — bestemmer om lys skal styres automatisk basert på lysintensitet.
  - Flags: `isStueAktiv`, `isBadAktiv`, `manuelLys
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
- **DHT11-sensor**: `readDHT11()` er implementert og fungerer for fukt- og temperaturlesing. `serialTempFukt()` viser data.
- **Automatisk lysstyrking**: `autoLysStyring()` og `oppdaterPhoto()` bruker fototransistor for automatisk lysintensitet-justering. Kan toggles med `manuelLys`-flag.
- **Dør-/vindu-sensorer**: `DOOR_WINDOW`-meny er deklarert men ikke implementert (knapp-handler er kommentert ut).
- **Innstillinger-meny**: `HJEM_MENY_INSTILLINGER` er deklarert men ikke implementert.
- **Lysmenyer**: `tegn_LYSHOVED()` og `tegn_LYSBAD()` er implementert for separat kontroll av stue og bad.
- Vær oppmerksom på eksisterende bug hvor et `if` muligens bruker `=` i stedet for `==` ved meny-sjekk (søk etter `if (aktivMeny =`).

Prioriteringer for videre utvikling
-----------------------------------
1. **Bug-fix**: Finn og rett `if (aktivMeny =` som skal være `if (aktivMeny ==`.
2. **Fullføring av menyer**: Implementer `DOOR_WINDOW`-meny (dør-/vindu-sensorer) og `HJEM_MENY_INSTILLINGER` (innstillings-meny).
3. **Testing**: Verifiser at DHT11, TMP36, fototransistor og automatisk lysstyrking fungerer som forventet.
4. **Kompilering**: Kjør kompilering og rapporterer eventuelle linker/kompilasjonsfeil.
5. **Utvidelser**: Vurder ytterligere funksjonalitet basert på brukererfaring.

---
Sist oppdatert: 3. mars 2026

`Fil laget av KI`