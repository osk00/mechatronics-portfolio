//Autor: Obada Alkhatib, Mtr: 1111992

#include "mbed.h"
#include "breadboard.h"
#include "CShiftRegisterOutputExtender.h"
#include "CSevenSegmentDisplay.h"
#include "stdint.h"
#include "CPolledTimer.h"
#include "CMovingAverageFilter.h"
#include "CExponentialFilter.h"


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


/*
 * Die 7-Segment-Anzeige wird eingeschaltet.
 * je nachdemm welche Taste gedrueckt wird.
 * Es wird nur die erste Digit-Stelle angezeigt.
 */
void task1() {

	ssdDigit0 = 1;
	ssdDigit1 = 0;
	ssdDigit2 = 0;
	ssdDigit3 = 0;

	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	uint8_t newKeys;
	while (true) {
		newKeys = ~keys;
		ssdSegments = newKeys;
    }
}//Ende

/*
 * Die 7-Segment-Anzeige zeigt nun die Zahl 0 - 15
 * in Hexadezimalformat an. Der Wert der Zahl kann
 * ueber die Tasten definiert werden.
 */
void task2() {

	uint8_t patterns[16] = {215, 17, 143, 155, 89, 218,
			222, 145, 223, 219, 221, 94, 198, 31, 206, 204};

	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	ssdDigit0 = 1;
	ssdDigit1 = 0;
	ssdDigit2 = 0;
	ssdDigit3 = 0;

	uint8_t i;

	while (true) {
		leds = keys;
		i = leds & 0x0F;
		ssdSegments = ~patterns[i];
	}
}//Ende

/*
 *Es handelt sich um eine Routine, die periodisch
 *die 7 Segment-Anzeige aktualisiert, um die Ziffern
 *darzustellen. Der Timer steuert die Geschwindigkeit,
 *mit der die Anzeige zwischen den wechselt
 */
void task3(){

	//Initialisierung des Schiebregisters
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);
	//Digits-Steuerpins in Array einsetzen.
	DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	//Initialisierung der 7-Segment-Anzeige
	CSevenSegmentDisplay ssd(&ssdSegments, digits);
	CPolledTimer digitTimer(5);

	//Meine Mtr. Nr. : 111 1992
	ssd[0] = 1;
	ssd[1] = 9;
	ssd[2] = 9;
	ssd[3] = 2;

	ssd.setDecimalPoint(3);

	while(1){
		if(digitTimer.timeReached()){
			ssd.activateNextDigit();
		}
	}
}//Ende

/*
 *Die Funktion simuliert eine sich aendernde Dezimalzahl
 *auf der 7 Segment-Anzeige. Der Timer "digitTimer" steuert
 *die Geschwindigkeit des Wechsels zwischen Digits, waehrend
 *der Timer "showNext" steuert die Aktualisierung der Dezimalzahl
 */
void task4(){
	//Initialisierung des Schiebregisters
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);
	//Digits-Steuerpins in Array einsetzen.
	DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	//Initialisierung der 7-Segment-Anzeige
	CSevenSegmentDisplay ssd(&ssdSegments, digits);
	CPolledTimer digitTimer(5);
	CPolledTimer showNext(1000);
	//die Dezimalzahl
	float value = 0.0001234;
	ssd = value;

	while(1){
		if (digitTimer.timeReached()) {
		    ssd.activateNextDigit();
		    if (showNext.timeReached()) {
		    	//wenn true, wird "value" wieder von 0.0001234 anfangen.
		        value = (value == 12340) ? 0.0001234 : value * 10;
		        ssd = value;
		    }
		}
	}
}//Ende

/*
 *Die Funktion integriert die Anzeige von Ziffern auf 7 Segment-Anzeige
 *mit der Ueberwachung eines Poteniometers. Die LED wird aktiviert,
 *wenn die gemessene Spannung ausserhalb des vordefinierten Fensters liegt.
 */
void task5(){
	//Initialisierung des Schiebregisters
	 CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			 &ssdShiftRegisterClock, &ssdBufferRegisterClock);
	//Digits-Steuerpins in Array einsetzen.
	 DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	//Initialisierung der 7-Segment-Anzeige
	 CSevenSegmentDisplay ssd(&ssdSegments, digits);
	 CPolledTimer digitTimer(5);

	 AnalogIn poti(BB_POTI, 3.3);
	 CPolledTimer measurementTimer(20);

	 /*Die oberen und unteren Grenzwerte des
	  *Analog-Digital-Wandlers (ADC1) werden konfiguriert,
	  *und das AWDEN-Bit (Analog Watchdog Enable) im
	  *ADC1 Control Register (CR1) wird gesetzt,
	  *um den analogen Watchdog zu aktivieren.*/
	 ADC1->HTR = 3071;
	 ADC1->LTR = 1023;
	 ADC1->CR1 |= ADC_CR1_AWDEN;

	 while (1) {
	     if (digitTimer.timeReached()) {
	        ssd.activateNextDigit();
	     }
	     if (measurementTimer.timeReached()) {
	        ssd = poti.read_voltage();

	        /*Es wird überprüft, ob der
	         *analoge Watchdog-Status gesetzt ist.
	         *Falls ja, wird die LED eingeschaltet, und der
	         *Status des analogen Watchdogs wird zurückgesetzt.*/
	        if (ADC1->SR & ADC_SR_AWD) {
	            leds = 1;
	            ADC1->SR &= ~ADC_SR_AWD;
	        } else {
	        	//wenn nicht, dann die LED wird ausgeschaltet.
	            leds = 0;
	         }
	     }
	 }
}//Ende

