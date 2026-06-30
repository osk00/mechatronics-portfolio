/*
 * CExponentialFilter.h
 *
 *  Created on: Dec 1, 2023
 *      Author: osk
 */

#ifndef CEXPONENTIALFILTER_H_
#define CEXPONENTIALFILTER_H_

class CExponentialFilter {


private:
	float m_factor;
	float m_last;

public:

	/*
	 *Konstruktor für die Klasse CExponentialFilter.
	 *
	 *@param factor - Der Filterfaktor, der den Einfluss
	 *des aktuellen Werts auf den gefilterten Wert bestimmt.
	 */
	CExponentialFilter(float);

	/*
	 *Überlädt den << Operator, um einen neuen Wert zum
	 *Exponentialfilter hinzuzufügen.
	 *Der Operator aktualisiert den gefilterten Wert
	 *basierend auf dem Filterfaktor.
	 *
	 *@param value - Der neue Wert, der zum
	 *Exponentialfilter hinzugefügt werden soll.
	 *
	 *@return Eine Referenz auf das aktualisierte
	 *CExponentialFilter-Objekt.
	 */
	CExponentialFilter& operator<<(float);

	/*
	 *Überladener Umwandlungsoperator, der das
	 *CExponentialFilter in einen float-Wert umwandelt.
	 *Der resultierende Wert ist der aktuelle gefilterte Wert.
	 *
	 *@return Der aktuelle gefilterte Wert als float.
	 */
	operator float() const;

};

#endif /* CEXPONENTIALFILTER_H_ */
