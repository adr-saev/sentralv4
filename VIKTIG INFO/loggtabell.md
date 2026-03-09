| DATO        | BILDER | BESKRIVELSE | KOMMENTAR |
|-------------|--------|-------------|-----------|
| 20/01/2026  | Lys og sensorer / Servo og temp | Plantegning for lys, sensorer, servo og temperatur. Funksjoner: hjemmesentral, lysdimming, temperaturvisning, vifte, dører, buzzer, innstillinger. Dokumentasjon versjon 1.0. | Potensielle skrivefeil. |
| 16/01/2026  |        | Laget `buttonClick()` (debounce + kort trykk), `buttonHold()` (holdetid), og `serialTemp()` for å hente temperatur fra alle sensorer. | |
| 20/01/2026  |        | Laget `lesSerial()` for å lese kommandoer via Serial (f.eks TEMP_MENY). Brukes til testing uten knapper. Planlagt funksjon for å skrive inn verdier for debugging. | Må implementere innskriving av verdier. |
| 25/01/2026  |        | Jobbet i `button.cpp`. `buttonClick()` fungerer ikke som ønsket. Forsøker å lage status per knapp. Jobbet lenge uten å løse problemet. | Trenger pause etter mye feilsøking. |
| 26/01/2026  |        | Knappene fungerer. Meny‑navigasjon fungerer. Fjernet timerfunksjon. Planlagt alarm ved døråpning. Startet lysjustering og lysmeny. Temp‑meny fungerer. Begynt på lysmeny i `meny.cpp`. | Skal bruke phototransistor for lysjustering. Manuell overstyring ved langt trykk. |
| 28/01/2026  |        | Laget `lys.cpp` og `lys.g`. Funksjon for valg av lysfarge (rød, grønn, blå, hvit). Begynt på lys‑ og innstillingsmenyer. | Må lage dimmefunksjon. |
| 02/02/2026  |        | Begynt på flytskjema for lysmenyer. Laget menyer for standardfarger i `meny.cpp`. Ikke implementert fargeendring eller knappetrykk ennå. | Må lage rom‑innstillinger. |
| 04/02/2026  |        | Ingen loggført fremdrift. | «Glemt å loggføre». |
| 09/02/2026  |        | AI‑vurdering (B+). Refaktorert `tegnMeny()`, dokumentert tegn_*(), fjernet SENSOR3, ryddet dead code, fikset navigasjon i lysmenyer. Implementert `aktivLys()`. Ryddet i `meny.cpp`. | Må fikse fuktighetssensor og phototransistor. Begynne på dører/vinduer og servo. |
| 11/02/2026  |        | Laget `readDHT()` i `funksjoner.cpp`. Fungerer ikke. TMP36 problemer. DHT viser NaN. | Må fikse autoscroll. |
| 16/02/2026  |        | Temperaturproblem fikset. Begge sensorene oppdaterer hvert sekund. Endret temperaturmenyen. | Neste steg: servo‑innstillinger, manuell styring og dimming. |
| 25/02/2026  |        | Lysstyring fungerer i Tinkercad, men ikke på Arduino Uno. Store endringer i `lys.cpp`. Menyfeil i `meny.cpp`. Fjernet direkte kall til `aktivLys()`. Bruker nå statusvariabler. | Må feilsøke videre. |
| 02/03/2026  |        | Lys fungerer på Arduino Mega. Begynner på dimmefunksjon. | |
| 04/03/2026  |        | Jobbet på testfunksjon for dimming. Fungerer i Tinkercad. Ikke testet på Arduino ennå. | |