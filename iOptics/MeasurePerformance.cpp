#include "stdafx.h"

#include "MeasurePerformance.h"

MeasurePerformance::MeasurePerformance(void)
{::QueryPerformanceFrequency(&m_freq);}

MeasurePerformance::~MeasurePerformance(void){}
	//reset and start counter

//get the result in mili-second
double	MeasurePerformance::duration()
{
	return ( m_duration * 1000.0) / m_freq.QuadPart;
}
