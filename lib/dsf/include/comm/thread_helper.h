#pragma once

/*
 * file:	thread_help.h
 * brief:	pthread包装一下
 * author:  pls
 **/

#include "stdinc.h"
#include <pthread.h>
#include <semaphore.h>

NS_BEGIN(dsf)

class ThreadHelper
{

public:
	static void *thread_func(void *arg)
	{
		ThreadHelper *self = (ThreadHelper *)arg;
		self->m_running = true;
		while(!self->m_stop)
			self->run();
		self->m_running = false;
        return NULL;
	}

	ThreadHelper()
		: m_running(false)
		, m_stop(false)
	{}

	virtual ~ThreadHelper(){}

	virtual void run() = 0;

	int start()
	{
		return pthread_create(&m_tid, NULL, thread_func, this);
	}

    bool isStop()
	{
		return m_stop;
	}
    
	void stop()
	{
		m_stop = true;
	}

private:
	volatile bool m_running;
	volatile bool m_stop;
	pthread_t m_tid;

};

class CAutoLock
{
public:
	CAutoLock(pthread_mutex_t& Lock) : m_lock(Lock)
	{
		pthread_mutex_lock(&m_lock);
	}
	~CAutoLock()
	{
		pthread_mutex_unlock(&m_lock);
	}
 
protected:
	pthread_mutex_t& m_lock;
};

NS_END()