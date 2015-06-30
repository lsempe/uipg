#pragma once

#include "control.h"

namespace render
{
	class spritesheet;
	class sprite;
}

namespace ui
{

	class progress_bar : public control
	{
	public:

		progress_bar(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch);
		void Load(const std::wstring& path);

		void SetMaximum(float maximum);

		void SetRatio(float ratio);

		float GetRatio() const;

		void SetValue(float value);

		void Update(float deltaTime) override;

		void Draw(const control&);

	protected:

		float m_maximum;
		float m_value;

		std::unique_ptr<render::texture> m_texture;
		std::unique_ptr<render::spritesheet> m_spriteSheet;
		std::shared_ptr<render::sprite> m_sprite;

		virtual void InternalDraw(const render::camera& camera);

	};

} // ui