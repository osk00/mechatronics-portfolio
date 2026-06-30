/*

Autor: Obada Alkhatib , Mtr.Nr. : 1111992 , Datum: 18.11.2023

 */

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
		//die neue gedrueckte Tasten werden mit aktuelle leds in XOR verglichen.
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
 *Task 5 hat die Funktion, die Leds in endlosen LEDs Laufs nach links zu setzen.
 * */
void task5() {

	uint8_t bitmuster = 0x01;

	while(true){

		leds = bitmuster;
		thread_sleep_for(1000);
		//Ist das letzte Bit gleich 1, dann sollte Bitmuster wieder neu anfangen.
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

		int64_t actualTime = std::chrono::duration_cast<std::chrono::milliseconds>(HighResClock::now().time_since_epoch()).count();

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
 *Task9 hat die Funktion das Lauflicht konfigurierbar zu machen, indem
 *man ueber einen Variable, der den Wert von -7 bis 7 eingestellt werden kann
 *und mit dem die Geschwindigkeit steurt.
 *Bei diesem Task wird 7 Tasten implemnetiert.
 *4 Tasten fuer eine dirket Einstellung
 *1 Taste fuer Erhoehung den Wert um 1
 *1 Taste fuer Verminderung den Wert um 1
 *und eine Taste noch fuer Richtung Wechsel.
 * */
void task9(){

	CPolledTimer chasingLightTimer(250);
	CDebouncer keys_D(&keys, 150);
	uint8_t previousKeysState = keys_D;
	uint8_t newKeysState = 0x00;
	uint8_t keysPushed;
	int8_t reglerWert = 0b0001;
	uint8_t laufLeds = 0x80;
	uint8_t reglerLeds = 0x01;

	//hier wird die LEDs auf zwei Untergruppen geteilt
	leds = laufLeds | reglerLeds;

	while(true){

		//Teil 1: + - und RichtungWechsel Tasten
		//falls 5. Taste gedrueckt wird, sollte inkriminierung auftauchen
		if(keysPushed || reglerWert ==0x00){
			if(keys_D== 0x10){
				if(reglerWert < 7){
					reglerWert++;
					//thread_sleep_for(200);
				}
			}
			//falls 6.Taste gedrueckt wird, sollte sdiskriminerung auftauchen.
			if(keys_D == 0x20){
				if(reglerWert > -7){
					reglerWert--;
					//thread_sleep_for(200);
				}
			}
			//ist vorletzte Taste gedruckt, sollte eine Richtung Wechsel auftauchen.
			if(keys_D == 0x40 ){
				reglerWert = (-1*reglerWert);
				//thread_sleep_for(200);
			}
		}

		//Teil 2: Dirketeinstellung
		newKeysState = int(keys_D);
		keysPushed = (reglerWert ^ (newKeysState & ~previousKeysState)) & 0x0F;
		//Falls die vierte gedruckt wird, sollte dann zweite Komplement angwwendet.
		reglerWert = (keysPushed > 7)?
				keysPushed ^ 0xF0 : keysPushed & 0x0F;
		previousKeysState = newKeysState;
		//update regler leds nach regler Wert
		reglerLeds = reglerWert & 0x0F;

		//Teil 3 von der Zeile 279 bis 295, Lauflicht
		if(chasingLightTimer.timeReached()){
			//wenn ReglerWert groesser als 0 sollte die Leds nach links bewegen
			if(reglerWert > 0){

				laufLeds = (laufLeds == 0x80) ?
						0x10 : laufLeds << 1;
			}else if(reglerWert < 0){

				laufLeds = (laufLeds == 0x10) ?
						0x80 : laufLeds >> 1;
			}else if (reglerWert == 0){
				//Falls reglerWert =0 dann sollte alle Leds ebenfalls gleich 0 sein
				laufLeds = 0x00;
			}
			//update leds Status
			leds = laufLeds | reglerLeds;
			chasingLightTimer.updateWaitTime(250*abs(reglerWert));
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
	task9();

	while (true) {}
}

