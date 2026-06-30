/*
 * Autor: Obada Alkhatib
 * Mtr : 1111992
 * Datum: 17.01.2024
 */

//CMeasuringDevice.cpp

#include "CMeasuringDevice.h"


const char CMeasuringDevice::nums[] = {'0','1','2','3','4','5','6','7','8','9'};

CMeasuringDevice::CMeasuringDevice(CSevenSegmentDisplay* display, BusIn* keys,
		BusOut* leds, BufferedSerial* usart, AnalogIn* voltageInput)
{

	m_keys = new CDebouncer(keys, 100);
	m_leds = leds;
	m_usart = usart;
	m_display = display;
	m_voltageInput = voltageInput;

	clearMessung();
}

CMeasuringDevice::~CMeasuringDevice() {
	delete m_keys;
	delete m_timer;
}

void CMeasuringDevice::poll() {
    // Tasten lesen
    pollTasten();
    // Lese die serielle Schnittstelle ab, wenn ein Eingabe vorhanden ist
    if (m_usart->readable()) {
        pollKeyboard();
    // Wenn der fortlaufende Messmodus ausgewählt ist, messe die Spannung und zeige sie an
    }else if(m_messModus == FORTLAUFEND){
    	pollFortlaufend();
    }
}

void CMeasuringDevice::pollTasten() {
    uint8_t newKeysState = m_keys->read();
    uint8_t keysPushed = ~m_previousKeyState & newKeysState;
    m_previousKeyState = newKeysState;

    setMessModus(keysPushed & 0x0F);
    setAnzeigeModus(keysPushed & 0xF0);
}



void CMeasuringDevice::pollKeyboard() {
    char keyboard[1];
    m_usart->read(keyboard, 1);

    switch (keyboard[0]) {
        case 'b': setAnzeigeModus(0x80); break;
        case 'c': setAnzeigeModus(0x40); break;
        case 'i': setAnzeigeModus(0x20); break;
        case 'p': setAnzeigeModus(0x10); break;
        case 'e': setMessModus(0x08); break;
        case 'f': setMessModus(0x04); break;
        case 'h': setMessModus(0x02); break;
        case 's': setMessModus(0x01); break;
        case 'v': clearMessung(); break;
        default: {
            zeigOutputMessage("Unbekannter Befehl", true);
            break;
        }
    }
}

void CMeasuringDevice::setMessModus(uint8_t modus) {
    switch (modus) {
        case 0x08: // Einzel-Messung Modus Taste
            zeigOutputMessage("Einzelmessung gestartet", true);
            m_messModus = EINZEL;
            // Wir lesen nur einmal im Einzelmodus,
            //daher wird dieser Timer nicht benötigt.
            delete m_timer;
            pollEinzel();
            break;
        case 0x04: // Fortlaufender Modus Taste
            zeigOutputMessage("Dauermessung gestartet", true);
            m_messModus = FORTLAUFEND;
            // Timer für fortlaufende Messung initialisieren
            m_timer = new CPolledTimer(100);
            break;
        case 0x02: // HALT Taste
            if (m_messModus == FORTLAUFEND) {
                zeigOutputMessage("Dauermessung pausiert" , true);
                m_messModus = HALT;
                m_display->setValue(m_lastMess);
            } else if (m_messModus == HALT) {
                zeigOutputMessage("Dauermessung fortgesetzt", true);
                m_messModus = FORTLAUFEND;
            }
            break;
        case 0x01: // Stop Taste
            zeigOutputMessage("Dauermessung gestoppt", true);
            m_messModus = NONE;
            m_lastMess = 0;
            m_lastU8 = 0;
            m_display->setValue(0);
            m_leds->write(0);
            break;
    }
}
void CMeasuringDevice::setAnzeigeModus(uint8_t modus) {
    switch (modus) {
        case 0x10: // PC
        	m_anzeigeModus = PC;
            m_leds->write(0x00);
            zeigPC();

            break;
        case 0x20: // BINARY
            zeigOutputMessage("Displaymodus: Binär", true);
            m_anzeigeModus = BINARY;
            zeigBinary();
            break;
        case 0x40: // BCD
            zeigOutputMessage("Displaymodus: BCD", true);
            m_anzeigeModus = BCD;
            zeigBCD();
            break;
        case 0x80: // BALKEN
            zeigOutputMessage("Displaymodus: Balken", true);
            m_anzeigeModus = BALKEN;
            zeigBalken();
            break;
    }
}
void CMeasuringDevice::pollEinzel() {
    // Potentiometerwert ablesen
    m_lastMess = lesPoti();
    m_lastU8 = static_cast<uint8_t>((m_lastMess / 3.3) * 255);

    // 7-Segment-Anzeige aktualisieren
    m_display->setValue(m_lastMess);

    // Zeigt den Wert basierend auf dem ausgewählten Anzeigemodus an
    switch (m_anzeigeModus) {
        case PC:
            m_leds->write(0x00);
            zeigPC();
            break;
        case BINARY:
            zeigBinary();
            break;
        case BCD:
            zeigBCD();
            break;
        case BALKEN:
            zeigBalken();
            break;
    }
}
void CMeasuringDevice::pollFortlaufend() {
    if (!m_timer->timeReached()) {
        return;
    }

    // Potentiometerwert ablesen
    m_lastMess = lesPoti();
    m_lastU8 = static_cast<uint8_t>((m_lastMess / 3.3) * 255);

    // 7-Segment-Anzeige aktualisieren
    m_display->setValue(m_lastMess);

    // Zeigt den Wert basierend auf dem ausgewählten Anzeigemodus an
    switch (m_anzeigeModus) {
        case PC:
            m_leds->write(0x00);
            zeigPC();
            break;
        case BINARY:
            zeigBinary();
            break;
        case BCD:
            zeigBCD();
            break;
        case BALKEN:
            zeigBalken();
            break;
    }
}



