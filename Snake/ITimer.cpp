#include "ITimer.h"
#include <Windows.h>
using namespace GameManager;

ITimer::ITimer()
:	m_SecondsPerCount(0.0),
	m_DeltaTime(-1.0),
	m_BaseTime(0),
	m_PausedTime(0),
	m_StopTime(0),
	m_PrevTime(0),
	m_CurrTime(0),
	m_Stopped(false)
{
	__int64 countPerSeconds;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSeconds);
	m_SecondsPerCount = 1.0 / (double)countPerSeconds;
}

float ITimer::TotalTime() const
{
	if (m_Stopped)
	{
		return (float)( ( ( m_StopTime - m_PausedTime ) - m_BaseTime )*m_SecondsPerCount );
	}
	else
	{
		return (float)( ( ( m_CurrTime - m_PausedTime ) - m_BaseTime )*m_SecondsPerCount );
	}
}

float ITimer::DeltaTime() const
{
	return (float)m_DeltaTime;
}

void ITimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_PausedTime = 0;
	m_Stopped = false;
}

void ITimer::Start()
{
	if (m_Stopped)
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

		m_PausedTime += ( startTime - m_StopTime );
		m_StopTime = 0;
		m_PrevTime = startTime;
		m_Stopped = false;
	}
}

void ITimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 stopTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);

		m_StopTime = stopTime;
		m_Stopped = true;
	}
}

void ITimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
	}
	else
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_CurrTime = currTime;
		m_DeltaTime = ( m_CurrTime - m_PrevTime )*m_SecondsPerCount;
		m_PrevTime = m_CurrTime;

		if (m_DeltaTime < 0.0)
		{
			m_DeltaTime = 0.0;
		}
	}
}


