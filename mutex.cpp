#include "mutex.h"

Mutex::Mutex()
{
#ifdef WIN32
	InitializeCriticalSection(&m_mutex);
#else
	pthread_mutex_init(&m_mutex, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
	DeleteCriticalSection(&m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void Mutex::lock()
{
#ifdef WIN32
	EnterCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}

void Mutex::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}

#ifdef WIN32
CRITICAL_SECTION* Mutex::getCriticalSection()
#else
pthread_mutex_t* Mutex::getMutex()
#endif
{
	return &m_mutex;
}


MutexGuard::MutexGuard(Mutex& mutex) : m_mutex(mutex)
{
	m_mutex.lock();
}

MutexGuard::~MutexGuard()
{
	m_mutex.unlock();
}
