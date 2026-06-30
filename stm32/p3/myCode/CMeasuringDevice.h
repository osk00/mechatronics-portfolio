/*
 * Autor: Obada Alkhatib
 * Mtr : 1111992
 * Datum: 17.01.2024
 */

// CMeasuringDevice.h

#ifndef CMEASURINGDEVICE_H_
#define CMEASURINGDEVICE_H_

#include "mbed.h"
#include "CSevenSegmentDisplay.h"
#include "CDebouncer.h"
#include "CPolledTimer.h"
#include <string>

/**
 * Repräsentiert die realisierte Anwendung "Messgerät".
 */
class CMeasuringDevice {

private:
    // Enum mit allen möglichen Anzeigemoden
    enum anzeigeModus {
        BINARY,
        BCD,
        BALKEN,
        PC
    };

    // Enum mit allen möglichen Messmoden
    enum messModus {
        NONE,
        EINZEL,
        FORTLAUFEND,
        HALT // Pause und wenn nochmal gewählt wird, wieder fortgesetzt.
    };

    // Zeiger auf Hardware-Komponenten
    CSevenSegmentDisplay* m_display;
    CDebouncer* m_keys;
    BusOut* m_leds;
    BufferedSerial* m_usart;
    AnalogIn* m_voltageInput;

    // Timer für fortlaufenden Messungen
    CPolledTimer* m_timer;

    // Variable für vorherigen Tastenzustand
    uint8_t m_previousKeyState;

    // Variablen für die letzte Messung
    float m_lastMess;
    uint8_t m_lastU8;

    // Variablen für Anzeige- und Messmodus
    anzeigeModus m_anzeigeModus;
    messModus m_messModus;

    // Konstante für Nummern als Char
    static const char nums[];

    // Private Methoden zur Verarbeitung von Eingabequellen

    /**
     * Überprüft, ob die Tasten gedrückt wurden und
     * stellt den Anzeige- und Messmodus entsprechend ein.
     */
    void pollTasten();

    /**
     * Überprüft, ob ein neues Zeichen über die serielle
     * Schnittstelle eingegangen ist. Falls dies zutrifft,
     * wird das Zeichen analysiert und entsprechend der
     * Anzeige- und Messmodus-Einstellungen konfiguriert.
     */
    void pollKeyboard();

    /**
     * Konfiguriert den Messmodus gemäß dem übergebenen Parameter
     * und führt die entsprechende Messung durch.
	 *
	 * @param mode Der neue Wert, um den Messmodus zu setzen.
     */
    void setMessModus(uint8_t);

    /**
     * Konfiguriert den Anzeigemodus gemäß dem übergebenen Parameter
     * und aktualisiert die Anzeige basierend auf dem ausgewählten Modus.
	 *
	 * @param mode Der neue Wert, um den Anzeigemodus zu setzen.
     */
    void setAnzeigeModus(uint8_t);

    /**
     * Führt eine einzige Messung aus.
     */
    void pollEinzel();

    /**
     * Führt die fortlaufende Messung durch. In dieser Methode
     * wird der m_timer verwendet, um die Messung regelmäßig
     * alle 100ms durchzuführen.
     */
    void pollFortlaufend();

    /**
     * Präsentiert den gemessenen Wert auf der PUTTY-Konsole
     * über die serielle Schnittstelle.
     */
    void zeigPC();

    /**
     * Präsentiert den gemessenen Wert in binärer Form über die LEDs.
     */
    void zeigBinary();

    /**
     * Präsentiert den gemessenen Wert in der BCD-Form auf den LEDs.
     * Hierbei werden die vier linken LEDs zur Darstellung der
     * Vorkommastelle und die vier rechten LEDs zur Darstellung
     * der ersten Nachkommastelle verwendet.
     */
    void zeigBCD();

    /**
     * Präsentiert den gemessenen Wert als Balken.
     * Je größer der Wert, desto mehr LEDs leuchten.
     */
    void zeigBalken();

    /**
     * Misst den Spannungswert und liefert ihn als
     * Gleitkommazahl zurück.
     */
    float lesPoti();

    /**
     * Setzt dieses CMeasuringDevice zurück in
     * denselben Zustand wie beim Programmstart.
     */
    void clearMessung();

    /**
     * Löscht den Inhalt der PUTTY-Konsole und
     * positioniert den Cursor auf die erste Stelle.
     */
    void clearFenster();

    /**
     * Schreibt den Inhalt des Parameters "message"
     * auf die PUTTY-Konsole.
	 *
	 * @param message Der Text, der auf der PUTTY-Konsole
	 * geschrieben werden soll.
     */
    void zeigOutputMessage(string message, bool clear);

public:
    /**
     * Erstellt eine neue Instanz der Anwendung, die
     * die angegebenen Hardware-Komponenten verwendet.
	 *
	 * @param display Die 7-Segment-Anzeige.
	 * @param keys Die Eingänge, an die die
	 * Taster angeschlossen sind.
	 * @param leds Die Ausgänge, an die die LEDs
	 * angeschlossen sind.
	 * @param usart Die serielle Schnittstelle.
	 * @param voltageInput Der Analogeingang.
     */
    CMeasuringDevice(CSevenSegmentDisplay* display, BusIn* keys,
       BusOut* leds, BufferedSerial* usart, AnalogIn* voltageInput);

    /**
     * Befreit die allokierten Ressourcen.
     */
    ~CMeasuringDevice();

    /**
     * Ruft die privaten Methoden der Form "poll..." auf,
     * um die verschiedenen Eingabequellen zu verarbeiten.
     */
    void poll();
};

#endif /* CMEASURINGDEVICE_H_ */
