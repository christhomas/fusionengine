#ifndef _FUSION_PTHREAD_H_
#define _FUSION_PTHREAD_H_

#include <pthread.h>

//	Only need this function on win32
#ifdef _WIN32
/********************************************************
*	int GetTimeOfDay(struct timeval *tp, void *not_used);
*
*	This function was posted to comp.lang.perl.moderated
*	Usenet newsgroup by Anton Shcherbinin <useperl@fastmail.fm>
*	on Mon, 29 Dec 2003 13:29:48 +0300,
*	Message-ID: <bsp067$ethbi$1@ID-211973.news.uni-berlin.de>
*
*	Licence: BSD-Like
*
********************************************************/
#include <windows.h>

typedef union {
    unsigned __int64	ft_i64;
    FILETIME			ft_val;
} FT_t;

typedef struct {
    unsigned long run_count;
    unsigned __int64 base_ticks;
    unsigned __int64 tick_frequency;
    FT_t base_systime_as_filetime;
    unsigned __int64 reset_time;
} my_cxt_t;

#ifdef __GNUC__
# define Const64(x) x##LL
#else
# define Const64(x) x##i64
#endif
#define EPOCH_BIAS  Const64(116444736000000000)
/* If the performance counter delta drifts more than 0.5 seconds from the
 * system time then we recalibrate to the system time.  This means we may
 * move *backwards* in time! */
#define MAX_PERF_COUNTER_SKEW Const64(5000000) /* 0.5 seconds */

/* Reset reading from the performance counter every five minutes.
 * Many PC clocks just seem to be so bad. */
#define MAX_PERF_COUNTER_TICKS Const64(300000000) /* 300 seconds */

static int
_gettimeofday(struct timeval *tp, void *not_used)
{
    my_cxt_t MY_CXT;
    memset(&MY_CXT,0,sizeof(my_cxt_t));

    unsigned __int64 ticks;
    FT_t ft;

    if (MY_CXT.run_count++ == 0 ||
	MY_CXT.base_systime_as_filetime.ft_i64 > MY_CXT.reset_time) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&MY_CXT.tick_frequency);
        QueryPerformanceCounter((LARGE_INTEGER*)&MY_CXT.base_ticks);
        GetSystemTimeAsFileTime(&MY_CXT.base_systime_as_filetime.ft_val);
        ft.ft_i64 = MY_CXT.base_systime_as_filetime.ft_i64;
	MY_CXT.reset_time = ft.ft_i64 + MAX_PERF_COUNTER_TICKS;
    }
    else {
	__int64 diff;
        QueryPerformanceCounter((LARGE_INTEGER*)&ticks);
        ticks -= MY_CXT.base_ticks;
        ft.ft_i64 = MY_CXT.base_systime_as_filetime.ft_i64
                    + Const64(10000000) * (ticks / MY_CXT.tick_frequency)
                    +(Const64(10000000) * (ticks % MY_CXT.tick_frequency)) / MY_CXT.tick_frequency;
	diff = ft.ft_i64 - MY_CXT.base_systime_as_filetime.ft_i64;
	if (diff < -MAX_PERF_COUNTER_SKEW || diff > MAX_PERF_COUNTER_SKEW) {
	    MY_CXT.base_ticks += ticks;
            GetSystemTimeAsFileTime(&MY_CXT.base_systime_as_filetime.ft_val);
            ft.ft_i64 = MY_CXT.base_systime_as_filetime.ft_i64;
	}
    }

    /* seconds since epoch */
    tp->tv_sec = (long)((ft.ft_i64 - EPOCH_BIAS) / Const64(10000000));

    /* microseconds remaining */
    tp->tv_usec = (long)((ft.ft_i64 / Const64(10)) % Const64(1000000));

    return 0;
}

#endif

/*
*	Wrapper functions for pthread events (wraps up the ugliness)
*/

class Event{
public:
	pthread_cond_t	cond;
	pthread_mutex_t	mutex;
	timeval			now;
	timespec		time;
	unsigned int	trigger;
	
	inline Event(unsigned int t=0)
	{
		trigger = t;
		
		pthread_cond_init(&cond,NULL);
		pthread_mutex_init(&mutex,NULL);
	}
	
	inline ~Event()
	{
		pthread_cond_destroy(&cond);
		pthread_mutex_destroy(&mutex);
	}
	
	inline void signal()
	{
		pthread_mutex_lock(&mutex);
		trigger++;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}

	inline int wait()
	{
		int result = 0;
		
		pthread_mutex_lock(&mutex);
		if(trigger == 0){
			result = pthread_cond_wait(&cond,&mutex);
		}else{ trigger--; }
		pthread_mutex_unlock(&mutex);
		
		return result;
	}

	inline int timedwait(int milliseconds)
	{
		if(milliseconds == INFINITE) return wait();
		
		int result = 0;
		
		pthread_mutex_lock(&mutex);
		if(trigger == 0){
			//	Create a timespec object with the time+waittime
			_gettimeofday(&now, NULL);
			time.tv_sec = now.tv_sec + (milliseconds/1000);
			time.tv_nsec = (now.tv_usec*1000) + ((milliseconds % 1000) * 1000);

			result = pthread_cond_timedwait(&cond,&mutex,&time);
			if(result != ETIMEDOUT) trigger--;
		}else{ trigger--; }
	
		pthread_mutex_unlock(&mutex);
		
		return result;
	}
};

#endif // #ifndef _FUSION_PTHREAD_H_