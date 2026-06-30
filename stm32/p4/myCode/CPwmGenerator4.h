/*
 * CPwmGenerator4.h
 *
 *  Created on: 23.01.2024
 *      Author: Obada Alkhatib
 */

#ifndef CPWMGENERATOR4_H_
#define CPWMGENERATOR4_H_
#include "main.h"
#include "mbed.h"
#include <stdint.h>
/**
 * Mithilfe von Timer 3 wird ein Generator für ein PWM-Signal auf PC6
 * modelliert. Die Periode ist fest auf 16 Sekunden eingestellt,
 * mit einer Ausgangsfrequenz des Prescalers von etwa 2048 Hz und
 * einem Counter-Modulo von 16*2048. Das
 * Verhältnis von Ein- zu Ausschalten ist auf 10:6 konfiguriert.
 */
class CPwmGenerator4 {
private:
	/**
	 * PC6 wird als Alternate Function-Ausgang konfiguriert, und der durch
	 * Timer 3/Compare Register 1 generierte Wert wird als das auszugebende
	 * Signal ausgewählt.
	 */
	void configureGpioPin();

	/**
	 * Der Timer 3, einschließlich des Compare Register 1, wird so konfiguriert,
	 * dass ein Signal erzeugt wird, wie es in der Klassenbeschreibung
	 * dargestellt ist.
	 */
	void configureTimer();

	uint16_t m_period;

	DigitalOut* m_pwmOut = 0;

	/*
	 * Aktiviert Interrupts und erstellt einen Funktionszeiger
	 * auf den Interrupt-Handler.
	 *
	 */
	void configureInterrupt();
	/*
	 * Wenn das Update-Flag ausgelöst wird, erfolgt die Setzung von LED 7,
	 * und beim Auslösen des Compare-Flags wird LED 7 zurückgesetzt. Die Flags,
	 * die den Interrupt verursacht haben, werden ebenfalls zurückgesetzt.
	 */
	void handleInterrupt();
public:
	/*
	 * Erzeugt einen neuen PWM-Generator mit übergebener Periodendauer
	 *
	 * @param period die gewünschte Periodendauer in Mikrosekunden.
	 */
	CPwmGenerator4(uint16_t period);

	/*
	 * Legt das Verhältnis zwischen dem Rechtecksignal und der
	 * Pulsbreite (Duty Cycle) fest.
	 *
	 * @param percent das gewünschte Verhältnis in Prozent
	 */
	void setDutyCycle(uint8_t percent);

	/*
	 * Konfiguriert einen DigitalOut-Pin für die Ausgabe des PWM-Signals.
	 *
	 * @param pin Zeiger auf den DigitalOut-Pin
	 */
	void setPwmOut(DigitalOut* pin);

};

#endif /* CPWMGENERATOR4_H_ */
