/*
 * CPwmGenerator3.h
 *
 *  Created on: 23.01.2024
 *      Author: Obada Alkhatib
 */

#ifndef CPWMGENERATOR3_H_
#define CPWMGENERATOR3_H_

#include <stdint.h>
/**
 * Mithilfe von Timer 3 wird ein Generator für ein PWM-Signal auf PC6 modelliert.
 * Die Periode ist fest auf 16 Sekunden eingestellt, mit einer Ausgangsfrequenz des
 * Prescalers von etwa 2048 Hz und einem Counter-Modulo von 16*2048. Das
 * Verhältnis von Ein- zu Ausschalten ist auf 10:6 konfiguriert.
 */
class CPwmGenerator3 {
private:
	/**
	 * PC6 wird als Alternate Function-Ausgang konfiguriert, und der durch
	 * Timer 3/Compare Register 1 generierte Wert wird als das auszugebende Signal
	 * ausgewählt.
	 */
	void configureGpioPin();

	/**
	 * Der Timer 3, einschließlich des Compare Register 1, wird so konfiguriert,
	 * dass ein Signal erzeugt wird, wie es in der Klassenbeschreibung dargestellt ist.
	 */
	void configureTimer();

	uint16_t m_period;
public:


	//@param period die gewünschte Periodendauer in Mikrosekunden.
	CPwmGenerator3(uint16_t period);

	/*
	 * Legt das Verhältnis zwischen dem Rechtecksignal und der Pulsbreite (Duty Cycle) fest.
	 *
	 * @param percent das gewünschte Verhältnis in Prozent
	 */
	void setDutyCycle(uint8_t percent);
};

#endif /* CPWMGENERATOR3_H_ */

