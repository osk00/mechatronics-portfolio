//Autor: Obada Alkhatib , Mtr.Nr. : 1111992

#include "mbed.h"
#include "breadboard.h"
#include "CPolledTimer.h"
#include "stdint.h"
#include "CDebouncer.h"



//Hier wird ein Objekt von Typ BusOut definiert.
BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3,
		BB_LED_4, BB_LED_5, BB_LED_6, BB_LED_7);

//Hier wird ein Objekt von Typ BusIn definiert.
BusIn keys(BB_BTN_0, BB_BTN_1, BB_BTN_2, BB_BTN_3,
		BB_BTN_4, BB_BTN_5, BB_BTN_6, BB_BTN_7);

/*
 * Die Task1-Funktion sollte die 8 LEDs alle 100 ms ein- und ausschalten
 * Sie haben hier 8 LEDs, die 2^8 = 256 Möglichkeiten entsprechen und so
 *  erhalten Sie eine Reihe von 8 Einsen in Binärzahl und somit
 * die alle 8 LEDs einschalten kann.
 * 256 in Dezimalzahl entspricht FF in Hexadezimal.
 */
void task1() {

	while (true) {
		leds = 0xFF; //entspricht 255 in Dezimalzahl
		thread_sleep_for(100);
		leds = 0x00; //entspricht 0 in Dezimalzahl
		thread_sleep_for(100);
	}
}//Ende

/*
 * Die task2-Funktion sollte den Tasten die Steuerung von LEDs geben,
 * da die "leds"-Größe in Bits gleich der "keys"-Größe in Bits ist.
 */
void task2() {

	while (true) {
		//die bits das leds ist dann das gleiche was die Tasten haben.
		leds = keys;
	}
}//Ende

/*
 *Hier werden die LEDS dirket auf gehalten Tasten angemacht.
 * */
void task3() {

	uint8_t previousKeyStates = keys; //muss ein Wert dazu zugewiesen wird.
	uint8_t newKeyStates;
	uint8_t keysPushed;

	while(true){
		// newKeyStates entpsricht eingabe der Tasten.
		newKeyStates = keys;
		//hier wird nur die Aenderung gesehen wird.
		keysPushed = newKeyStates & ~previousKeyStates;
		/*die neue gedrueckte Tasten
		 *werden mit aktuelle leds in XOR verglichen.*/
		leds = leds ^ keysPushed;
		// Die akteulle bits Ordnung wird als previous betrachtet.
		previousKeyStates = newKeyStates;
		//den Loop laueft weiter so.
	}
}//Ende

/*
 *Diieser Task ist genauso gleich wie Task3 nur der Unterschied
 *ist, dass den Prinzip Debouncer hier verwendet wird.
 * */
void task4() {

	CDebouncer keysDebouncer(&keys, 150);
	uint8_t previousKeyStates = 0x00;
	uint8_t newKeyStates;
	uint8_t keysPushed;

	while(true){

		newKeyStates = int(keysDebouncer);
		keysPushed = newKeyStates & ~previousKeyStates;
		leds = leds ^ keysPushed;
		previousKeyStates = newKeyStates;
	}
}//Ende

/*
 *Task 5 hat die Funktion,
 *die Leds in endlosen LEDs Laufs nach links zu setzen.
 */
void task5() {

	uint8_t bitmuster = 0x01;

	while(true){

		leds = bitmuster;
		thread_sleep_for(1000);
		/*Ist das letzte Bit gleich 1,
		 * dann sollte Bitmuster wieder neu anfangen.*/
		bitmuster = (bitmuster == 0x80) ?
				0x01 : bitmuster << 1;
	}
}//Ende

/*
 *Task 6 ist genauso gleich wie task5, aber hier wird
 *die Waittime Prinzip implemntiert, indem man die vergange Zeit messt
 *und die als Nebentaetigkeit benutzt.
 * */
void task6() {
	uint8_t Bitmuster = 0x01;
	int64_t waitTime = 1000;

	int64_t targetTime = std::chrono::duration_cast<std::chrono::milliseconds>
	(HighResClock::now().time_since_epoch()).count() + waitTime;

	while(true){
		leds.write(Bitmuster);

		int64_t actualTime = std::chrono::duration_cast
				<std::chrono::milliseconds>
				(HighResClock::now().time_since_epoch()).count();

		if(actualTime >= targetTime){

			Bitmuster = Bitmuster << 1;
			if(Bitmuster == 0x00){
				Bitmuster = 0x01;
			}
			else
			{
				leds.write(Bitmuster);
			}
			targetTime = actualTime + waitTime;

		}
	}
}//Ende


/*
 *Task 7 funktioniert genauso wie Task 6, aber hat ein zusaetzliche Vorteil
 *,dass der Task 7 die Klasse CPolledTimer dirket nutzt und den Timer Prinzip
 *durch die Elementfunktion timeReached realisiert.
 * */
void task7() {
	CPolledTimer twoHz(500);
	CPolledTimer threeHz(333);
	CPolledTimer fourHz(250);
	while (true) {
		if (twoHz.timeReached()) {
			leds = leds ^ (1 << 0);
		}
		if (threeHz.timeReached()) {
			leds = leds ^ (1 << 1);
		}
		if (fourHz.timeReached()) {
			leds = leds ^ (1 << 2);
		}
	}

}//Ende


/*
 *Task 8 hat das gleiche Verhalten wie Task 6,
 *aber hier wird ein Objekt instialsiert und durch dem eine Warte Zeit
 *eingesetzt. wird.
 * */

void task8() {

	uint8_t bitmuster = 0x01;
	CPolledTimer chasingLightTimer(1000);

	while(true){
		leds = bitmuster;

		if(chasingLightTimer.timeReached()){

			bitmuster = (bitmuster == 0x80) ?
					0x01 : bitmuster << 1;
			chasingLightTimer = CPolledTimer(1000);
		}
	}
}//Ende


