/*
 * CMovingAverageFilter.cpp
 *
 *  Created on: Dec 1, 2023
 *      Author: osk
 */

#include "CMovingAverageFilter.h"

CMovingAverageFilter::CMovingAverageFilter(uint16_t size) {
	m_values = new uint16_t[size];
	m_sum = 0;

	for(int i = 0 ; i<size ; i++){
		m_values[i] = 0;
	}

	m_valuesSize = size;
	m_nextIndex = 0;
}

CMovingAverageFilter& CMovingAverageFilter::operator<<(uint16_t value){
	m_sum = m_sum - m_values[m_nextIndex];
	m_values[m_nextIndex] = value;
	m_sum = m_sum + m_values[m_nextIndex];
	m_nextIndex = (m_nextIndex + 1 ) % m_valuesSize;
	return *this;
}

CMovingAverageFilter::operator uint16_t() const{
	return m_sum/m_valuesSize;
}
