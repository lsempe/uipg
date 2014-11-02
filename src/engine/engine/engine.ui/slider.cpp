#include "slider.h"
#include "engine.render/spritesheet.h"
#include "engine.render/texture.h"

namespace ui
{
	slider::slider(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: scrollbar(core, font, spriteBatch)
		, m_minimum(0)
		, m_maximum(100)
	{
		m_type = Horizontal;
	}

	void slider::SetCursor(const std::wstring texturePath)
	{
		size_t defaultAnimID;
		auto& cursor = m_cursorSprite->CreateAnimation(L"cursor", defaultAnimID);
		cursor.AddFrame(math::rectangle(416,72,36,42), 0.f);
	}

	void slider::Update(float deltaTime)
	{
		scrollbar::Update(deltaTime);
	}

	void slider::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		wchar_t text[16];

		wsprintf(text, TEXT("%d"), Value());
		math::vector2 textSize = static_cast<math::vector2>( m_font->MeasureString(text) ); 
		m_font->DrawString(m_spriteBatch.get(), text,  math::vector2(m_rectangle.Right() + (textSize.x() * 0.25f), m_rectangle.Top()));

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);
		
		if ( m_cursorSprite == nullptr )
		{
			m_spriteBatch->Draw(textureView, m_cursor, nullptr, m_foregroundColor);
		}
	}
}