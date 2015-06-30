#include "input_cursor.h"
#include "CommonStates.h"

namespace ui
{
	input_cursor::input_cursor(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font,  const std::shared_ptr<render::platform::sprite_batch>& spriteBatch, std::wstring path)
		: control(core, font, spriteBatch)
		, m_pressedAnimID(0)
		, m_defaultAnimID(0)
		, m_currentAnimID(0)
	{
		if ( spriteBatch == nullptr )
		{
			m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(core->GetDevice()->GetImmediateContext()));
		}

		if (!path.empty())
		{
			m_texture = std::shared_ptr<render::texture>(new render::texture(m_core->GetDevice(), path));
			m_rectangle.SetSize(math::vector2(m_texture->Width(), m_texture->Height()));
		}
	}

	void input_cursor::Load(const std::wstring& path)
	{
		m_spriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(m_core->GetDevice(), path));

		auto& defaultAnim = m_spriteSheet->CreateAnimation(L"default", m_defaultAnimID);
		defaultAnim.AddFrame(math::rectangle(178, 448, 16, 16), 0.f);
		defaultAnim.Loop() = false;

		auto& pressedAnim = m_spriteSheet->CreateAnimation(L"pressed", m_pressedAnimID);
		pressedAnim.AddFrame(math::rectangle(196, 448, 16, 16), 0.f);
		pressedAnim.Loop() = false;

		m_sprite = m_spriteSheet->Create();
		m_sprite->Play(m_defaultAnimID);
		m_currentAnimID = m_defaultAnimID;
	}

	bool input_cursor::HandleInput(float /*deltaTime*/, input::input_state& inputState)
	{
		auto mouse = inputState.GetMouse();
		if (mouse)
		{
			if (m_currentAnimID != m_pressedAnimID && mouse->ButtonPressed(input::mouse::eButton::Left))
			{
				m_sprite->Play(m_pressedAnimID);
				m_currentAnimID = m_pressedAnimID;
				return true;
			}
			else
			if (m_currentAnimID == m_pressedAnimID && mouse->ButtonReleased(input::mouse::eButton::Left))
			{
				m_currentAnimID = m_defaultAnimID;
				m_sprite->Play(m_defaultAnimID);
				return true;
			}
		}

		return false;
	}

	void input_cursor::Update(float deltaTime)
	{
		control::Update(deltaTime);

		m_sprite->Update(deltaTime);
	}

	void input_cursor::Draw()
	{
		if (m_texture)
		{
			DirectX::CommonStates states(m_core->GetDevice()->GetDevice());
			m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied(), nullptr, states.DepthNone());

			m_spriteBatch->Draw(*m_texture->GetView(), m_rectangle, nullptr);

			m_spriteBatch->End();
		}

		if (m_sprite)
		{
			m_sprite->Draw(m_rectangle.Position());
		}
	}

	void input_cursor::SetTexture(std::shared_ptr<render::texture> texture)
	{
		m_texture = nullptr;
		m_texture = texture;
	}

}