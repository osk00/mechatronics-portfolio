/*
 * CSevenSegmentDisplay.h
 *
 *  Created on: 18.10.2020
 *      Author: mnl
 */

#ifndef CSEVENSEGMENTDISPLAY_H_
#define CSEVENSEGMENTDISPLAY_H_

#include "CShiftRegisterOutputExtender.h"

/**
 * Modelliert eine 4-stellige 7-Segment-Anzeige. Um eine "aktive" und
 * flackerfreie Darstellung zu erhalten, muss sichergestellt werden,
 * dass die Methode "activateNextDigit" mindestens alle 5 ms aufgerufen wird.
 */
class CSevenSegmentDisplay {
private:

	CShiftRegisterOutputExtender* m_ssdSegments;
	DigitalOut* m_digits[4];
	uint8_t m_activeDigit;
	uint8_t m_values[4];
	uint8_t m_digitPoint;

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
	 * Hilfsmethode für das Zeitmultiplexverfahren. Aktiviert die nächste
	 * Anzeigestelle.
	 */
	void activateNextDigit();

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
};

#endif /* CSEVENSEGMENTDISPLAY_H_ */
