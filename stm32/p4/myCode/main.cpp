/*
 * Autor: Obada Alkhatib
 * Mtr.: 1111992
 * Datum: 23.01.2024
 */

//main.cpp


#include "mbed.h"
#include "main.h"
#include "breadboard.h"
#include "CPwmGenerator1.h"
#include "CPwmGenerator2.h"
#include "CPwmGenerator3.h"
#include "CPwmGenerator4.h"
#include "CExponentialFilter.h"
#include "CShiftRegisterOutputExtender.h"
#include "CSevenSegmentDisplay.h"
#include "CPolledTimer.h"
#include "CDebouncer.h"


// Legen Sie die globalen Objekte, die die Peripheriekomponenten
// repräsentieren, hier an (vor den Funktionen).

// DigitalOut led1(LED1);

BusIn keys(BB_BTN_0, BB_BTN_1, BB_BTN_2, BB_BTN_3,
		BB_BTN_4, BB_BTN_5, BB_BTN_6, BB_BTN_7);

AnalogIn poti(BB_POTI, 3.3);

DigitalOut ssdSerialData(SSD_SER);
DigitalOut ssdShiftRegisterClock(SSD_SCLK);
DigitalOut ssdBufferRegisterClock(SSD_RCLK);

DigitalOut ssdDigit0(SSD_DIGIT_0, OpenDrainNoPull);
DigitalOut ssdDigit1(SSD_DIGIT_1, OpenDrainNoPull);
DigitalOut ssdDigit2(SSD_DIGIT_2, OpenDrainNoPull);
DigitalOut ssdDigit3(SSD_DIGIT_3, OpenDrainNoPull);

CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);
/**
 * Realisiert mit CPwmGenerator1 auf PC6 ein PWM-Signal. Zusätzlich
 * werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 */
void task1() {
	// Die LEDs werden flexibler verwendet als in den vorherigen Laboren
	// und daher nicht als globales Objekt definiert sondern nach Bedarf
	// in den verschiedenen Tasks.
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);
	CPwmGenerator1 fixedGenerator;

    while (true) {
    	// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
    	leds = (TIM3->CNT >> 10) & 0x3f;
    }
}


/*
 * Beim Wechsel des Poti wird die Laufzeit der LED6 eingestellt.
 * Zusätzlich werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 */
void task2(){
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);
	CPwmGenerator2 fixedGenerator;
	CExponentialFilter filter(0.001f);
	uint8_t filterPercent;

	while(true){
		filter << poti.read_voltage();
		filterPercent = (filter / 3.3) * 100;
		fixedGenerator.setDutyCycle(filterPercent);
    	// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
		leds = (TIM3->CNT >> 10) & 0x3f;
	}
}

/*
 * Die Helligkeit von LED6 wird beim Umschalten des Poti eingestellt.
 * Zusätzlich werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 */
void task3(){
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);
	CPwmGenerator3 fixedGenerator(20000);
	CExponentialFilter filter(0.001f);
	uint8_t filterPercent;

	while(true){
		filter << poti.read_voltage();
		filterPercent = (filter / 3.3) * 100;
		fixedGenerator.setDutyCycle(filterPercent);
    	// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
		leds = (TIM3->CNT >> 10) & 0x3f;
	}
}

/*
 * Beim Umschalten des Poti wird die Helligkeit sowohl von LED6 als
 * auch von LED7 eingestellt.
 * Zusätzlich werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 */
void task4(){
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);
	CPwmGenerator4 fixedGenerator(20000);
	CExponentialFilter filter(0.03);
	uint8_t filterPercent;
	DigitalOut led7(PC_7);


	fixedGenerator.setPwmOut(&led7);

	while(true){
		filter << poti.read_voltage();
		filterPercent = (filter / 3.3) * 100;
		fixedGenerator.setDutyCycle(filterPercent);
		// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
		leds = (TIM3->CNT >> 10) & 0x3f;
	}
}