/*
 * Ich habe die Funkton "task9()" von Grund auf wieder implementiert,
 * da mein letzte Code nicht richtig geschrieben wurde. Diese Code
 * funktioniert effizienter und wird nach Bedarf in das Durhfuehrung-pdf
 * geschrieben.
 */
/*Die Funktion hat die Aufgabe, die 4-Leds auf der linken Seite wahlweise
 * nach links oder rechts zu bewegen und wird mit 7 Tasten gesteuert.
 * Die 1. bis 4. Taste sind fuer dirkete Einstellung zustaendig
 * 5.Taste Inkrementierung
 * 6.Tasete Dekrementierung
 * 7.Taste Vorzeichenwechsel
 * Dann sind die 4 Leds auf der rechten Seite fuer die Anzeige der
 * Geschwindigkeit in Bits zustaendig.
 * Die Negation wird nach der Negation des Zweierkomplement implementiert.
 *  Geschwindigkeitsbereich liegt zwischen -7 und 7
 */
void task9(){
	//Fuer die Steuerung von der Geschwindigkeit der Lauf-Leds
	int8_t speed = 0b0001;
	//Fuer die lauf-Leds
	uint8_t RunningLeds = 0x80;
	//Tasten Ablesung, wie im Task 4
	uint8_t previousKeyStates = keys;
	uint8_t newKeyStates;
	uint8_t keysPushed;

	int debouncerWaitTime = 100;

	//Ein Objekt von CPolledTimer und CDebouncer werden hier definiert.
	CPolledTimer chasingLightTimer(250);
	CDebouncer Debouncer(&keys, debouncerWaitTime);

	while(1){

	//Tasten ablesen
	newKeyStates = int(Debouncer);
	keysPushed = ~previousKeyStates & newKeyStates;
	previousKeyStates = newKeyStates;

	//Die 8-Leds werden hier aktualisiert
	leds = (RunningLeds & 0xF0) | (speed & 0x0F);

	//Teil 1: "+" ,  "-"  ,  "<>"   Tasten

		// "+"
		if(((keysPushed & 0xF0) == 0x10) && speed < 7){
			 speed++;
			 //Die 8-Leds werden hier wieder fuer bessere
			 //Reaktionszeit der Geschwindigkeit-Leds aktualisiert.
			 leds = (RunningLeds & 0xF0) | (speed & 0x0F);
			 /*"waitTime" wird hier abhaengig von
			  * absolute "speed" aktualisiert.*/
			 chasingLightTimer.updateWaitTime(250*abs(speed));
		}

		// "-"
		if(((keysPushed & 0xF0) == 0x20) && speed > -7){
			 speed--;
			 //Die 8-Leds werden hier wieder fuer bessere
			 //Reaktionszeit der Geschwindigkeit-Leds aktualisiert.
			 leds = (RunningLeds & 0xF0) | (speed & 0x0F);
			 /*"waitTime" wird hier abhaengig von
			  * absolute "speed" aktualisiert.*/
			 chasingLightTimer.updateWaitTime(250*abs(speed));
		}

		// "<>"
		if((keysPushed & 0xF0) == 0x40){
			speed = -speed;
			 //Die 8-Leds werden hier wieder fuer bessere
			 //Reaktionszeit der Geschwindigkeit-Leds aktualisiert.
			leds = (RunningLeds & 0xF0) | (speed & 0x0F);
		}

	//Teil 2: Dirkete Einstellung
		if((keysPushed & 0x0F) !=0){
			/*Die 4.Taste ist fuer das Vorzeichen gedacht,
			 * funktioniert also wie die Wechsel-Taste und
			 * sorgt dafuer, dass der Variable "speed" im
			 * Bereich -7 .... 7 bleibt.*/
			if(keysPushed == 0x08){
				speed = -speed;
			}else{
				speed = ((keysPushed & 0x0F) ^ speed);
			}
			/*"waitTime" wird hier abhaengig von
			 * absolute "speed" aktualisiert.*/
			 chasingLightTimer.updateWaitTime(250*abs(speed));
		}

	//Teil 3: Lauf-Leds
		if(chasingLightTimer.timeReached()){
			if(speed > 0){
				/*falls Lauf-Leds "RunningLeds" gleich 0 ist,
				 *braucht der Variable "RunningLeds"
				 *wieder eine Anfangspunkt*/
				if(RunningLeds == 0){
					RunningLeds = 0x80;
				}
				//Verschiebt die beleuchtende Led einmal nach rechts
				RunningLeds = (RunningLeds == 0x10) ?
					0x80 : RunningLeds >> 1;
			} else if(speed < 0){
				/*falls Lauf-Leds "RunningLeds" gleich 0 ist,
				 *braucht der Variable "RunningLeds"
				 *wieder eine Anfangspunkt*/
				if(RunningLeds == 0){
					RunningLeds = 0x80;
				}
				//Verschiebt die beleuchtende Led einmal nach links
				RunningLeds = (RunningLeds == 0x80) ?
					0x10 : RunningLeds << 1;
			} else{
				/*Falls "speed" gleich 0 ist,
				 *werden alle Leds damit ausgeschaltet.*/
				RunningLeds = 0x00;
			}
			//Die 8-Leds werden hier aktualisiert
			leds = (RunningLeds & 0xF0) | (speed & 0x0F);
		}
	}
}//Ende

int main() {
	// Diese Elementfunktion setzt das BusIn-Objekt auf PullDown
	keys.mode(PullDown);
	//task1();
	//task2();
	//task3();
	//task4();
	//task5();
	//task6();
	//task7();
	//task8();
	//task9();

	while (true) {}
}

