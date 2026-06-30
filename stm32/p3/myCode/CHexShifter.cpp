#include "CHexShifter.h"

CHexShifter::CHexShifter(CSevenSegmentDisplay* display,
		BusIn* keys, BusOut* leds, BufferedSerial* usart)
    : m_display(display), m_keys(new CDebouncer(keys)),
	  m_leds(leds), m_usart(usart), m_insertLeft(false) {

	m_display->setDecimalPoint(-1);
}

CHexShifter::~CHexShifter() {
    delete m_keys;
}

void CHexShifter::poll() {
    pollUsart();
    pollKeys();
}

void CHexShifter::pollUsart() {
    while (m_usart->readable()) {
        char c;
        m_usart->read(&c, 1);
        processChar(c);
    }
}
void CHexShifter::processChar(char c) {
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
    		|| (c >= 'A' && c <= 'F')) {

        int value;
        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else {
            value = (c & 0x0F) + 9;  // wechselt 'a'-'f' or 'A'-'F' to 10-15
        }
        processDigit(static_cast<uint8_t>(value));
    }
}




void CHexShifter::processDigit(uint8_t value) {
    if (m_insertLeft) {
        //Verschiebt die Digits nach rechts und erzeugt neue Digit links
        for (int digit = 2; digit >= 0; --digit) {
            m_display->operator[](digit + 1) = m_display->operator[](digit);
        }
        m_display->operator[](0) = value;
    } else {
    	//Verschiebt die Digits nach links und erzeugt neue Digit rechts
        for (int digit = 0; digit < 3; ++digit) {
            m_display->operator[](digit) = m_display->operator[](digit + 1);
        }
        m_display->operator[](3) = value;
    }

    sendDisplayedDigits();
}

void CHexShifter::sendDisplayedDigits() const {
    char buffer[2];
    for (int digit = 0; digit < 4; ++digit) {
        buffer[0] = static_cast<char>(m_display->operator[](digit) + '0');
        m_usart->write(buffer, 1);
    }
    m_usart->write("\r\n", 2);
}


void CHexShifter::pollKeys() {
	uint8_t previousKeysState = 0x00 ;
	uint8_t newKeysState = 0x00 ;
	uint8_t keysPushed = 0x00;
	previousKeysState = m_leds->read();
	newKeysState = m_keys->read();
	keysPushed = previousKeysState ^ newKeysState;
	keysPushed = keysPushed & 0x01;

	if(keysPushed == 0x01)
	{
		m_insertLeft = false;
		m_leds->write(0x01);
	}

	else
	{
		m_insertLeft = true;
		m_leds->write(0x00);
	}

}
