#pragma once

class MeasurePerformance
{
public:
	MeasurePerformance(void);
	~MeasurePerformance(void);
	//reset and start counter
	void start();
	//stop counter
	void end();
	//restart ounter
	void resume();
	//get the result in mili-second
	double	duration();
private:
	LARGE_INTEGER m_freq;
	LARGE_INTEGER m_start;
	long long m_duration;
};
inline void MeasurePerformance::start()
{
	m_duration = 0;
	QueryPerformanceCounter(&m_start);
}
//stop counter
inline void MeasurePerformance::end()
{
	LARGE_INTEGER tmEnd;
	QueryPerformanceCounter(&tmEnd);
	m_duration	+= tmEnd .QuadPart- m_start.QuadPart;
}
//restart ounter
inline void MeasurePerformance::resume()
{
	QueryPerformanceCounter(&m_start);
}