# STM32 Embedded C++ — PWM-Signalgenerierung & Timer-Interrupts

Eine Sammlung von Embedded-C++-Programmen für STM32 (Mbed OS), die verschiedene PWM-Signalgeneratoren mit Timer-Interrupts und periodischen Signalen implementieren.

## Hardware

- STM32-Board mit Mbed OS
- Breadboard mit 8 LEDs, 8 Tastern und einem Potentiometer — Pin-Belegung in `breadboard.h`
- PWM-Ausgang auf PC6 (Timer 3, Kanal 1)

## Funktionsumfang

Das Projekt ist in mehrere eigenständige Klassen gegliedert, die verschiedene Ansätze zur PWM-Erzeugung mit Timern demonstrieren:

- **CPwmGenerator1** — Basis-PWM-Generator mit fester Periodendauer von 16 Sekunden und festem Duty Cycle (10:6 Verhältnis). Timer 3 wird ohne Interrupts betrieben.
- **CPwmGenerator2** — Erweiterung von Generator 1 mit der Möglichkeit, den Duty Cycle über `setDutyCycle(uint8_t percent)` anzupassen. Das PWM-Signal kann zwischen 0% und 100% variiert werden.
- **CPwmGenerator3** — PWM-Generator mit konfigurierbarer Periodendauer (in Mikrosekunden) über den Konstruktor. Der Duty Cycle kann ebenfalls dynamisch angepasst werden.
- **CPwmGenerator4** — Fortgeschrittener PWM-Generator mit Timer-Interrupts. Bei jedem Timer-Update wird eine LED gesetzt, beim Compare-Event wird sie zurückgesetzt. Dadurch wird das PWM-Signal zusätzlich auf einem DigitalOut-Pin ausgegeben.

## Zentrale Komponenten

- `CPwmGenerator1` — PWM-Erzeugung mit Timer 3, fester Frequenz und festem Duty Cycle
- `CPwmGenerator2` — PWM-Erzeugung mit variablem Duty Cycle (0–100%)
- `CPwmGenerator3` — PWM-Erzeugung mit konfigurierbarer Periodendauer und variablem Duty Cycle
- `CPwmGenerator4` — PWM-Erzeugung mit Interrupts, zusätzliche Ausgabe über DigitalOut-Pin

## Build

Entwickelt mit STM32CubeIDE / Mbed OS. Die gewünschte Klasse wird durch Instanziierung des entsprechenden Objekts in `main.cpp` ausgewählt. Die PWM-Ausgabe erfolgt auf PC6.

## Technische Details

- **Timer:** TIM3
- **Prescaler:** 84-1 (ergibt eine Timer-Frequenz von 1 MHz bei 84 MHz APB1-Takt)
- **PWM-Pin:** PC6 (Alternate Function 2)
- **Periode:** Konfigurierbar (bei Generator 1 und 2 fest auf 16 Sekunden, bei Generator 3 und 4 variabel)
- **Interrupts:** Update-Interrupt und Compare-Interrupt (nur bei Generator 4)

## Motivation

Eine praktische Auseinandersetzung mit der Timer-Peripherie von STM32-Mikrocontrollern. Die verschiedenen Implementierungen zeigen, wie PWM-Signale auf unterschiedliche Weise erzeugt werden können — von einfachen festen Konfigurationen bis hin zu dynamisch anpassbaren Signalen mit Interrupt-basierter Steuerung.
