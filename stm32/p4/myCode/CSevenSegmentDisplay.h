/*
 * CSevenSegmentDisplay.h
 *
 *  Created on: 18.10.2020
 *      Author: mnl
 */

#ifndef CSEVENSEGMENTDISPLAY_H_
#define CSEVENSEGMENTDISPLAY_H_
#include "CShiftRegisterOutputExtender.h"
#include "mbed.h"




/**
 * Die Interrupt-Service-Routine (ISR) für den Timer TIM10, die aufgerufen wird,
 * wenn ein Update- oder Capture/Compare 1-Interrupt auftritt.
 * In dieser Funktion wird überprüft, welcher der beiden
 * Interrupts ausgelöst wurde.
 * Wenn der Capture/Compare 1-Interrupt ausgelöst wurde, wird das aktive
 * Dezimalzeichen der Siebensegmentanzeige ausgeschaltet.
 * Wenn der Update-Interrupt ausgelöst wurde, wird das nächste Dezimalzeichen
 * der Siebensegmentanzeige aktiviert.
 * Diese ISR wird in C-Funktionssignatur deklariert, um sicherzustellen,
 * dass der C++-Compiler den richtigen Aufrufmechanismus verwendet.
 */
extern "C" void TIM1_UP_TIM10_IRQHandler();

/**
 * Modelliert eine 4-stellige 7-Segment-Anzeige. Um eine "aktive" und
 * flackerfreie Darstellung zu erhalten, muss sichergestellt werden,
 * dass die Methode "activateNextDigit" mindestens alle 5 ms aufgerufen wird.
 */
class CSevenSegmentDisplay {

	friend void TIM1_UP_TIM10_IRQHandler();

private:

	static CSevenSegmentDisplay* m_instance;

	CShiftRegisterOutputExtender* m_ssdSegments;
	DigitalOut* m_digits[4];
	uint8_t m_activeDigit;
	uint8_t m_values[4];
	uint8_t m_digitPoint;
	int m_brightness;


	/**
	* Hilfsmethode für das Zeitmultiplexverfahren. Aktiviert die nächste
	* Anzeigestelle.
	*/
	void activateNextDigit();
	/**
	 * Konfiguriert die Interrupt-Einstellungen für TIM10.
	 * Diese Funktion aktiviert das TIM10-Interrupt im NVIC
	 * und richtet Update- und Capture/Vergleich-1-Interrupts ein.
	 */
	void configureInterrupt();

	/**
	 * Konfiguriert die Einstellungen des TIM10-Timers.
	 * Diese Funktion setzt den Prescaler, den Auto-Reload-Wert,
	 * die Anfangshelligkeit und den PWM-Modus für TIM10.
	 * Sie löst außerdem ein Update-Ereignis aus und löscht die Update-Flagge.
	 */
	void configureTimer();


public:



	static uint8_t patterns[];
	/**
	 * Erzeugt ein neues Objekt, das eine 4-stellige 7-Segment-Anzeige
	 * ansteuert, deren Segmente (und Dezimalpunkt) an das über
	 * ssdSegments ansteuerbare Schieberegister angeschlossen sind und
	 * deren Anzeigestellen über die übergebenen digitalen Ausgänge
	 * aktiviert werden können.
	 *
	 * @param ssdSegments die Modellierung des Schieberegisters zur
	 *  Ansteuerung der Segemente
	 * @param digits die Ausgänge zum Aktivieren der Anzeigestellen
	 */
	CSevenSegmentDisplay(CShiftRegisterOutputExtender* ssdSegments,
			DigitalOut* digits[4]);



	/**
	 * Ermöglicht den Zugriff auf die an der ausgewählten Anzeigestelle
	 * darzustellende Ziffer. Da eine Referenz auf den gespeicherten Wert
	 * zurückgeliefert wird, kann der Operator sowohl als Ziel einer
	 * Zuweisung als auch zum Abfragen des aktuell gespeicherten Wertes
	 * verwendet werden.
	 *
	 * @param position Index der Anzeigestelle (0 für Stelle ganz rechts)
	 * @return Referenz auf den an der ausgewählten Stelle anzuzeigenden Wert
	 */
	uint8_t& operator[] (int position);


	/*
	 *Setzt das Dezimalzeichen an der angegebenen Stelle
	 *Setzt  auf dem Siebensegmentdisplay.
	 *
	 *@param digit - Die Dezimalposition (0 bis 3), wobei 0 das
	 *rechte Dezimalzeichen und 3 das linke Dezimalzeichen ist.
	 */
	void setDecimalPoint(int digit);

	/*
	 *Setzt den Wert auf dem Siebensegmentdisplay entsprechend
	 *der angegebenen Dezimalzahl.
	 *@param value - Der Dezimalwert, der auf dem Siebensegmentdisplay
	 *angezeigt werden soll (0 bis 9999).
	 */
	void setValue(int value);

	/*
	 *Überlädt den Zuweisungsoperator, um einem Siebensegmentdisplay
	 *einen neuen Dezimalwert zuzuweisen.
	 *
	 *@param value - Der Dezimalwert, der auf dem Siebensegmentdisplay
	 *angezeigt werden soll.
	 *@return Eine Referenz auf das aktualisierte Siebensegmentdisplayobjekt.
	 */
	CSevenSegmentDisplay& operator=(int value);

	/*
	 *Setzt den Wert auf dem Siebensegmentdisplay entsprechend
	 *der angegebenen Gleitkommazahl.
	 *
	 *@param value - Der Gleitkommawert, der auf dem
	 *Siebensegmentdisplay angezeigt werden soll.
	 */
	void setValue(float value);

	/*
	 *Überlädt den Zuweisungsoperator, um einem Siebensegmentdisplay
	 *einen neuen Gleitkommawert zuzuweisen.
	 *
	 *@param value - Der Gleitkommawert, der auf dem Siebensegmentdisplay
	 *angezeigt werden soll.
	 *@return Eine Referenz auf das aktualisierte Siebensegmentdisplayobjekt.
	 */
	CSevenSegmentDisplay& operator=(float value);
	/**
	 * Initialisiert das TIM10 Timer-Peripheriegerät zur Steuerung
	 * einer Siebensegmentanzeige.
	 * Diese Funktion aktiviert den Takt für TIM10 und konfiguriert
	 * die Timer- und Interrupt-Einstellungen.
	 */
	void initTimer();
	/**
	 * Schaltet das aktuell aktive Dezimalsegment aus, wodurch die Anzeige
	 * praktisch ausgeschaltet wird.
	 * Diese Methode wird normalerweise während der Interrupt-Behandlung
	 * verwendet, um das aktive Dezimalzeichen auszuschalten.
	 */
	void deleteDigits();
	/**
	 * Setzt die Helligkeitsstufe der Siebensegmentanzeige.
	 * Diese Methode passt den Helligkeitsprozentsatz im
	 * gültigen Bereich (5-100) an.
	 *
	 * @param percent Die Helligkeitsstufe als Prozentsatz (5-100).
	 */
	void setBrightness(int percent);
	/**
	 * Ruft die aktuelle Helligkeitsstufe der Siebensegmentanzeige ab.
	 *
	 * @return Die Helligkeitsstufe als Prozentsatz.
	 */
	int getBrightness();
};

#endif /* CSEVENSEGMENTDISPLAY_H_ */
