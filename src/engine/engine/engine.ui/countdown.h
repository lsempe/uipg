#pragma once

#include "engine.core/event_handler.h"

namespace ui
{

class countdown
{
public:

	countdown(float duration)
		: m_duration(duration)
		, m_elapsed(0.f)
	{
	}

	void Update(float deltaTime)
	{
		if ( m_elapsed > 0.f )
		{
			m_elapsed -= deltaTime;

			if ( m_elapsed <= 0.f )
			{
				m_onDone.Invoke(this, *this);
			}
		}
	}

	void Start()
	{
		m_elapsed = m_duration;
	}

	const bool IsDone() const { return m_elapsed <= 0.f; }
	
	event_handler<countdown&>& OnDone() { return m_onDone; }

private:

	float m_duration;
	float m_elapsed;

	event_handler<countdown&> m_onDone;

};

} // ui