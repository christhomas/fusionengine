#ifndef _TIMER_H_
	#define _TIMER_H_

#include <windows.h>

// simple win32 timer and stopclock

class Timer{
private:
	bool				m_Active;
	LARGE_INTEGER	m_frequency_clockticks,
					m_frequency_milliseconds,
					m_current,
					m_previous,
					m_ticks;

	__int64			m_totalticks;

	float				m_milliseconds;
	float				m_totalmilliseconds;

	bool				m_stopwatch;
	int				m_stoptime;
	
public:
	inline Timer(){
		Reset();
		SetFrequency();
	}
	inline ~Timer(){
		Stop();
	}
	
	inline void	SetFrequency(void){
		QueryPerformanceFrequency(&m_frequency_clockticks);
		m_frequency_milliseconds.QuadPart = m_frequency_clockticks.QuadPart/1000;

		QueryPerformanceCounter(&m_current);
		m_previous.QuadPart = m_current.QuadPart;
	}

	inline void	GetFrequencyClockTicks(unsigned int &high,unsigned int &low){
		high	= m_frequency_clockticks.HighPart;
		low	= m_frequency_clockticks.LowPart;
	}

	inline void	GetFrequencyMilliseconds(unsigned int &high,unsigned int &low){
		high	= m_frequency_milliseconds.HighPart;
		low	= m_frequency_milliseconds.LowPart;
	}

	inline void Set(int milliseconds){
		m_stoptime	= (milliseconds)<<8;
		m_stopwatch	= true;
	}

	inline void	Start(void){
		m_Active = true;
	}

	inline void	Stop(void){
		m_Active = false;
	}

	inline void	Reset(void){
		m_stopwatch		= false;
		m_stoptime		= -1;
		m_milliseconds	= 0;
		m_totalmilliseconds	= 0;
		memset(&m_totalticks,0,sizeof(m_totalticks));
	}

	inline void Tick(void){
		if(m_Active==true){
			QueryPerformanceCounter(&m_current);
			m_ticks.QuadPart		=	m_current.QuadPart - m_previous.QuadPart;
			m_previous.QuadPart	=	m_current.QuadPart;

			m_totalticks			+=	m_ticks.QuadPart;
			m_milliseconds		=	(float)(m_ticks.QuadPart / m_frequency_milliseconds.QuadPart);
			m_totalmilliseconds		+=	m_milliseconds;

			if((m_stopwatch == true) && (m_totalmilliseconds >= m_stoptime)){
				m_totalmilliseconds = 0;
				Stop();
			}
		}
	}

/*
 *	Absolute timers, the total time since the timer started
 */
	inline unsigned int GetMillisecondsAbs(void){
		return (unsigned int)m_totalmilliseconds;
	}

	inline float GetMillisecondsAbs_fp(void){
		return m_totalmilliseconds;
	}

/*
 *	Difference timers, since the last update, how much time has passed
 */
	inline unsigned int GetMillisecondsDiff(void){
		return (unsigned int)m_milliseconds;
	}

	inline float GetMillisecondsDiff_fp(void){
		return m_milliseconds;
	}
};

#endif // ifndef _TIMER_H_
