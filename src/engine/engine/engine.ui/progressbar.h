#pragma once

#include "control.h"

namespace ui
{

	class progress_bar : public control
	{
	public:

		progress_bar(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch);

		void SetMaximum(float maximum);

		void SetRatio(float ratio);

		float GetRatio() const;

		void SetValue(float value);

	protected:

		float m_maximum;
		float m_value;

		std::unique_ptr<render::texture> m_texture;

		virtual void InternalDraw(const render::camera& camera);

	};

} // ui