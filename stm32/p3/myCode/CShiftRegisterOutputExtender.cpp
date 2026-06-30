/*
 * CShiftRegisterOutputExtender.cpp
 *
 *  Created on: 17.10.2020
 *      Author: mnl
 */

#include "CShiftRegisterOutputExtender.h"
#include "stdint.h"

CShiftRegisterOutputExtender::CShiftRegisterOutputExtender(
		DigitalOut *serialData, DigitalOut *shiftRegisterClock,
		DigitalOut *bufferRegisterClock) {

		m_serialData = serialData;
		m_shiftRegisterClock = shiftRegisterClock;
		m_bufferRegisterClock = bufferRegisterClock;
}

void CShiftRegisterOutputExtender::prepareOutput(uint8_t value) {

	for (int i = 7; i >= 0; i--) {
	    *m_shiftRegisterClock = 0;
	    *m_serialData = value & (1 << i);
	    *m_shiftRegisterClock = 1;
	}

	m_preparedOutput = value;

}

void CShiftRegisterOutputExtender::updateOutput() {
	*m_bufferRegisterClock = 1;
	*m_bufferRegisterClock = 0;
	 m_currentOutput = m_preparedOutput;
}

void CShiftRegisterOutputExtender::write(uint8_t value) {
	prepareOutput(value);
	updateOutput();
}

CShiftRegisterOutputExtender& CShiftRegisterOutputExtender::operator =(
		uint8_t value) {
	write(value);
	return *this;
}

uint8_t CShiftRegisterOutputExtender::read() const {

	return m_currentOutput;
}

CShiftRegisterOutputExtender::operator int() const {

	return m_currentOutput;
}
