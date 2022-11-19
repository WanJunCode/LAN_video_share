#pragma once

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

inline unsigned int PthreadSelf()
{
#ifdef WIN32
    return ::GetCurrentThreadId();
#else
    return ::pthread_self();
#endif
}