/**
 * Diese Funktion repräsentiert die Task 5 des Hauptprogramms.
*  In dieser Aufgabe wird eine Siebensegmentanzeige initialisiert und
*  konfiguriert, um die Zahl 1992(Mtr. Nr.) anzuzeigen.
*  Die Dezimalpunktposition wird auf -1 gesetzt, was bedeutet, dass
*  kein Dezimalpunkt angezeigt wird.
*  Eine LED (led0) wird ebenfalls initialisiert.
*  Die Helligkeit der Siebensegmentanzeige wird auf 100% gesetzt.
*  Die Funktion befindet sich in einer Endlosschleife, die
*  periodisch die CPU in den Schlafmodus versetzt, um Strom zu sparen.
*  Währenddessen wird die LED ein- und ausgeschaltet, um
*  den Betriebszustand anzuzeigen.
*/
void task5() {
    CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
            &ssdShiftRegisterClock, &ssdBufferRegisterClock);

    DigitalOut *digits[] = { &ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3 };
    CSevenSegmentDisplay ssd(&ssdSegments, digits);


    ssd = 1992;
    ssd.setDecimalPoint(-1);
    DigitalOut led0(BB_LED_0);
    ssd.setBrightness(100);
    while (true) {
        __disable_irq();
        led0 = 0;
        __WFI();
        led0 = 1;
        __enable_irq();
    }
}


/**
 * Diese Funktion repräsentiert die Aufgabe 6 im Hauptprogramm.
 * In dieser Aufgabe wird eine Siebensegmentanzeige (SSD) initialisiert
 * und konfiguriert.
 * Die SSD zeigt den Wert 1992 an und setzt die Dezimalpunkt-Position auf
 * -1 (kein Dezimalpunkt).
 * Eine LED (led0) wird ebenfalls initialisiert.
 * Ein Debouncer für Tasten, ein Timer und eine Prozentvariable
 * werden eingerichtet.
 * Die Helligkeit der SSD wird auf 100% eingestellt.
 * Die Funktion befindet sich in einer Endlosschleife, die den
 * Tastenzustand überwacht und die Helligkeit periodisch ändert.
 * Die Helligkeit wird zwischen 5% und 100% zyklisch verändert.
 */
void task6(){

	DigitalOut* digits[4] = {&ssdDigit0, &ssdDigit1, &ssdDigit2, &ssdDigit3};
	CSevenSegmentDisplay ssd(&ssdSegments, digits);

	ssd = 1992;
	ssd.setDecimalPoint(-1);
	DigitalOut led0(BB_LED_0);
	CDebouncer debouncer(&keys, 10);
	CPolledTimer timer(50);
	ssd.setBrightness(100); // Initial value
	int percent = ssd.getBrightness();
	uint8_t keyStates;
	bool direction = true;

	while (true) {
	    keyStates = debouncer.read();

	    if (keyStates == 128) {
	        if (timer.timeReached()) {
	        	//Helligkeit je nach Richtung erhöhen oder verringern
	            percent = (direction) ? percent - 1 : percent + 1;
	            ssd.setBrightness(percent);
	        }
	    }
	    // Zyklisches Ändern der Helligkeit zwischen 5% und 100%
	    if (percent == 100) {
	        direction = true;
	    } else if (percent == 5) {
	        direction = false;
	    }
	}
}
/*
 * Beim Druck der Taste0 wird die Helligkeit der SSD einstellbar sein.
 * Wenn die Helligkeit noch nicht das maximal erreicht hat, wird die
 * Helligkeit erhöht das Programm die Helligkeit alle 50 ms um 1%,
 * und das Gegensatz wenn die Helligkeit noch nicht das minimal
 * erreicht hat.
 */
/**
 * Funktion, die der Timer 3 IRQ Handler aufrufen soll. Ermöglicht
 * die Konfiguration unterschiedlicher effektiver IRQ Handler je
 * nach Nutzung des Timers.
 *
 * Die aufzurufende Funktion muss im Rahmen der Konfiguration des
 * Interrupts zugewiesen werden.
 */
function<void()> activeTIM3_IRQHandler;

/**
 * Interrupt-Handler für Timer 3. Der Handler ruft die Funktion auf,
 * die der globalen Variablen activeTIM3_IRQHandler zugewiesen wurde.
 */
extern "C" void TIM3_IRQHandler() {
	activeTIM3_IRQHandler();
}

/**
 * Haupt- oder Einstiegsfunktion des Programms. Wird beim Start nach der
 * Initialisierung der globalen Variablen (bei Objekten: Aufruf des
 * Konstruktors) ausgeführt.
 */
int main() {
	// Falls notwendig, ergänzen Sie hier Aufrufe, um Konfigurationen
	// an globalen Objekten vorzunehmen, die nicht durch die Angabe von
	// Argumenten beim Aufruf des Konstruktors vorgenommen werden können.
	keys.mode(PullDown);

	// Aufruf der Funktion, die die Lösung der gerade bearbeiteten Aufgabe
	// implementiert.
	//task1();
	//task2();
	//task3();
	//task4();
	//task5();
	task6();


    while (true) {
    }
}

