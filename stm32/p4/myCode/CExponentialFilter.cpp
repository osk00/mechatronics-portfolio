/*
 * CExponentialFilter.cpp
 *
 *  Created on: Dec 1, 2023
 *      Author: osk
 */

#include "CExponentialFilter.h"


CExponentialFilter::CExponentialFilter(float factor){
	m_factor = factor;
	m_last = 0;
}


CExponentialFilter& CExponentialFilter::operator<<(float value){
	value = m_last + (m_factor*(value - m_last));
	m_last = value;
	return *this;
}

CExponentialFilter::operator float() const{
	return m_last;
}
