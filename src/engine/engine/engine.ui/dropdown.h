#pragma once

#include "control.h"
#include "core.h"
#include "countdown.h"
#include "scrollbar.h"

namespace ui
{

class dropdown : public control
{
public:

	enum eState
	{
		Default,
		Open
	};

	eState m_state;

	dropdown(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_scrollBar(core, font, spriteBatch)
		, m_open(false)
		, m_inputDelay(0.25f)
		, m_state(Default)
		, m_cursor(0)
		, m_selectedItem(nullptr)
		, m_arrowBoxWidth(32.f)
	{
	}

	class dropdownitem
	{
	public:

		dropdownitem(const std::wstring& text, const render::color& color, std::shared_ptr<DirectX::SpriteFont> font)
			: m_text(text)
			, m_visible(false)
			, m_color(color)
			, m_font(font)
		{}

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			m_mouseOver = false;
			auto mouse = inputState.GetMouse();
			if ( mouse != nullptr )
			{				
				const auto position = mouse->GetPosition();
				if ( position.y() >= m_position.y() && position.y() <= m_position.y() + m_font->GetLineSpacing() )
				{
					m_mouseOver = true;
					if ( mouse->ButtonPressed(input::mouse::eButton::Left) )
					{
						m_onSelected.Invoke(this, *this);
						return true;
					}
				}
			}

			return false;
		}

		virtual void Draw(const math::vector2& position, std::shared_ptr<DirectX::SpriteFont> font, std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		{
			m_position = position;			
			m_font->DrawString(spriteBatch.get(), m_text.c_str(), position, m_mouseOver ? render::color::RED : m_color);
		}

		bool& Visible() { return m_visible; }
		render::color& Color() { return m_color; }
		std::wstring& Text() { return m_text; }
		bool& MouseOver() { return m_mouseOver; }

		event_handler<dropdownitem&>& OnSelected() { return m_onSelected; }

	protected:

		math::vector2 m_position;
		render::color m_color;
		std::wstring m_text;
		bool m_visible;
		bool m_mouseOver;
		std::shared_ptr<DirectX::SpriteFont> m_font;
		event_handler<dropdownitem&> m_onSelected;
	};

	void Add(const std::wstring text)
	{
		dropdownitem item(text, render::color::WHITE, m_font);
		item.OnSelected() += [=]( void*, dropdownitem& item) { m_selectedItem = &item; m_open = false; };
	
		m_items.push_back(item);

		if (m_items.size() == 1 )
		{
			m_selectedItem = &(*m_items.begin());
		}

		Refresh();
	}

	void Clear()
	{
		m_items.clear();
	}

	virtual void Refresh()
	{
		if ( m_items.size() > 1 )
		{
			auto backRect = m_rectangle;
			backRect.Top() = m_rectangle.Top() + m_font->GetLineSpacing();
			backRect.Height() = m_font->GetLineSpacing() * ((m_items.size()/2) - 1);

			m_scrollBar.SetBackgroundColor( render::color::LIGHTGRAY );
			m_scrollBar.SetForegroundColor( render::color::GREEN );
			m_scrollBar.SetSize( math::vector2(m_rectangle.Width() * 0.1f, backRect.Height() ) );
			m_scrollBar.SetPosition( math::vector2(m_rectangle.Right() - m_scrollBar.Size().x(), m_rectangle.Top() + m_font->GetLineSpacing() ) );

			m_scrollBar.Visible() = true;
		}
		else
		{
			m_scrollBar.Visible() = false;
		}
		
	}

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
	{
		math::rectangle arrowBox = m_rectangle;
		arrowBox.Left() = m_rectangle.Right() - 32.f;
		arrowBox.Width() = 32.f;

		auto mouse = inputState.GetMouse();
		if ( mouse != nullptr )
		{
			if ( arrowBox.Contains( mouse->GetPosition() ) )
			{
				if ( m_state == Default && m_inputDelay.IsDone() && mouse->ButtonPressed(input::mouse::eButton::Left) )
				{
					m_open = !m_open;
					m_inputDelay.Start();
					m_state = Open;
					return true;
				}
			}

			if ( m_state == Open && mouse->ButtonReleased(input::mouse::eButton::Left) )
			{
				m_state = Default;
			}			
		}

		if ( m_scrollBar.HandleInput(deltaTime, inputState) )
		{
			return true;
		}

		if ( !m_open )
			return false;

		for ( auto& it : m_items )
		{
			if ( it.Visible() && it.HandleInput(deltaTime, inputState) )
				return true;
		}

		return false;
	}

	virtual void Update(float deltaTime)
	{
		m_inputDelay.Update(deltaTime);

		m_scrollBar.Update(deltaTime);
	}

	virtual void InternalDraw()
	{
		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);

		// Draw the arrow box
		math::rectangle arrowBox = m_rectangle;
		arrowBox.Left() = m_rectangle.Right() - m_arrowBoxWidth;
		arrowBox.Width() = m_arrowBoxWidth;
		m_spriteBatch->Draw(textureView, arrowBox, nullptr, render::color::RED);

		if ( m_selectedItem != nullptr )
		{
			m_font->DrawString(m_spriteBatch.get(), m_selectedItem->Text().c_str(), m_rectangle.Position(), m_selectedItem->Color());
		}

		if ( !m_open || m_items.size() <= 1 )
			return;

		auto backRect = m_rectangle;
		backRect.Top() = m_rectangle.Top() + m_font->GetLineSpacing();
		backRect.Height() = m_font->GetLineSpacing() * ((m_items.size()/2) - 1);

		m_spriteBatch->Draw(textureView, backRect, nullptr, m_backgroundColor);

		math::vector2 itemsPosition = m_rectangle.Position();

		size_t numItems = static_cast<size_t>(backRect.Height() / m_font->GetLineSpacing());
		const float ratio = m_scrollBar.Ratio();
		m_cursor = static_cast<size_t>( ratio * (m_items.size() - 1) );
		size_t i = 0; 
		for ( std::list<dropdownitem>::iterator it = m_items.begin(); it != m_items.end(); ++it )
		{
			if ( i < m_cursor )
			{
				it->Visible() = false;
				++i;
				continue;
			}

			it->Visible() = true;

			itemsPosition.y() += m_font->GetLineSpacing();
			if ( itemsPosition.y() >= backRect.Bottom() )
				break;

			it->Draw(itemsPosition, m_spriteBatch);
			
			++i;
		}

		if ( m_open )
		{
			auto dropDownBoxRect = m_rectangle;
			dropDownBoxRect.Height() -= m_font->GetLineSpacing();
			dropDownBoxRect.Top() += m_font->GetLineSpacing();
		}

		m_scrollBar.Draw();
	}



protected:

	float m_arrowBoxWidth;

	std::list<dropdownitem> m_items;
	dropdownitem* m_selectedItem;

	bool m_open;
	size_t m_cursor;

	countdown m_inputDelay;

	scrollbar m_scrollBar;

};

} // ui