/*
 * CPwmTest1.h
 *
 *  Created on: 19.12.2020
 *      Author: mnl
 */

#ifndef CPWMGENERATOR1_H_
#define CPWMGENERATOR1_H_

/**
 * Nutzt Timer 3, um einen Generator für ein PWM-Signal auf PC6 zu
 * modellieren. Die Signalperiode ist fix auf 16 Sekunden eingestellt,
 * wobei die Ausgangsfrequenz des Prescalers etwa 2048 Hz beträgt
 * und der Counter-Modulo 16*2048 beträgt. Das Konfigurationsverhältnis
 * von Ein- und Ausschalten ist auf 10:6 festgelegt.
 */
class CPwmGenerator1 {
private:
	/**
	 * Konfiguriert PC6 als Alternate Function-Ausgang und wählt den Wert
	 * des von Timer 3/Compare Register 1 generierten Signals aus.
	 */
	void configureGpioPin();

	/**
	 * Konfiguriert Timer 3 einschließlich Compare Register 1, um ein Signal
	 * gemäß der Klassenbeschreibung zu erzeugen.
	 */
	void configureTimer();

public:

	CPwmGenerator1();
};

#endif /* CPWMGENERATOR1_H_ */
