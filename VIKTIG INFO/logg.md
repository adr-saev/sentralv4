
# LOGG - AV ADRIAN NESSE

## Plan tegning

![Lys og Sensorer](dokumentasjon/plantegning,%20lys%20og%20sensorer.jpg)
![Servo og Temp](dokumentasjon/plantegning,%20servo%20og%20temp.jpg)
#### Dato : 20/01/2026

----------------------------------------------------
### Funksjoner:
-   Hjemmesentral
-   Lysdimming
-   Tempratur oversikt
    - Skru på vifte, dersom det blir varmt nok i rommet.
    - Samme på begge rommene
-   Åning og lukking av dører
-   evt. instillinger for buzzer, måleenhet, osv.
    
----------------------------------------------------

**Skrevet av**: _Adrian Nesse_
**Dato**: _25/01/2026_ 
**Version**: _1.0_


--------------------------------------------------
### POTENSIELLE FEIL!:
- Skrivefeil


--------------------------------------------------


## DATO: 16/01/2026

**buttonClick()**
Har laget en funksjon for registrering av click, funksjonen har debounce, og en funksjonalitet som gjør at den registerer kun vis trykket varer i mindre enn ...

**buttonHold()**
Gjør nesten samme som button click, forskjellen er at denne returnerer kun sant dersom variablelen ***holdTime*** altså instillt tid, f. eks kan jeg skrive buttonHold(pin, 1000); dette gjør at jeg må holde pin eller f.eks. button1 i 1000 ms, eller 1 sek.

**serialTemp()**
Laget en funksjon som skal gjøre det lettere og hente ut tempraturen fra alle sensorer.

--------------------------------------------------

## DATO: 20/01/2026

**lesSerial()** 
Har laget en funskjno for lesSerial(), denne funksjonen leser fra serial, så jeg kan skrive skrive inn i terminal slik at en vis ting vil skje, f. eks. kan jeg skrive ***TEMP_MENY*** så vil Tempratur menyen åpnes. Brukes til og testing uten å måtte bruke knapper. 

Hovedsaklig blir dette gjort i dag, fordi jeg ikke har tilgang på så mange knapper, og utganger som jeg skulle ønsket. Må ordne opp i dette. VIl implementere en funksjon som gjør at jeg kan endre på verdier slik at jeg kan simulere trykk og sensor endringer. 

**NB**:Har laget en rekke komandoer som skal utføre forskjellige ting, flere kommandoer kommer

_MÅ IMPLEMENTERES_ 
Må fikse slik at jeg kan skrive inn verdier for f. eks temptraturen, slik at jeg kan debugge via det.

**I meny.cpp**


--------------------------------------------------

## DATO: 25/01/2026

**I button.cpp** 
buttonClick() fungerer ikke slik som den skal, så jobber med og fikse det. ønsker også å implementert en funksjon slik at vær knapp har en egen status, slik at det ikke registrer en knapp som ikke skal registrers

Jobbet mye med dette i dag. 
Har ikke klart og løse problemet enda. Fikk det til og fungere en stund, men da var det litt hær og der, prøver å feilsøke, men har holdt på for lenge. Trenger en pause.

--------------------------------------------------

## DATO 26/01/2026

**meny.cpp og buttons.cpp**
Endelig fått knappene til og fungere, nå gjør det som de skal. Prøvde og lage en ny funksjon for de, men fikk første til og fungere.

Kan bytte på menyer, og gå inn på temp. menyen, det er eneste aktive akkuratt nå.

**Prosjektet**
Tar vekk timer funksjonen, vil ikke ha den. Men skal heller implementere en alarm, sånn at vis alarmen er på og dør blir åpnet så vil alarmen gå. 

Har laget prototype tegning. 

Har begynt på lys justeringer, og hvordan det vil fungere. Må nok lage en funksjon for å endre en verdi når en knapp blir holdt inne. 

**TEMPRATUR MENY**
Temp menyen fungerer nå, kan gå frem og tilbake mellom sensorene, kan også gå tilbake.

**meny.cpp**
Begynt på lys menyen.

Har ikke begynt på funksjonaliteten, bare å få det opp på lcd skjermen. Skal potensielt få en 16x04 skjerm, så har litt arbeid og gjøre. Får jobbe hjemme med lysene. 

*__Skal også ha phototransistor til å jsutere lysene. Skal ligge inn en manuell overstyring når man holder inne en knapp lenge nokk. __*

--------------------------------------------------


## DATO 28/01/2026

**LAGET lys.cpp & lys.g**

Laget funksjon for å velge lys fargen for lyskontroll i stuen og bad.

Definert 4 standard farger. Rød, grønn, blå og hvit

*_MÅ LAGE FUNKSJON FOR DIMMING_*

**meny.cpp**
Begynt og tegne menyene for Lys og instillinger. Lager først instillinger, for farger osv.

--------------------------------------------------

## DATO 02/02/2026

**Generelt**
Begynt på flyt skjema, kommer til og ligge det inn i dokumentasjon mappen. 
Fått Flytskjema for lys menyer.