/*
 *Die Funktion integriert eine filternde Funktion
 *fuer die Potentiometermessungen, um moegliche
 *Rauscheffekte zu reduziieren, waehrend es gleichzeitig
 *die Anzeige steurt.
 */
void task6() {
	//Initialisierung des Schiebregisters
    CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
    		&ssdShiftRegisterClock, &ssdBufferRegisterClock);
	//Digits-Steuerpins in Array einsetzen.
    DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	//Initialisierung der 7-Segment-Anzeige
    CSevenSegmentDisplay ssd(&ssdSegments, digits);
    CPolledTimer digitTimer(5);

    AnalogIn poti(BB_POTI, 3.3);
    CPolledTimer measurementTimer(20);

    /*Die oberen und unteren Grenzwerte des
   	  *Analog-Digital-Wandlers (ADC1) werden konfiguriert,
   	  *und das AWDEN-Bit (Analog Watchdog Enable) im
   	  *ADC1 Control Register (CR1) wird gesetzt,
   	  *um den analogen Watchdog zu aktivieren.*/
    ADC1->HTR = 3071;
    ADC1->LTR = 1023;
    ADC1->CR1 |= ADC_CR1_AWDEN;

    CMovingAverageFilter filter(100);
    float nVolt;

    while (1) {
        filter << poti.read_u16();

        if (digitTimer.timeReached()) {
            ssd.activateNextDigit();
        }

        if (measurementTimer.timeReached()) {
        	//wird der gefilterte analoge Wert in eine Spannung "newVolt"
        	//umgerechnet und auf dem Siebensegmentdisplay (ssd) angezeigt.
            nVolt = static_cast<float>(uint16_t(filter)) / 0xFFFF * 3.3;
            ssd = nVolt;

            /*Es wird überprüft, ob der
            *analoge Watchdog-Status gesetzt ist.
            *Falls ja, wird die LED eingeschaltet, und der
            *Status des analogen Watchdogs wird zurückgesetzt.*/
            if (ADC1->SR & ADC_SR_AWD) {
                leds = 1;
                ADC1->SR &= ~ADC_SR_AWD;
            } else {
            	//wenn nicht, dann die LED wird ausgeschaltet.
                leds = 0;
            }
        }
    }
}//Ende

/*
 *Die Funktion ist wie "task 6", aber hier wird mit einem ExponentialFilter
 *statt Mittelwert Filter umgesetzt.
 */
void task7(){
	//Initialisierung des Schiebregisters
    CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
    		&ssdShiftRegisterClock, &ssdBufferRegisterClock);
	//Digits-Steuerpins in Array einsetzen.
    DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	//Initialisierung der 7-Segment-Anzeige
    CSevenSegmentDisplay ssd(&ssdSegments, digits);
    CPolledTimer digitTimer(5);

    AnalogIn poti(BB_POTI, 3.3);
    CPolledTimer measurementTimer(20);

    /*Die oberen und unteren Grenzwerte des
   	  *Analog-Digital-Wandlers (ADC1) werden konfiguriert,
   	  *und das AWDEN-Bit (Analog Watchdog Enable) im
   	  *ADC1 Control Register (CR1) wird gesetzt,
   	  *um den analogen Watchdog zu aktivieren.*/
    ADC1->HTR = 3071;
    ADC1->LTR = 1023;
    ADC1->CR1 |= ADC_CR1_AWDEN;

    CExponentialFilter filter(0.001);
    float nVolt;

    while (1) {
        filter << poti.read_u16();

        if (digitTimer.timeReached()) {
            ssd.activateNextDigit();
        }

        if (measurementTimer.timeReached()) {
        	//wird der gefilterte analoge Wert in eine Spannung "newVolt"
        	//umgerechnet und auf dem Siebensegmentdisplay (ssd) angezeigt.
            nVolt = static_cast<float>(uint16_t(filter)) / 0xFFFF * 3.3;
            ssd = nVolt;

            /*Es wird überprüft, ob der
            *analoge Watchdog-Status gesetzt ist.
            *Falls ja, wird die LED eingeschaltet, und der
            *Status des analogen Watchdogs wird zurückgesetzt.*/
            if (ADC1->SR & ADC_SR_AWD) {
                leds = 1;
                ADC1->SR &= ~ADC_SR_AWD;
            } else {
            	//wenn nicht, dann die LED wird ausgeschaltet.
                leds = 0;
            }
        }
    }
}//Ende

int main() {

	keys.mode(PullDown);


	//task1();
	//task2();
	//task3();
	//task4();
	//task5();
	//task6();
	task2	();



    while (true) {
    }
}//Ende

