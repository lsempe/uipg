#pragma once

#include "engine.core/sys.h"
#include  <memory>
#include  <list>
#include <functional>
#include <math.h>
#include "engine.math/math.h"
#include "engine.core/event_handler.h"

namespace ui
{
	
	namespace curves
	{	
		// t: The ratio of interpolation [0..1]
	
		float none(float t);
		float linear(float t);
		float quadratic(float t);
		float cubic(float t);
		float quart(float t);
		float quint(float t);
		float sine(float t);
		float expo(float t);
		float elastic(float t);
		float circular(float t);

		template <float (*function)(float)>
		float ease_in(float t)
		{
			return function(t);
		}

		template <float (*function)(float)>
		float ease_out(float t)
		{
			return 1.0f - ease_in<function>( 1.0f - t );
		}

		template <float (*function)(float)>
		float ease_in_out(float t)
		{
			if ( t < 0.5f )
				return ease_in<function>( 2.0f * t ) * 0.5f;
			else
				return 0.5f + ease_out<function>( 2.0f * t - 1.0f ) * 0.5f;
		}
	 
	} // curves

	//////////////////////////////////////////////////

	class animation
	{
	public:

		typedef std::function<void(float)> update_function;
		typedef std::function<float(float)> ease_function;
		typedef std::function<void()> finish_function;

		animation(float start, float end, float duration, ease_function easeFunction, update_function updateFunction, finish_function finishFunction)
			: m_start(start)
			, m_end(end)
			, m_duration(duration)
			, m_time(0)
			, m_easeFunction(easeFunction)
			, m_updateFunction(updateFunction)
			, m_finishFunction(finishFunction)
		{}

		static void swap(float& a, float& b)
		{
			const float tmp = a;
			a = b;
			b = tmp;
		}

		animation(float& value, float end, float duration, ease_function easeFunction,  finish_function finishFunction = nullptr)
			: m_start(value)
			, m_end(end)
			, m_duration(duration)
			, m_time(0)
			, m_easeFunction(easeFunction)
			, m_finishFunction(finishFunction)
		{
			using namespace std::placeholders;
			m_updateFunction = std::bind( &swap, std::ref(value), _1);
		}

		animation(animation&& rhs)
		{
			m_start = rhs.m_start;
			m_end = rhs.m_end;
			m_duration = rhs.m_duration;
			m_time = rhs.m_time;
			m_easeFunction = rhs.m_easeFunction;
			m_updateFunction = rhs.m_updateFunction;
			m_finishFunction = rhs.m_finishFunction;
		}

		animation& operator = (animation&& rhs)
		{
			m_start = rhs.m_start;
			m_end = rhs.m_end;
			m_duration = rhs.m_duration;
			m_time = rhs.m_time;
			m_easeFunction = rhs.m_easeFunction;
			m_updateFunction = rhs.m_updateFunction;
			m_finishFunction = rhs.m_finishFunction;
			return *this;
		}

		void Update(float deltaTime)
		{
			const float dt( math::Min(m_duration - m_time, deltaTime) );
		
			m_time += dt;
		
			const float t = m_easeFunction( m_time / m_duration );
			const float result = math::Lerp(m_start, m_end, t); 
		
			m_updateFunction(result);
		
			if ( m_time >= m_duration )
			{			
				m_onFinished.Invoke(this, 0);
				m_finishFunction();
			}
		}

		bool IsFinished() const { return m_time >= m_duration; }

		event_handler<int>& OnFinished() { return m_onFinished; }

	protected:

		 update_function	m_updateFunction;
		 ease_function		m_easeFunction;
		 finish_function	m_finishFunction;

		 event_handler<int> m_onFinished;

		 float m_start;
		 float m_end;
		 float m_duration;
		 float m_time;
	
	};



	class interpolator
	{
	public:

		void Add(animation& anim)
		{
			m_activeAnimations.push_back(std::move(anim));
		}

		void Update(float deltaTime)
		{
			auto anim = m_activeAnimations.begin();
			while ( anim != m_activeAnimations.end() )
			{
				anim->Update(deltaTime);
			
				if ( anim->IsFinished() )
				{
					anim = m_activeAnimations.erase(anim);
				}
				else
				{
					++anim;
				}
			}
		}

		size_t ActiveAnimationCount() const { return m_activeAnimations.size(); }

	protected:

		std::list<animation> m_activeAnimations;

	};

} // ui