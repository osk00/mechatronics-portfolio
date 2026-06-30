/*
 * CPwmGenerator4.cpp
 *
 *  Created on: 23.01.2024
 *      Author: Obada Alkhatib
 */


#include "CPwmGenerator4.h"
#include "mbed.h"

void CPwmGenerator4::configureGpioPin() {
	// GPIO-Pin PC6 auf alternate function 2 lonfigurieren.
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, 2 << GPIO_MODER_MODE6_Pos);
	MODIFY_REG(GPIOC->AFR[0], GPIO_AFRL_AFSEL6,
			GPIO_AF2_TIM3 << GPIO_AFRL_AFSEL6_Pos);
}

void CPwmGenerator4::configureTimer() {
    // Clock für den Timer TIM3 aktivieren
	// (TIM3EN Bit im APB1ENR Register setzen)
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Prescaler-Wert (Teiler für die Timer-Taktquelle) setzen,
    // um eine Frequenz von ca. 2048 Hz zu erreichen.
    // Der Wert 84 wird verwendet, da die Basisfrequenz des APB1-Busses
    // normalerweise 84 MHz beträgt.
    // Der Timer zählt dann mit einer Frequenz
    // von SystemClock / (PSC + 1) = 1 MHz.
    TIM3->PSC = 84 - 1;

    // Auto-Reload-Wert (ARR) setzen, um die gewünschte Periodendauer
    // minus 1 festzulegen.
    // Dieser Wert bestimmt die Gesamtdauer des Timerzyklus, bevor er
    // wieder von vorne beginnt.
    TIM3->ARR = m_period - 1;

    // Compare-Wert für das Vergleichsregister (CCR1) setzen, optional
    // für benutzerdefinierte Konfigurationen.
    // Dieser Wert wird hier fest auf 10 << 11 gesetzt.
    TIM3->CCR1 = 10 << 11;

    // PWM-Modus 1 für Kanal 1 (CCMR1 Register) konfigurieren.
    // Dieser Modus setzt den Ausgang auf 'aktiv bei Übereinstimmung'.
    MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M, 6 << TIM_CCMR1_OC1M_Pos);

    // Ausgabe des PWM-Signals für Kanal 1 aktivieren
    //(CC1E Bit im CCER Register setzen).
    SET_BIT(TIM3->CCER, TIM_CCER_CC1E);

    // Update-Event erzwingen, um die gepufferten Register zu
    // aktualisieren und den Timer zu starten.
    SET_BIT(TIM3->EGR, TIM_EGR_UG);

    // Das Update-Flag (UIF) im Statusregister (SR) löschen, um
    // unerwünschte Ereignisse zu vermeiden.
    CLEAR_BIT(TIM3->SR, TIM_SR_UIF);

    // Timer aktivieren, indem das CEN-Bit (Counter Enable) im
    // Control Register 1 (CR1) gesetzt wird.
    SET_BIT(TIM3->CR1, TIM_CR1_CEN);
}

CPwmGenerator4::CPwmGenerator4(uint16_t period) {

	m_period = period;

	// Verwendete Peripheriegeräte konfigurieren.
	configureGpioPin();
	configureTimer();
	configureInterrupt();
}

void CPwmGenerator4::setDutyCycle(uint8_t percent) {
    // Überprüfen, ob der angegebene Prozentsatz größer oder gleich 100 ist.
    // Wenn ja, setze den Prozentsatz auf 100, um sicherzustellen,
	//dass er nicht größer als 100 ist.
    if (percent >= 100){
        percent = 100;
    }

    // Berechne den Compare-Wert (CCR1) basierend auf dem Prozentsatz
    // und dem Auto-Reload-Wert (ARR).
    // Der Prozentsatz wird zuerst mit dem Auto-Reload-Wert multipliziert
    // und dann durch 100 geteilt.
    // Der Compare-Wert bestimmt den Pulsweitenmodulationsanteil des Signals
    // und beeinflusst die Helligkeit oder den Zustand des Ausgangs.
    TIM3->CCR1 = ((TIM3->ARR + 1) * percent) / 100;
}

void CPwmGenerator4::configureInterrupt(){
	activeTIM3_IRQHandler = bind(&CPwmGenerator4::handleInterrupt, this);

	__NVIC_EnableIRQ(TIM3_IRQn);
	SET_BIT(TIM3->DIER, TIM_DIER_UIE);
	SET_BIT(TIM3->DIER, TIM_DIER_CC1IE);
}


void CPwmGenerator4::handleInterrupt() {
	// Prüft, ob Update-Flag ausgelöst ist. Wenn das der Fall ist, setzt
	// Update-Flag zurück und setzt LED 7.
	if(READ_BIT(TIM3->SR, TIM_SR_UIF)) {
		CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
		*m_pwmOut = 1;
	}

	// Prüft, ob Compare-Flag ausgelöst ist. Wenn das der Fall ist, setzt
	// Compare-Flag zurück und setzt LED 7 zurück.
	else if(READ_BIT(TIM3->SR, TIM_SR_CC1IF)) {
		CLEAR_BIT(TIM3->SR, TIM_SR_CC1IF);
		*m_pwmOut = 0;
	}
}


void CPwmGenerator4::setPwmOut(DigitalOut* pin){
	m_pwmOut = pin;
}



