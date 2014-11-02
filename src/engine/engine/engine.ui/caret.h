#pragma once

#include <memory>

#include "SpriteBatch.h"

#include "engine.render/color.h"
#include "engine.render/texture.h"
#include "engine.math/rectangle.h"
#include "engine.ui/interpolator.h"

namespace ui
{

class caret
{
public:
	caret(float blinkRate, render::color color, float x, float y, float width, float height)
		: m_blinkTimer(0.f)
		, m_blinkDuration(blinkRate)
		, m_toggle(true)
		, m_color(color)
		, m_rectangle(x,y,width,height)
	{
	}

	void SetSize(float width, float height)
	{
		m_rectangle.Width() = width;
		m_rectangle.Height() = height;
	}
				
	void SetLeft(float x)
	{
		m_rectangle.Left() = x;
	}

	void SetTop(float y)
	{
		m_rectangle.Top() = y;
	}

	void SetPosition(float x, float y)
	{
		m_rectangle.Left() = x;
		m_rectangle.Top() = y;
	}
		
	void Update(float deltaTime)
	{

#if EXAMPLE_WITHOUT_ANIMATION
		m_blinkTimer -= deltaTime;
		if ( m_blinkTimer <= 0.f )
		{
			m_blinkTimer = m_blinkDuration;
			m_toggle = !m_toggle;				
		}
		m_color.A() = m_toggle ?  1.f - (m_blinkTimer / m_blinkDuration) :  (m_blinkTimer / m_blinkDuration);
#else
		m_interpolator.Update(deltaTime);

		animation anim(m_color.A(), m_toggle ? 1.f : 0.f, 0.25f, curves::ease_in_out<curves::linear>, [&] { m_toggle = !m_toggle; } );
		if ( m_interpolator.ActiveAnimationCount() == 0 )
		{
			m_interpolator.Add(anim);
		}
#endif // EXAMPLE_WITHOUT_ANIMATION

	}

	void Draw(std::shared_ptr<render::texture> texture, std::shared_ptr<render::platform::sprite_batch> spriteBatch)
	{
		spriteBatch->Draw(*texture->GetView(), m_rectangle, nullptr, m_color);
	}

private:

	float m_blinkTimer;
	float m_blinkDuration;
	bool m_toggle;
	render::color m_color;
	math::rectangle m_rectangle;
	interpolator m_interpolator;
};

} // ui