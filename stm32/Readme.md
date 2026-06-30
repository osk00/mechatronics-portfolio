
---

## 🎯 Projekte im Detail

### 1. LED/Taster-Steuerung
Grundlegende Konzepte der Embedded-Programmierung:
- **LED-Blinken** — alle 8 LEDs im festen Intervall ein- und ausschalten
- **Taster-LED-Zuordnung** — direkte Steuerung sowie Umschalten per Tastendruck
- **Entprellte Eingabe** — `CDebouncer`-Klasse filtert Tastenprellen heraus
- **Nicht-blockierendes Timing** — `CPolledTimer`-Klasse für periodische Abläufe ohne Blockierung
- **Lauflicht** — variable Geschwindigkeit und Richtung, vollständig über Taster steuerbar

### 2. 7-Segment-Anzeige & Sensor-Filterung
Ansteuerung einer 4-stelligen 7-Segment-Anzeige mit Sensorauswertung:
- **Direkte Tastenanzeige** — Tastenzustände werden bitweise auf der 7-Segment-Anzeige dargestellt
- **Hexadezimalanzeige** — Anzeige eines per Taster gewählten Werts (0–15)
- **Zeitmultiplex-Ansteuerung** — alle 4 Anzeigestellen werden per Timer flackerfrei aktualisiert
- **Dynamische Zahlenanzeige** — Dezimalzahl mit Dezimalpunkt, periodisch verändert
- **Potentiometer-Messung mit Watchdog** — Spannungsmessung mit LED-Warnung bei Über-/Unterschreitung
- **Rauschunterdrückung** — Glättung der Messwerte mit gleitendem Mittelwert (`CMovingAverageFilter`) und exponentiellem Filter (`CExponentialFilter`)

### 3. PWM-Signalgenerierung & Timer-Interrupts
Verschiedene Ansätze zur PWM-Erzeugung mit Timern:
- **CPwmGenerator1** — Basis-PWM mit fester Periodendauer und festem Duty Cycle
- **CPwmGenerator2** — PWM mit variablem Duty Cycle (0–100%)
- **CPwmGenerator3** — PWM mit konfigurierbarer Periodendauer und variablem Duty Cycle
- **CPwmGenerator4** — PWM mit Timer-Interrupts, zusätzliche Ausgabe über DigitalOut-Pin

### 4. Digitalvoltmeter & Serielle Hex-Anzeige
Kombination von 7-Segment-Anzeige mit serieller Kommunikation:
- **UART-Test** — sendet fortlaufend Zeichen über die serielle Schnittstelle
- **Hex-Schiebeanzeige (`CHexShifter`)** — empfängt Hex-Ziffern über UART und schiebt sie auf der Anzeige ein; Einfügerichtung per Taster umschaltbar
- **Digitalvoltmeter (`CMeasuringDevice`)** — misst Spannung am Potentiometer und stellt sie als Dezimalwert, Binärwert, BCD-Wert oder LED-Balken dar; unterstützt Einzelmessung und fortlaufende Messung mit Pause/Fortsetzen

---

## 🔑 Zentrale Komponenten (Bibliotheken)

Über alle Projekte hinweg werden folgende wiederverwendbare Klassen eingesetzt:

| Klasse | Beschreibung |
|--------|--------------|
| `CDebouncer` | Entprellt Taster-Eingaben (Hardware-Entprellung in Software) |
| `CPolledTimer` | Nicht-blockierender Polling-Timer für periodische Abläufe |
| `CShiftRegisterOutputExtender` | Steuert ein gepuffertes 8-Bit-Schieberegister zur Ausgangserweiterung |
| `CSevenSegmentDisplay` | Ansteuerung einer 4-stelligen 7-Segment-Anzeige im Zeitmultiplexverfahren |
| `CExponentialFilter` | Exponenzieller Glättungsfilter für verrauschte Sensordaten |
| `CPwmGenerator1-4` | Verschiedene PWM-Generatoren mit Timer 3 auf PC6 |

---
