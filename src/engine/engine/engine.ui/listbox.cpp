#include "listbox.h"

namespace ui
{
	listbox::listbox(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<DirectX::SpriteBatch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_scrollBar(core, font, spriteBatch)
		, m_lastMouseWheelValue(0)
		, m_sorting(Descending)
		, m_multiSelection(true)
	{
	}

	listbox::~listbox()
	{
		Clear();
	}

	void listbox::Refresh()
	{
		auto backRect = m_rectangle;

		m_scrollBar.SetBackgroundColor( render::color::LIGHTGRAY );
		m_scrollBar.SetForegroundColor( render::color::GREEN );

		if ( m_items.size() > 1 )
		{
			backRect.Top() = m_rectangle.Top();
			backRect.Height() = m_font->GetLineSpacing() * ((m_items.size()/3) );

			m_scrollBar.Visible() = true;
		}
		else
		{
			m_scrollBar.Visible() = false;
		}

		m_scrollBar.SetSize( math::vector2(m_rectangle.Width() * 0.1f, m_rectangle.Height() ), ui::control::Pixels );
		m_scrollBar.SetPosition( math::vector2(m_rectangle.Right() - m_scrollBar.Size().x(), m_rectangle.Top()), ui::control::Pixels  );

	}

	bool listbox::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		UNREFERENCED(deltaTime);

		if ( inputState.GetMouse() != nullptr )
		{
			auto mouse = inputState.GetMouse();
			if ( m_rectangle.Contains(mouse->GetPosition()) )
			{				
				if ( mouse->GetMouseWheelValue() != m_lastMouseWheelValue )
				{					
					float delta = static_cast<float>( m_lastMouseWheelValue - mouse->GetMouseWheelValue() );
				
					m_scrollBar.ApplyMovement(delta);
					
					m_lastMouseWheelValue = mouse->GetMouseWheelValue();		

					return true;
				}
			}	
		}

		if ( m_scrollBar.HandleInput(deltaTime, inputState) )
		{
			return true;
		}

		for ( auto it : m_items )
		{
			if ( it->Visible() && it->HandleInput(deltaTime, inputState) )
				return true;
		}

		return false;
	}

	void listbox::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		
		auto firstItem = m_items.begin();		

		auto backRect = m_rectangle;
		backRect.Top() = m_rectangle.Top();
		//backRect.Height() = ((firstItem != m_items.end() ) ? (*firstItem)->Rectangle().Height() : m_font->GetLineSpacing())  * ((m_items.size()/2) );
		backRect.Height() = m_rectangle.Height();// m_font->GetLineSpacing()  * (m_items.size()/2);

		m_backgroundColor.A() = m_alpha;

		m_spriteBatch->Draw(textureView, backRect, nullptr, m_backgroundColor);

		math::vector2 itemsPosition = m_rectangle.Position();
		if ( m_sorting == Ascending ) 
		{
			itemsPosition.y() = backRect.Bottom() - m_font->GetLineSpacing();
		}

		const float ratio = m_scrollBar.Ratio();
		size_t cursor = static_cast<size_t>(ratio * (m_items.size()- 2));
		size_t i = 0; 

		for ( auto it : m_items )
		{
			if ( i < cursor )
			{
				it->Visible() = false;
				++i;
				continue;
			}

			if ( itemsPosition.y() + m_font->GetLineSpacing() >= m_rectangle.Bottom() )
				break;

			it->Visible() = true;
			
			it->Draw(itemsPosition, m_spriteBatch);
			
			if ( m_sorting == Descending ) 
			{
				itemsPosition.y() += m_font->GetLineSpacing();
				if ( itemsPosition.y() >= backRect.Bottom() )
					break;
			}
			else
			{
				itemsPosition.y() -= m_font->GetLineSpacing();
				if ( itemsPosition.y() <= backRect.Top() )
					break;
			}
			++i;
		}

		if ( itemsPosition.y() >= backRect.Height() )
		{
			m_scrollBar.Draw(camera);
		}

	}


}