/*
 * Autor: Obada Alkhatib
 * Mtr : 1111992
 * Datum: 17.01.2024
 */

//main.cpp

#include "mbed.h"
#include "breadboard.h"
#include "CPolledTimer.h"
#include "CDebouncer.h"
#include "CSevenSegmentDisplay.h"
#include "CHexShifter.h"
#include "CMeasuringDevice.h"

// Legen Sie die globalen Objekte, die die Peripheriekomponenten
// repräsentieren, hier an (vor den Funktionen).

// DigitalOut led1(LED1);

BusIn keys(BB_BTN_0,BB_BTN_1,BB_BTN_2,BB_BTN_3,
		BB_BTN_4,BB_BTN_5,BB_BTN_6,BB_BTN_7);

BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3,
		BB_LED_4, BB_LED_5, BB_LED_6, BB_LED_7);

DigitalOut ssdSerialData(SSD_SER);
DigitalOut ssdShiftRegisterClock(SSD_SCLK);
DigitalOut ssdBufferRegisterClock(SSD_RCLK);

DigitalOut ssdDigit0(SSD_DIGIT_0, OpenDrainNoPull);
DigitalOut ssdDigit1(SSD_DIGIT_1, OpenDrainNoPull);
DigitalOut ssdDigit2(SSD_DIGIT_2, OpenDrainNoPull);
DigitalOut ssdDigit3(SSD_DIGIT_3, OpenDrainNoPull);

/**
 * Implementierung der Lösung zur Aufgabe 1.
 */
void task1() {
	// Schreiben Sie vor der Enlosschleife den einmal auszuführenden Code

	BufferedSerial usart(PA_2, PA_3, 115200);
    while (true) {
    	// Schreiben Sie in der Endlosschleife den immer wieder auszuführenden
    	// Code
    	char data[] = { '*' };
    	usart.write(data, 1);
    }
}


/*
 * Die Funktion hat die Aufgabe, die Mikrocontroller mit
 * dem Rechner ueber Putty kommunizieren zu lasssen
 * und die Message auf die 7-Segment_Anzeige zeigen lassen.
 */
void task2(){
	//Initialisierung des Schiebregisters
		CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
				&ssdShiftRegisterClock, &ssdBufferRegisterClock);
		//Digits-Steuerpins in Array einsetzen.
		DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1,
				&ssdDigit2, &ssdDigit3};
		//Initialisierung der 7-Segment-Anzeige
		CSevenSegmentDisplay ssd(&ssdSegments, digits);
		CPolledTimer digitTimer(5);

		BufferedSerial usart(PA_2, PA_3, 115200);
		usart.set_blocking(false);
		CHexShifter application(&ssd, &keys, &leds, &usart);


		while(1){
			if(digitTimer.timeReached()){
				ssd.activateNextDigit();
			}
			application.poll();
		}
}//Ende


void task3(){
	//Initialisierung des Schiebregisters
		CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
				&ssdShiftRegisterClock, &ssdBufferRegisterClock);
		//Digits-Steuerpins in Array einsetzen.
		DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1,
				&ssdDigit2, &ssdDigit3};
		//Initialisierung der 7-Segment-Anzeige
		CSevenSegmentDisplay ssd(&ssdSegments, digits);
		CPolledTimer digitTimer(5);

		BufferedSerial usart(PA_2, PA_3, 115200);
		usart.set_blocking(false);
		AnalogIn poti(BB_POTI, 3.3);
		CMeasuringDevice application(&ssd, &keys,
				&leds, &usart, &poti);


		while(1){
			if(digitTimer.timeReached()){
				ssd.activateNextDigit();
			}
			application.poll();
		}
}//Ende

int main() {

	keys.mode(PullDown);

	//task1();
	//task2();
	task3();

	while (true) {
    }
}

