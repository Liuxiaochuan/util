#ifndef _MUTEX_H_
#define _MUTEX_H_

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif
#include "noncopyable.h"

class Mutex : private Noncopyable
{
public:
  	Mutex();
	~Mutex();

public:
	void lock();
	void unlock();

public:
#ifdef WIN32
	CRITICAL_SECTION* getCriticalSection();
#else
	pthread_mutex_t* getMutex();
#endif

private:
#ifdef WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif
};


class MutexGuard : private Noncopyable
{
public:
	explicit MutexGuard(Mutex& mutex);
	~MutexGuard();

private:
	Mutex& m_mutex;
};

#endif
