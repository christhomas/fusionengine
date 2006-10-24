#ifndef _TIMER_H_
	#define _TIMER_H_

#include <sys/time.h>
#include <cstdlib>

class Timer{
private:
	bool				m_Active;
	struct timeval		m_current,m_previous,m_diff;

	float				m_milliseconds;
	float				m_totalmilliseconds;

	bool				m_stopwatch;
	int				m_stoptime;

public:
	inline Timer(){
		Reset();
	}
	inline ~Timer(){
		Stop();
	}
	
	inline void Set(int milliseconds){
		m_stoptime	= milliseconds;
		m_stopwatch	= true;
	}

	inline void	Start(void){
		m_Active = true;
		
		gettimeofday(&m_previous,NULL);
	}

	inline void	Stop(void){
		m_Active = false;
	}

	inline void	Reset(void){
		m_stopwatch		= false;
		m_stoptime		= -1;
		m_milliseconds	= 0;
		m_totalmilliseconds	= 0;
	}

	inline void Tick(void){
		if(m_Active==true){
			gettimeofday(&m_current,NULL);
			
			//	calculate the difference between two previous and current times
			m_diff.tv_sec		=	m_current.tv_sec - m_previous.tv_sec;
			m_diff.tv_usec 		=	m_current.tv_usec - m_previous.tv_usec;
			
			//	now update the previous time with the current time
			m_previous.tv_sec	=	m_current.tv_sec;
			m_previous.tv_usec	=	m_current.tv_usec;
			
			//	calculate the number of milliseconds diff and total
			m_milliseconds	=	(m_diff.tv_sec * 1000) + m_diff.tv_usec;
			m_totalmilliseconds	+=	m_milliseconds;
			
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
