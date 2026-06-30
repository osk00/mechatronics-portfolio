/*
 * CMovingAverageFilter.h
 *
 *  Created on: Dec 1, 2023
 *      Author: osk
 */

#ifndef CMOVINGAVERAGEFILTER_H_
#define CMOVINGAVERAGEFILTER_H_

#include "stdint.h"


class CMovingAverageFilter {

private:
	uint16_t* m_values;
	uint16_t m_valuesSize;
	uint16_t m_nextIndex = 0;
	uint32_t m_sum;

public:

	/*
	 *Konstruktor für die Klasse CMovingAverageFilter.
	 *@param size - Die Größe des Moving-Average-Filters
	 *(Anzahl der Werte, die berücksichtigt werden sollen).
	 */
	CMovingAverageFilter(uint16_t size = 16);


	/*
	 *Überlädt den << Operator, um einen neuen Wert zum
	 *Moving-Average-Filter hinzuzufügen.
	 *
	 *@param value - Der neue Wert, der zum Filter hinzugefügt
	 *werden soll.
	 *@return Eine Referenz auf das aktualisierte
	 *CMovingAverageFilter-Objekt.
	 */
	CMovingAverageFilter& operator<<(uint16_t);


	/*
	 *Überladener Umwandlungsoperator, der das
	 *CMovingAverageFilter in einen uint16_t-Wert umwandelt.
	 *Der resultierende Wert ist der Durchschnitt der im
	 *Filter gespeicherten Werte.
	 *
	 *@return Der Durchschnitt der im Filter gespeicherten
	 *Werte als uint16_t.
	 */
	operator uint16_t() const;

};

#endif /* CMOVINGAVERAGEFILTER_H_ */
