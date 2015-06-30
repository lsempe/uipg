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

	void slider::Load(const std::wstring& path)
	{
		m_cursorSpriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(m_core->GetDevice(), path));

		size_t defaultAnimID;
		auto& cursor = m_cursorSpriteSheet->CreateAnimation(L"cursor", defaultAnimID);
		cursor.AddFrame(math::rectangle(273,306,10,16), 0.f);

		m_cursorSprite = m_cursorSpriteSheet->Create();
		m_cursorSprite->Play(defaultAnimID);
	}

	void slider::Update(float deltaTime)
	{
		scrollbar::Update(deltaTime);

		if (m_cursorSprite)
		{
			m_cursorSprite->Update(deltaTime);
		}
	}

	void slider::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);

		wchar_t text[16];
		wsprintf(text, TEXT("%d"), Value());
		math::vector2 textSize = static_cast<math::vector2>(m_font->MeasureString(text));
		m_font->DrawString(m_spriteBatch.get(), text, math::vector2(m_rectangle.Right() + (textSize.x() * 0.25f), m_rectangle.Top()));

		if (m_cursorSprite)
		{
			m_spriteBatch->End();
			{
				m_cursorSprite->Draw(m_cursor.Position(), render::color::WHITE, 0.f, math::vector2::Zero, math::rectangle::Zero, m_cursor.Size());
			}
			m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
		}
		else
		{
			m_spriteBatch->Draw(textureView, m_cursor, nullptr, m_foregroundColor);
		}
	}
}