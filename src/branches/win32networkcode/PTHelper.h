#ifndef _PTHELPER_H_
#define _PTHELPER_H_

#include <pthread.h>

//	Only need this function on win32
#ifdef _WIN32
#include <winsock2.h>
int _gettimeofday(struct timeval *tp, void *not_used);
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
	bool			trigger;
	
	inline Event(bool t)
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
		trigger = true;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}

	inline int wait()
	{
		int result = 0;
		
		pthread_mutex_lock(&mutex);
		if(trigger == false) result = pthread_cond_wait(&cond,&mutex);
		trigger = false;
		pthread_mutex_unlock(&mutex);
		
		return result;
	}

	inline int timedwait(int milliseconds)
	{
		if(milliseconds == INFINITE) return wait();
		
		int result = 0;
		
		pthread_mutex_lock(&mutex);
		if(trigger == false){
			//	Create a timespec object with the time+waittime
			_gettimeofday(&now, NULL);
			time.tv_sec = now.tv_sec + (milliseconds/1000);
			time.tv_nsec = (now.tv_usec*1000) + ((milliseconds % 1000) * 1000);

			result = pthread_cond_timedwait(&cond,&mutex,&time);
		}
		trigger = false;		
		pthread_mutex_unlock(&mutex);
		
		return result;
	}
};

#endif // #ifndef _PTHELPER_H_