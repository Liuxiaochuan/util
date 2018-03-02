#include "thread.h"

Thread::Thread() : m_started(false), m_detached(false)
{

}

Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	if (!m_started)
	{
#ifdef WIN32
		if (m_handle = (HANDLE)_beginthreadex(NULL, 0, Func, (void*)this, 0, &m_threadId))
		{
			if (m_detached)
			{
				CloseHandle(m_handle);
			}
			m_started = true;
		}
#else
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		if (m_detached)
		{
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		}
		if (0 == pthread_create(&m_threadId, &attr, Func, (void*)this))
		{
			m_started = true;
		}
		pthread_attr_destroy(&attr);
#endif
	}
}

void Thread::sleep(unsigned int milliSeconds)
{
#ifdef WIN32
	::Sleep(milliSeconds);
#else
	timeval timeout = {milliSeconds / 1000, milliSeconds % 1000};
	select(0, NULL, NULL, NULL, &timeout);
#endif
}

void* Thread::join()
{
#ifdef WIN32
	DWORD status = (DWORD)NULL;
	if (m_started && !m_detached)
	{
		WaitForSingleObject(m_handle, INFINITE);
		GetExitCodeThread(m_handle, &status);
		CloseHandle(m_handle);
		m_detached = true;
	}
	return (void*)status;
#else
	void* status = NULL;
	if (m_started && !m_detached)
	{
		pthread_join(m_threadId, &status);
		m_detached = true;
	}
	return status;
#endif
}

void Thread::stop()
{
	if (m_started && !m_detached)
	{
#ifdef WIN32
		TerminateThread(m_handle, NULL);
		CloseHandle(m_handle);
#else
		pthread_cancel(m_threadId);
		pthread_detach(m_threadId);
#endif
		m_detached = true;
	}
}

void Thread::detach()
{
	if (m_started && !m_detached)
	{
#ifdef WIN32
		CloseHandle(m_handle);
#else
		pthread_detach(m_threadId);
#endif
	}
	m_detached = true;
}

#ifdef WIN32
unsigned __stdcall Thread::Func(void* para)
#else
void* Thread::Func(void* para)
#endif
{
	Thread* thread = (Thread*)para;
	thread->run();
#ifdef WIN32
	return 0;
#else
	return (void*)0;
#endif
}
