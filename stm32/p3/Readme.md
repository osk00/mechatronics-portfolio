# STM32 Embedded C++ — Digitalvoltmeter & Serielle Hex-Anzeige

Zwei Anwendungen für STM32 (Mbed OS), die eine 4-stellige 7-Segment-Anzeige mit serieller Kommunikation (UART) und Taster-/LED-Steuerung kombinieren: ein digitales Voltmeter und eine Hexadezimal-Schiebeanzeige.

## Hardware

- STM32-Board mit Mbed OS
- Breadboard mit 8 LEDs, 8 Tastern, einem Potentiometer und einer 4-stelligen 7-Segment-Anzeige (angesteuert über ein Schieberegister) — Pin-Belegung in `breadboard.h`
- Serielle Schnittstelle (UART) zur Kommunikation mit einem Terminal (z. B. PuTTY)

## Funktionsumfang

Das Projekt ist in mehrere eigenständige Aufgaben (`task1()`–`task3()` in `main.cpp`) gegliedert:

- **UART-Test** — sendet fortlaufend ein Zeichen über die serielle Schnittstelle
- **Hex-Schiebeanzeige (`CHexShifter`)** — empfängt Hex-Ziffern (0–9, a–f) über die serielle Schnittstelle und schiebt sie auf der 7-Segment-Anzeige ein; per Taster lässt sich die Einfügerichtung (links/rechts) umschalten, der aktuelle Zustand wird per LED angezeigt und die Anzeige als Zeile an das Terminal zurückgesendet
- **Digitalvoltmeter (`CMeasuringDevice`)** — misst die Spannung am Potentiometer und stellt sie wahlweise als Dezimalwert auf der 7-Segment-Anzeige sowie als Binärwert, BCD-Wert, LED-Balkenanzeige oder formatierte PC-Ausgabe dar; unterstützt Einzelmessung, fortlaufende Messung mit Pause/Fortsetzen sowie Steuerung sowohl über Taster als auch über Tastaturbefehle im Terminal

## Zentrale Komponenten

- `CShiftRegisterOutputExtender` — steuert ein gepuffertes 8-Bit-Schieberegister zur Erweiterung der Ausgänge
- `CSevenSegmentDisplay` — Ansteuerung einer 4-stelligen 7-Segment-Anzeige im Zeitmultiplexverfahren, inklusive Dezimalpunkt und Float-Darstellung
- `CPolledTimer` — ein einfacher, wiederverwendbarer Polling-Timer (ohne Interrupts, ohne blockierende Delays)
- `CDebouncer` — entprellt Taster-Eingaben
- `CHexShifter` — verarbeitet serielle Hex-Eingaben und steuert die Schiebeanzeige
- `CMeasuringDevice` — kapselt die komplette Logik des Digitalvoltmeters inklusive Anzeige- und Messmodi

## Build

Entwickelt mit STM32CubeIDE / Mbed OS. Die gewünschte Aufgabe wird durch Auskommentieren des entsprechenden Aufrufs in `main()` ausgewählt. Für die Terminal-Interaktion wird ein serielles Terminalprogramm wie PuTTY (115200 Baud) benötigt.

## Motivation

Eine praktische Auseinandersetzung mit objektorientierter Embedded-Entwicklung — Kapselung komplexer Anwendungslogik in eigenständige Klassen, Verarbeitung serieller Kommunikation neben Taster- und Sensorabfragen, sowie Aufbau eines kleinen interaktiven Mess- und Anzeigesystems.