**meny.cpp** 
Laget menyene for stnd farger. _Ikke implimentert faktisk farge endring_. Ingen knappetrykk heller. Kan bla gjennom menyene. Må fortsatt lage Rom instillinger osv.

--------------------------------------------------

## DATO 04/02/2026

INGEN ANELSE PÅ PROGRAMM GJORT; GLEMT OG LOGGFØRE

--------------------------------------------------

## DATO 09/02/2026

### AI VURDERING
#### Programrevisjon Komplettert

| Aspekt | Karakter | Status |
|--------|---------|--------|
| **Dokumentasjon** | A- | ✅ Komplett, tydelig |
| **Ryddighet** | A- | ✅ Velorganisert |
| **Logikk** | B+ | ✅ Fungerer korrekt |
| **Kodekvalitet** | B+ | ✅ Lesbar, vedlikeholdt |
| **Kommentarer** | B+ | ✅ Beskrivende |
| **TOTALT** | **B+** | ✅ Solid, produksjonsklar kode |

**Programmet er nå i veldig god stand!** Dokumentasjonen er komplett, koden er ryddig, og logikken fungerer.

#### Gjennomførte forbedringer:
- ✅ Refaktorert `tegnMeny()` fra 400 til 5 separate funksjoner
- ✅ Dokumentert alle tegn_*() funksjoner
- ✅ Fjernet SENSOR3 referanser fra lesSerial()
- ✅ Standardisert variabelnavngiving
- ✅ Ryddet opp tomme seksjoner og dead code
- ✅ Fikset navigationbugs i lys-menyer

**lys.cpp**

Fikset og laget aktivLys() funksjon

**meny.cpp** 

Laget om programmet, gjort det mer leslig. Har laget en tegn_**() for hver meny, for lettere feilsøking, testing, implementering. 

Implementert aktivLys() og laget en variabel for om lysene er aktiv eller ikke. Lys fungerer når man øsnker at de skal på.

_Lag serial kommandoer for lys aktivering, og nye meny endringer._

Ryddet opp i programmet og fått det vurdert av AI, har gjort endringer utifra kommentarene AI har gitt, for å rydde mer opp i programmet, første vurdering var gjennomsnitts karakter _C-_, har fått den opp til en _B_

**Generelt i programmet**
Laget kommentarer på alle void funksjonene i _meny.cpp_, skal fikse i resten av programmet.

### MÅ FIKSES
_Fiks fuktighets sensoren på badet, samt bruk av phototransistor._

_BEGYNN PÅ DØRER OG VINDUER MED EN GANG DETTE ER FERDIG_
_LES DEG OPP PÅ SERVOMOTOR_ FOR VIFTE OG DØRÅPNER OG VINDU ÅPNER,

Lag funskjon for å sjekk om dør / vindu er åpen, dette blir en *bool* 

## DATO 11/02/2026

**funksjoner.cpp**
Laget funksjon for readDHT, den fungerer ikke. Har også problmerer med TMP36 sensor. Så dette skal fikses.

**Problemet**
DHT registrer ikke. Viser nan, må også fikse autoscoll.


## DATO 16/02/2026

**TEMPRATUR PROBLEM FIKSET** 

Har endret en del kode i _funksjoner.cpp_, har gjort slik at begge sensorene (_TMP36_ og _DHT11_) oppdaterer 1 gang i sekundet, og samt skriver til serial monitor 1 gang i sekundet.

Har også endret en del på hvordan tempratur menyen har endret seg

#### Neste steg 
Begynn på servo settings, eller manuellstyring og dimming.

# DATO 25/02/2026

[**lys.cpp**](src/lys.cpp)
Har begynt på lysstyringen, den fungerer på tinkercad, men sliter med og få den implementert inn i programmet her. Tror ikke prossesoren jeg har er kraftig nokk ( Arduino Uno )-

Må feilsøke hvorfor lyset starter på. Har gjort store endringer i programmet. Har laget en funksjon som oppdaterer lysene, slik at de kan dimmes og automatisk styres. Har flyttet Serial.print ut av [aktivLys()](src/lys.cpp#L53-L66) har gjort store endringer i mellomrommet [lys.cpp](src/lys.cpp#L33-L66)
 
[**Meny.cpp**](src/meny.cpp)
Menyen feiler når jeg prøver og gå rundt, tror det har noe med arduinoen. Har fjernet [aktivLys()](src/lys.cpp#L53-L66) fra alle plassene det sto , nå endrer vi heller bare status på isBadAktiv og isStueAktiv.

## MÅ FEILSØKE VIDERE

# DATO 02/03/2026

[**lys.cpp**](src/lys.cpp)
Har fått lyset til og fungere, alt fungerer med arduino mega. Skal begynne på dimme funksjon


# DATO 04/03/2026

Har ikke jobbet i prosjektet i dag. Har jobbet på en test funksjon. Jeg gjobber med dimmingen, har fått dette til og fungere på tinkercad. Men har ikke koblet det opp på arduino enda. 