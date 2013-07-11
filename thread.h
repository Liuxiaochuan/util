#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

class Thread
{
public:
	Thread();
	virtual ~Thread();

public:
	void start();

public:
	void sleep(unsigned int milliSeconds);
	void* join();
	void stop();
	void detach();

protected:
	virtual void run() = 0;

private:
#ifdef WIN32
	static unsigned __stdcall Func(void* para);
#else
	void* Func(void* para);
#endif

private:
	bool m_started;
	bool m_detached;
#ifdef WIN32
	HANDLE m_handle;
	unsigned m_threadId;
#else
	pthread_t m_threadId;
#endif
};

#endif
