#include "input_cursor.h"
#include "CommonStates.h"

namespace ui
{
	input_cursor::input_cursor(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font,  const std::shared_ptr<render::platform::sprite_batch>& spriteBatch, std::wstring path)
		: control(core, font, spriteBatch)
	{
		if ( spriteBatch == nullptr )
		{
			m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(core->GetDevice()->GetImmediateContext()));
		}

		m_texture = std::shared_ptr<render::texture>(new render::texture(m_core->GetDevice(), path));
		m_rectangle.SetSize( math::vector2(m_texture->Width(), m_texture->Height()) );		
		//m_rectangle.SetSize( math::vector2(256,256) );		
	}

	void input_cursor::Draw()
	{
		DirectX::CommonStates states(m_core->GetDevice()->GetDevice());
		m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied(), nullptr, states.DepthNone());

		m_spriteBatch->Draw(*m_texture->GetView(), m_rectangle, nullptr);

		m_spriteBatch->End();
	}

	void input_cursor::SetTexture(std::shared_ptr<render::texture> texture)
	{
		m_texture = nullptr;
		m_texture = texture;
	}

}