void CMeasuringDevice::zeigPC() {
	clearFenster();

    char buffer[7];

    buffer[0] = nums[(*m_display)[0]];
    buffer[1] = '.';
    for(int i = 1;i<4;++i){
        buffer[i+1] = nums[(*m_display)[i]];
    	}
    buffer[5] = ' ';
    buffer[6] = 'V';

    m_usart->write(buffer, sizeof(buffer));
	zeigOutputMessage(", Displaymodus: PC", false);
    }

void CMeasuringDevice::zeigBinary() {
	m_leds->write(m_lastU8);
}

void CMeasuringDevice::zeigBCD() {

	m_leds->write((((*m_display)[0] << 4) | (*m_display)[1]));
}

void CMeasuringDevice::zeigBalken() {
	// Berechne die Anzahl der beleuchteten LEDs
	// basierend auf dem Potentiometer-Wert
	int anzahlBeleuchteteLeds = m_lastU8 * 8 / 255;
	// Erzeuge ein Bitmuster, um die beleuchteten LEDs
	// von rechts nach links zu repräsentieren
	int ledMuster = (1 << anzahlBeleuchteteLeds) - 1;
	// Setze die LEDs basierend auf dem Bitmuster
	m_leds->write(ledMuster << (8 - anzahlBeleuchteteLeds));
}

float CMeasuringDevice::lesPoti() {
	return m_voltageInput->read_voltage();
}

void CMeasuringDevice::clearMessung() {
	m_previousKeyState = 0x00;
	// Wir sollen ohne Messung anfangen
	m_messModus = NONE;
	// Wir sollen im Balken Modus anfangen
	m_anzeigeModus = BALKEN;
	m_lastMess = 0;
	m_lastU8 = 0;
	*m_leds = 0;
	*m_display = 0;

	zeigOutputMessage("MIT-Digitalvoltmeter : SW-Rev. 0.1", true);
}

void CMeasuringDevice::clearFenster() {
    const char returnS = '\r';
    const char leer = ' ';

    // Bewegt den Cursor wieder an den Anfang
    m_usart->write(&returnS, 1);

    // Schreibt 50 Leerzeichen, um die Konsole zu leeren
    for (int i = 0; i < 50; i++) {
        m_usart->write(&leer, 1);
    }

    // Bewegt den Cursor wieder an den Anfang
    m_usart->write(&returnS, 1);
}

void CMeasuringDevice::zeigOutputMessage(string message, bool  clear) {
	if(clear == true){
		clearFenster();
	}
	m_usart->write(&message[0], message.size());
}

