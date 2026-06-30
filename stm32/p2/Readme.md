
# STM32 Embedded C++ — 7-Segment-Anzeige & Sensor-Filterung

Eine Sammlung von Embedded-C++-Programmen für STM32 (Mbed OS), die eine 4-stellige 7-Segment-Anzeige über ein Schieberegister ansteuern und Sensordaten (Potentiometer) mit verschiedenen Filtermethoden verarbeiten.

## Hardware

- STM32-Board mit Mbed OS
- Breadboard mit 8 LEDs, 8 Tastern, einem Potentiometer und einer 4-stelligen 7-Segment-Anzeige (angesteuert über ein Schieberegister) — Pin-Belegung in `breadboard.h`

## Funktionsumfang

Das Projekt ist in mehrere eigenständige Aufgaben (`task1()`–`task7()` in `main.cpp`) gegliedert, die aufeinander aufbauen:

- **Direkte Tastenanzeige** — Tastenzustände werden bitweise auf der 7-Segment-Anzeige dargestellt
- **Hexadezimalanzeige** — Anzeige eines per Taster gewählten Werts (0–15) im Hexadezimalformat
- **Zeitmultiplex-Ansteuerung** — alle 4 Anzeigestellen werden per Timer im Wechsel aktualisiert, um Flackerfreiheit zu erreichen
- **Dynamische Zahlenanzeige** — eine Dezimalzahl wird mit Dezimalpunkt dargestellt und periodisch verändert
- **Potentiometer-Messung mit Watchdog** — die gemessene Spannung wird angezeigt, eine LED leuchtet bei Über-/Unterschreiten eines definierten Spannungsfensters (Analog Watchdog)
- **Rauschunterdrückung** — Glättung der Potentiometerwerte über einen gleitenden Mittelwert (`CMovingAverageFilter`) bzw. einen exponentiellen Filter (`CExponentialFilter`)

## Zentrale Komponenten

- `CShiftRegisterOutputExtender` — steuert ein gepuffertes 8-Bit-Schieberegister zur Erweiterung der Ausgänge
- `CSevenSegmentDisplay` — Ansteuerung einer 4-stelligen 7-Segment-Anzeige im Zeitmultiplexverfahren, inklusive Dezimalpunkt und Float-Darstellung
- `CPolledTimer` — ein einfacher, wiederverwendbarer Polling-Timer (ohne Interrupts, ohne blockierende Delays)
- `CMovingAverageFilter` — gleitender Mittelwertfilter für verrauschte Sensordaten
- `CExponentialFilter` — exponentieller Glättungsfilter als Alternative zum Mittelwertfilter

## Build

Entwickelt mit STM32CubeIDE / Mbed OS. Die gewünschte Aufgabe wird durch Auskommentieren des entsprechenden Aufrufs in `main()` ausgewählt.

## Motivation

Eine praktische Auseinandersetzung mit Bare-Metal-Embedded-Entwicklung — Ansteuerung von Mehrsegmentanzeigen per Zeitmultiplex, Auswertung analoger Sensorwerte sowie Vergleich verschiedener digitaler Filteransätze zur Rauschunterdrückung.
