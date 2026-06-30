# STM32 Embedded C++ — LED/Taster-Steuerung

Eine Sammlung kleiner Embedded-C++-Programme für STM32 (Mbed OS), die grundlegende Bare-Metal-Konzepte demonstrieren: bitweise I/O-Steuerung, Entprellen von Tastern (Debouncing) und nicht-blockierende Polling-Timer.

## Hardware

- STM32-Board mit Mbed OS
- Breadboard mit 8 LEDs, 8 Tastern und einem Potentiometer (Pin-Belegung in `breadboard.h`)

## Funktionsumfang

Das Projekt ist in mehrere eigenständige Aufgaben (`task1()`–`task9()` in `main.cpp`) gegliedert, die aufeinander aufbauen:

- **LED-Blinken** — alle 8 LEDs im festen Intervall ein- und ausschalten
- **Taster-LED-Zuordnung** — direkte Steuerung sowie Umschalten per Tastendruck
- **Entprellte Eingabe** — eine wiederverwendbare `CDebouncer`-Klasse, die Prellen der Taster herausfiltert
- **Nicht-blockierendes Timing** — eine `CPolledTimer`-Klasse für periodische Abläufe, ohne die Hauptschleife zu blockieren
- **Lauflicht (Knight-Rider-Effekt)** — variable Geschwindigkeit und Richtung, vollständig über Taster steuerbar

## Zentrale Komponenten

- `CDebouncer` — kapselt ein `BusIn`-Objekt und liefert stabile, entprellte Werte
- `CPolledTimer` — ein einfacher, wiederverwendbarer Polling-Timer (ohne Interrupts, ohne blockierende Delays)

## Build

Entwickelt mit STM32CubeIDE / Mbed OS. Die gewünschte Aufgabe wird durch Auskommentieren des entsprechenden Aufrufs in `main()` ausgewählt.

## Motivation

Eine praktische Auseinandersetzung mit Bare-Metal-Embedded-Entwicklung — Debouncing, kooperatives Multitasking per Polling und eine saubere Trennung der Hardware-Logik in wiederverwendbare Klassen.

