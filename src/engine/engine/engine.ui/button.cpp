#include "button.h"
#include "engine.ui/align.h"
#include <string>

namespace ui
{
	button::button(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_state(Default)
	{
	}

	bool button::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		auto mouse = inputState.GetMouse();
		if ( mouse != nullptr )
		{			
			const bool leftMouseButtonPressed = mouse->ButtonPressed(input::mouse::eButton::Left);
			if ( m_mouseOver && leftMouseButtonPressed )
		{
				if ( m_sprite != nullptr )
				{
					m_sprite->Play(Pressed);
				}

				m_state = Pressed;
				m_onPressed.Invoke(this, *this);
				return true;
			}
			else
			if ( m_state == Pressed && !leftMouseButtonPressed )
			{
				if ( m_sprite != nullptr )
				{
					m_sprite->Play(Default);
				}

				m_state = Default;
				m_onReleased.Invoke(this, *this);
		}
		}

		return control::HandleInput(deltaTime, inputState);
	}

	void button::Update(float deltaTime)
	{
		if ( m_sprite != nullptr )
		{
			m_sprite->Update(deltaTime);
		}

		control::Update(deltaTime);
	}

	void button::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		m_spriteBatch->End();
		if ( m_sprite )
		{
			m_sprite->Draw(m_rectangle.Position(), render::color::WHITE, 0.f, math::vector2::Zero, math::rectangle::Zero, m_rectangle.Size() );
		}
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);

		if (m_font != nullptr && m_showText)
		{
			math::vector2 size = static_cast<math::vector2>( m_font->MeasureString(m_text.c_str()) );	

			math::rectangle textRectangle = m_rectangle;
			textRectangle.SetSize(size);

			align a(textRectangle, m_rectangle);
			a.Apply(ui::align::Center | ui::align::Middle);
		
			m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), textRectangle.Position(), m_foregroundColor);
		}

	}

	void button::SetText(const std::wstring& text)
	{
		m_text = text;
		Refresh();
	}

	void button::Refresh()
	{
		//math::vector2 size = static_cast<math::vector2>( m_font->MeasureString(m_text.c_str()) );		
		//m_rectangle.SetSize(size);
		//m_rectangle.Inflate(4.f, 4.f);
	}

	void button::Load(const std::wstring& path)
	{
		m_spriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(m_core->GetDevice(), path));
		
		size_t defaultAnimID;		
		auto& defaultAnim = m_spriteSheet->CreateAnimation(L"default", defaultAnimID);
		defaultAnim.AddFrame(math::rectangle(0,0,190,44), 0.f);
		//defaultAnim.AddFrame(math::rectangle(0,96,186,48), 0.33f);
		//defaultAnim.AddFrame(math::rectangle(0,144,186,48), 0.33f);
		//defaultAnim.AddFrame(math::rectangle(0,192,186,48), 0.33f);
		defaultAnim.Loop() = false;
		
		size_t pressedAnimID;
		auto& pressedAnim = m_spriteSheet->CreateAnimation(L"pressed", pressedAnimID);
		pressedAnim.AddFrame(math::rectangle(0, 45, 190, 42), 0.33f);
		pressedAnim.Loop() = false;

		m_sprite = m_spriteSheet->Create();		
		//m_sprite->OnEnterFrame() += [&](void*, const render::spritesheet::animation_track& track){ if ( track.FrameNumber() == 3 ) { OutputDebugStringW( L"\nEnter!" ); } };
		//m_sprite->OnExitFrame() += [&](void*, const render::spritesheet::animation_track& track) { if ( track.IsLastFrame() ) { OutputDebugStringW( L"\nExit!" ); } };
		//m_sprite->OnAnimationFinished() += [&](void*, const render::spritesheet::animation_track& ) { OutputDebugStringW( L"\nDone!" ); };
		m_sprite->Play(defaultAnimID);

		/*auto& anim = m_spriteSheet->CreateAnimation(L"default");
		anim.AddFrame(math::rectangle(0,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(200,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(400,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(600,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(800,0,200,200), 0.1f);*/

		//m_spriteSheet->Play(L"default");
	}

}