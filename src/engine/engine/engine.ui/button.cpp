#include "button.h"
#include "engine.ui/align.h"
#include <string>

namespace ui
{
	button::button(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_state(Default)
	{
		//m_spriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(core->GetDevice(), spritePath
		m_colors[Default] = render::color::BLUE;
		m_colors[Pressed] = render::color::GREEN;
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
		if  ( m_spriteSheet != nullptr )
		{
			//m_spriteSheet->Update(deltaTime);
		}

		if ( m_sprite != nullptr )
		{
			m_sprite->Update(deltaTime);
		}

		control::Update(deltaTime);
	}

	void button::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		if ( m_spriteSheet != nullptr )
		{
			//m_spriteSheet->Draw(L"default", 0, m_rectangle.Position());
//			m_spriteSheet->Draw(m_rectangle.Position().x(), m_rectangle.Position().y(), render::color::WHITE, 0.f, math::vector2::Zero);

			m_sprite->Draw(m_rectangle.Position());
		}
		else
		{
			render::color color = m_colors[m_state];
			m_spriteBatch->Draw(*m_core->GetWhiteTexture()->GetView(), m_rectangle, nullptr, color);
		}

		if (m_font != nullptr && m_showText)
		{
			math::vector2 size = static_cast<math::vector2>( m_font->MeasureString(m_text.c_str()) );	

			math::rectangle textRectangle = m_rectangle;
			textRectangle.SetSize(size);

			align a(textRectangle, m_rectangle);
			a.Apply(ui::align::Center | ui::align::Middle);
		
			m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), textRectangle.Position(), m_foregroundColor);
		}

		
		//m_spriteBatch->Draw(m_textures[Default]->GetView(), rc, nullptr, m_backgroundColor);

		//m_spriteBatch->Draw(m_textures[Pressed]->GetView(), rc, nullptr, m_backgroundColor);

		//m_spriteBatch->Draw(m_textures[Selected]->GetView(), rc, nullptr, m_backgroundColor);

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

	

	std::shared_ptr<render::sprite> m_sprite;

	void button::Load(const wchar_t* spritePath, int frameWidth, int frameHeight)
	{
		SetSize(math::vector2(frameWidth, frameHeight), ui::control::Pixels );
		m_spriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(m_core->GetDevice(), spritePath));
		
		size_t defaultAnimID;		
		auto& defaultAnim = m_spriteSheet->CreateAnimation(L"default", defaultAnimID);
		defaultAnim.AddFrame(math::rectangle(0,0,200,200), 0.33f);
		defaultAnim.AddFrame(math::rectangle(200,0,200,200), 0.33f);
		defaultAnim.AddFrame(math::rectangle(400,0,200,200), 0.33f);
		defaultAnim.AddFrame(math::rectangle(600,0,200,200), 0.33f);
		defaultAnim.AddFrame(math::rectangle(800,0,200,200), 0.33f);
		defaultAnim.Loop() = true;
		
		size_t pressedAnimID;
		auto& pressedAnim = m_spriteSheet->CreateAnimation(L"pressed", pressedAnimID);
		pressedAnim.AddFrame(math::rectangle(400,0,200,200), 0.33f);
		pressedAnim.AddFrame(math::rectangle(200,0,200,200), 0.33f);
		pressedAnim.AddFrame(math::rectangle(000,0,200,200), 0.33f);

		m_sprite = m_spriteSheet->Create();		
		m_sprite->OnEnterFrame() += [&](void*, const render::spritesheet::animation_track& track){ if ( track.FrameNumber() == 3 ) { OutputDebugStringW( L"\nEnter!" ); } };
		m_sprite->OnExitFrame() += [&](void*, const render::spritesheet::animation_track& track) { if ( track.IsLastFrame() ) { OutputDebugStringW( L"\nExit!" ); } };
		m_sprite->OnAnimationFinished() += [&](void*, const render::spritesheet::animation_track& ) { OutputDebugStringW( L"\nDone!" ); };
		m_sprite->Play(defaultAnimID);

		/*auto& anim = m_spriteSheet->CreateAnimation(L"default");
		anim.AddFrame(math::rectangle(0,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(200,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(400,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(600,0,200,200), 0.1f);
		anim.AddFrame(math::rectangle(800,0,200,200), 0.1f);*/

		//m_spriteSheet->Play(L"default");
	}



	void button::SetState(eState state)
	{
		(void)state;
	//	auto& anim = m_spriteSheet->CreateAnimation(L"Test");
//		anim.AddFrame(math::rectangle(0,0,160,160), -1.f);
	}

}