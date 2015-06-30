#include "scrollbar.h"
#include "engine.render/spritesheet.h"

namespace ui
{
	scrollbar::scrollbar(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)		
		, m_state(Default)
		, m_lastMouseWheelValue(0)
		, m_type(Vertical)
		, m_visible(false)
	{
	}

	void scrollbar::Load(const std::wstring& path)
	{
		m_spriteSheet = std::unique_ptr<render::spritesheet>(new render::spritesheet(m_core->GetDevice(), path));

		size_t defaultAnimID;
		auto& cursor = m_spriteSheet->CreateAnimation(L"default", defaultAnimID);
		cursor.AddFrame(math::rectangle(338, 189, 48, 50), 0.f);

		m_sprite = m_spriteSheet->Create();
		m_sprite->Play(defaultAnimID);

	}

	bool scrollbar::HandleInput(float, const input::input_state& inputState)
	{			
		auto& edge = (m_type == Vertical) ? m_cursor.Top() : m_cursor.Left();
		auto& cursorSize = (m_type == Vertical) ? m_cursor.Height() : m_cursor.Width();
		const auto minimal = (m_type == Vertical) ? m_rectangle.Top() : m_rectangle.Left();
		const auto maximal = (m_type == Vertical)  ? m_rectangle.Bottom() - m_cursor.Height() : m_rectangle.Right() - m_cursor.Width();

		auto mouse = inputState.GetMouse();
		if ( mouse != nullptr )
		{
			auto& input = (m_type == Vertical) ? mouse->GetPosition().y() : mouse->GetPosition().x();

			if ( m_cursor.Contains(mouse->GetPosition()) )
			{
				if ( mouse->ButtonPressed(input::mouse::Left) )
				{
					if ( m_state == Default )
					{
						m_state = Pressed;
						m_delta = input - edge;
					}
					else
					{
						edge = input - m_delta;
						math::Clamp(edge,  minimal, maximal);						
					}
										
					return true;
				}
				else
				{
					if ( m_state == Pressed )
					{
						m_state = Default;
					}
				}
			}
			
			if ( m_rectangle.Contains( mouse->GetPosition() ) )
			{
				if ( mouse->ButtonPressed(input::mouse::Left) )
				{
					edge = input - ( cursorSize / 2.f );
					return true;
				}			

				if ( mouse->GetMouseWheelValue() != m_lastMouseWheelValue )
				{
					ApplyMovement(static_cast<float>(m_lastMouseWheelValue - mouse->GetMouseWheelValue()));
					m_lastMouseWheelValue = mouse->GetMouseWheelValue();

					return true;
				}
			}
		}

		return false;
	}

	void scrollbar::ApplyMovement(float delta)
	{
		auto& edge = (m_type == Vertical) ? m_cursor.Top() : m_cursor.Left();
		const auto minimal = (m_type == Vertical) ? m_rectangle.Top() : m_rectangle.Left();
		const auto maximal = (m_type == Vertical)  ? m_rectangle.Bottom() - m_cursor.Height() : m_rectangle.Right() - m_cursor.Width();

		const float spacing = (m_rectangle.Height() / m_font->GetLineSpacing());		

		edge += (delta > 0) ? spacing : -(spacing);
		math::Clamp(edge,  minimal, maximal);
	}

	void scrollbar::Update(float deltaTime)
	{
		if (m_sprite)
		{
			m_sprite->Update(deltaTime);
		}
	}

	void scrollbar::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);	
	
		if (m_sprite)
		{
			m_spriteBatch->End();
			{
				m_sprite->Draw(m_cursor.Position(), m_foregroundColor, 0.f, math::vector2::Zero, math::rectangle::Zero, m_cursor.Size());
			}
			m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
		}
		else
		{
			m_spriteBatch->Draw(textureView, m_cursor, nullptr, m_foregroundColor);
		}
	}

	void scrollbar::Refresh()
	{
		m_cursor = m_rectangle;
		
		if ( m_type == Vertical )
			m_cursor.Height() = m_rectangle.Height() * 0.15f;
		else
			m_cursor.Width() = m_rectangle.Width() * 0.1f;

	}

	float scrollbar::Ratio() const 
	{ 
		math::rectangle effectiveArea = m_rectangle;

		float halfSize = 0.f;
		float edge = 0.f;
		float size = 0.f;
		float coordinate = 0.f;
		if ( m_type == Vertical ) 
		{
			halfSize = m_cursor.Height() * 0.5f;
			edge = effectiveArea.Top() + halfSize;
			size = effectiveArea.Height() - m_cursor.Height();
			coordinate = m_cursor.Top() + 0.5f;
		}
		else
		{
			halfSize = m_cursor.Width() * 0.5f;
			edge = effectiveArea.Left() + halfSize;
			size = effectiveArea.Width() - m_cursor.Width();
			coordinate = m_cursor.Left() + 0.5f;
		}	

		return ((coordinate + halfSize) - edge) / size; 
	}
}