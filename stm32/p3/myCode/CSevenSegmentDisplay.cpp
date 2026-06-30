/*
 * CSevenSegmentDisplay.cpp
 *
 *  Created on: 18.10.2020
 *      Author: mnl
 */

#include "CSevenSegmentDisplay.h"


uint8_t CSevenSegmentDisplay::patterns[] = {215, 17, 143, 155, 89, 218,
		222, 145, 223, 219, 221, 94, 198, 31, 206, 204};

CSevenSegmentDisplay::CSevenSegmentDisplay(
		CShiftRegisterOutputExtender *ssdSegments, DigitalOut* digits[4]) {
	// Übergeben Werte in Attribten speichern
	m_ssdSegments = ssdSegments;
	for (int digit = 0; digit < 4; digit++) {
		m_digits[digit] = digits[digit];
	}

	// Array mit anzuzeigenden Ziffern initialisieren
	for (int digit = 0; digit < 4; digit++) {
		m_values[digit] = 0;
	}

	// Als Startzustand wird Stelle 0 aktiviert und alle Segmente werden
	// ausgeschaltet.
	*m_digits[0] = 1;
	for (int digit = 1; digit < 4; digit++) {
		*m_digits[digit] = 0;
	}
	m_activeDigit = 0;
	*m_ssdSegments = 0xff;
}

void CSevenSegmentDisplay::activateNextDigit() {

     //Berechnung des Index des nächsten Dezimalzeichens
	 //im Zyklus (0 bis 3).
	 int nextDigitIndex = (m_activeDigit + 1) % 4;
	 //Abrufen des Werts des nächsten Dezimalzeichens.
	 int nextDigitValue = m_values[nextDigitIndex];

	 //Vorbereiten der Ausgabe für die Siebensegmentsegmente
	 //unter Berücksichtigung des Dezimalpunkts.
	 m_ssdSegments->prepareOutput(~(patterns[nextDigitValue] |
	  	(m_digitPoint == nextDigitIndex ? 32 : 0)));
	 //Deaktivieren des aktuellen Dezimalsegments.
	 *m_digits[m_activeDigit] = 0;
	 //Aktualisieren des aktiven Dezimalsegments
     m_activeDigit = nextDigitIndex;

     // Aktualisieren der Ausgabe auf dem 7-Segment-Anzeige.
     m_ssdSegments->updateOutput();
     //Aktualisieren des aktiven Dezimalsegments
	 *m_digits[m_activeDigit] = 1;
}



uint8_t& CSevenSegmentDisplay::operator[] (int position) {
	static uint8_t dummy = 0;
	if (position < 0 || position > 3) {
		return dummy;
	}
	return m_values[position];
}

void CSevenSegmentDisplay::setDecimalPoint(int digit){
	if((digit > -1) && (digit < 4)){
		m_digitPoint = 3 - digit;
	} else if(digit == -1) {
		m_digitPoint = -1;
	}
}

void CSevenSegmentDisplay::setValue(int value){

	if (value < 0 || value > 9999) {
	    for (int digit = 0; digit < 4; digit++) {
	        m_values[digit] = 14;
	    }
	} else {
	    for (int digit = 3; digit >= 0; digit--) {
	        m_values[digit] = value % 10;
	        value /= 10;
	    }
	}
}

CSevenSegmentDisplay& CSevenSegmentDisplay::operator=(int value){
	setValue(value);
	return *this;
}

void CSevenSegmentDisplay::setValue(float value){

	 int point = 0;

	 while (value < 1000 && point < 3) {
	     point++;
	     value *= 10;
	 }

	 setValue(int(value));
	 setDecimalPoint(point);
}

CSevenSegmentDisplay& CSevenSegmentDisplay::operator=(float value){
	setValue(value);
	return *this;
}

