#include "progressbar.h"
#include "engine.ui/control.h"
#include "engine.ui/helpers.h"
#include "engine.render/texture.h"

namespace ui
{
	progress_bar::progress_bar(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch)
		: control(core, font, spriteBatch)
		, m_value(0)
		, m_maximum(100.f)
		, m_texture(nullptr)
	{
	}

	void progress_bar::SetMaximum(float maximum)
	{
		if ( maximum > 0.f )
		{
			float ratio = m_value / m_maximum;
			m_maximum = maximum;
			m_value = m_maximum * ratio;
		}
		else
		{
			throw new std::exception("The maximum must be greater than zero.");
		}
	}

	void progress_bar::SetRatio(float ratio)
	{
		m_value = m_maximum * ratio;
		math::Clamp(m_value, 0.f, m_maximum);
	}

	float progress_bar::GetRatio() const 
	{
		return (m_maximum > 0.f) ? m_value / m_maximum : 0.f;
	}

	void progress_bar::SetValue(float value)
	{
		m_value = value;
		math::Clamp(m_value, 0.f, m_maximum);
	}

	void progress_bar::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		m_backgroundColor.A() = m_alpha;
		m_foregroundColor.A() = m_alpha;

		m_spriteBatch->Draw(*m_core->GetWhiteTexture()->GetView(), m_rectangle, nullptr, m_backgroundColor);

		math::rectangle progressRect = m_rectangle;
		progressRect.Width() = GetRatio() * m_rectangle.Width();
		m_spriteBatch->Draw(*m_core->GetWhiteTexture()->GetView(), progressRect, nullptr, m_foregroundColor);
	}
}