
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

class timer
{
public:
	timer()
	{
		if(!QueryPerformanceFrequency((LARGE_INTEGER *) &m_qwTicksPerSec))
			m_qwTicksPerSec = 1000;
		m_fTicksPerSec  = (float) ((__int64) m_qwTicksPerSec );
		m_fFrequency  = m_fTicksPerSec;
	}

	virtual void Start()
	{
		m_fLastInterval = 0.0f;
		m_fLastTime = 0.0f;
		m_dwFrames = 0;
		m_fAverageFramesPerSec = 0.0f;
		m_fFramesPerSec = 0.0f;
		m_fSecsPerFrame = 0.0f;
		m_qwTicksPerFrame = 0;
		m_qwStartTicks = m_qwTicks = GetTicks();
	}

	virtual void Frame()
	{
		unsigned __int64 qw;
		qw = GetTicks();
		if(qw != m_qwTicks)
		{
			m_qwTicksPerFrame = qw - m_qwTicks;
			m_qwTicks = qw;
			m_fFramesPerSec = 0.75f * m_fFramesPerSec + 0.25f * (m_fTicksPerSec / (float) (__int64) m_qwTicksPerFrame);
			if(m_fFramesPerSec < 1.0f)
				m_fFramesPerSec = 1.0f;
			m_fSecsPerFrame = 1.0f / m_fFramesPerSec;
		}

		m_dwFrames++;

		float fTime = GetDuration();
		m_fInterval = fTime - m_fLastInterval;
		m_fLastInterval = fTime;

		// Update the scene stats once per second
		if((fTime - m_fLastTime) > 1.5f )
		{
			m_fAverageFramesPerSec = m_dwFrames / (fTime - m_fLastTime);
			m_fLastTime = fTime;
			m_dwFrames = 0;
		}
	}

	virtual unsigned __int64 GetTicks()
	{
		unsigned __int64 qw;

		if(QueryPerformanceCounter((LARGE_INTEGER *) &qw))
		{
			while(!qw)
				QueryPerformanceCounter((LARGE_INTEGER *) &qw);
		} 
		else
		{
			qw = (unsigned __int64) timeGetTime();
		}
		return qw;
	}

	float GetMilliseconds()
	{
		unsigned __int64 qw;

		if(QueryPerformanceCounter((LARGE_INTEGER *) &qw))
		{
			while(!qw)
				QueryPerformanceCounter((LARGE_INTEGER *) &qw);
		} 
		else
		{
			qw = (unsigned __int64) timeGetTime();
		}
		return float(qw / m_fFrequency);
	}

	float GetDuration()
	{
		INT64 qwDuration = (__int64)(GetTicks() - m_qwStartTicks);
		float fDuration = (float)qwDuration / (float)(__int64)m_qwTicksPerSec;
		return fDuration;
	}

	virtual float GetInterval() const { return m_fInterval; }
	virtual float GetFramesPerSec() const { return m_fFramesPerSec; }
	virtual float GetAverageFramesPerSec() const { return m_fAverageFramesPerSec; }
	virtual float GetSecsPerFrame() const { return m_fSecsPerFrame; }
	virtual unsigned __int64 GetTicksPerSec() const { return m_qwTicksPerSec; }
	virtual unsigned __int64 GetTicksPerFrame() const { return m_qwTicksPerFrame; }

protected:
	float m_fTicksPerSec;
	float m_fFramesPerSec;
	float m_fAverageFramesPerSec;
	float m_fSecsPerFrame;
	DWORD m_dwFrames;
	float m_fLastTime;
	float m_fInterval;
	float m_fLastInterval;
	float m_fFrequency;

	unsigned __int64 m_qwTicks;
	unsigned __int64 m_qwStartTicks;
	unsigned __int64 m_qwTicksPerSec;
	unsigned __int64 m_qwTicksPerFrame;
};
