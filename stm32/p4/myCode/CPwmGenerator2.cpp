/*
 * CPwmGenerator2.cpp
 *
 *  Created on: 23.01.2024
 *      Author: Obada Alkhatib
 */

#include "CPwmGenerator2.h"
#include "mbed.h"

void CPwmGenerator2::configureGpioPin() {
	// GPIO-Pin PC6 auf alternate function 2 lonfigurieren.
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, 2 << GPIO_MODER_MODE6_Pos);
	MODIFY_REG(GPIOC->AFR[0], GPIO_AFRL_AFSEL6,
			GPIO_AF2_TIM3 << GPIO_AFRL_AFSEL6_Pos);
}

void CPwmGenerator2::configureTimer() {
	// Prescaler auf berechneten Wert für ca. 2048 Hz output setzen.
	TIM3->PSC = 41016 - 1;
	TIM3->ARR = 16 * 2048 - 1;

	// Compare register 1 auf 10 * 2048 setzen, PWM1 konfigurieren
	// und Ausgabe des OC1REF einschalten.
	//setDutyCycle(0);
	//TIM3->CCR1 = 10 << 11;
	//MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M, 6 << TIM_CCMR1_OC1M_Pos);
	//ET_BIT(TIM3->CCER, TIM_CCER_CC1E);

	// Update-Event zum Aktualisieren der gepufferten Register erzwingen,
	// das dadurch im SR gesetztes UIF löschen und Timer starten.
	SET_BIT(TIM3->EGR, TIM_EGR_UG);
	CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);

	TIM3->CCR1 = 10<<11;
	MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M, 6 << TIM_CCMR1_OC1M_Pos);
	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);
}

CPwmGenerator2::CPwmGenerator2() {
	// Timer mit Takt versorgen, sonst ist eine Konfiguration nicht möglich.
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CCR1 = 0;

	// Verwendete Peripheriegeräte konfigurieren.
	configureGpioPin();
	configureTimer();
}

void CPwmGenerator2::setDutyCycle(uint8_t percent) {

	TIM3->CCR1 = ((percent * TIM3->ARR) / 100);

	if (percent > 100){
		TIM3->CCR1 = TIM3->ARR;
	}

}










