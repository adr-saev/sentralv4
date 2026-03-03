/*
 alarm.h

 Kortfattet beskrivelse:
	- Deklarasjoner for alarm-modulen: accessor-funksjoner, oppdatering,
		start/stopp og reset.
	- Alle funksjoner er designet for å brukes av hovedsketchen
		for å kontrollere og lese alarmstatus.

 Funksjoner:
	- getSec()/getMin()/getHour(): returnerer gjeldende nedtellingstid.
	- getAlarmState(): returnerer om alarmen er aktiv (1) eller ikke (0).
	- alarmFinished(): sjekker og returnerer true når alarmen har nådd 0.
	- oppdaterAlarm(): skal kalles regelmessig for å dekrementere tid.
	- startStoppAlarm(): starter eller stopper alarmen avhengig av state.
	- resetAlarm(): tilbakestiller mål-tid til 0.
	- endreTid(type, verdi): inkrementerer/ dekrementerer mål-tid.
*/

#ifndef ALARM_H
#define ALARM_H

int getSec();
int getMin();
int getHour();
int getAlarmState();
bool alarmFinished();

void oppdaterAlarm();
void startStoppAlarm();
void resetAlarm();
void endreTid(int type, int verdi);

#endif

