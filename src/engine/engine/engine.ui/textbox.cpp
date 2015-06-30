#include "textbox.h"

namespace ui
{
	const unsigned int textbox::kTabSpaces = 4;

	textbox::textbox(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_caret(.25f, render::color::WHITE, 0.f, 0.f, 0.f, 0.f)
		, m_lastKey(0)
		, s_inputThrottleLength(0.5f)
		, m_inputThrottle(0.f)
		, m_longPress(false)
		, m_scrollingIndex(0)
		, m_autoCompleteSupported(false)
	{
	}

	void textbox::Refresh()
	{
		m_textPosition = math::vector2(m_rectangle.Left() + 2.f, m_rectangle.Top() + (m_font->GetLineSpacing() * 0.1f));

		math::vector2 inputTextSize = (math::vector2)m_font->MeasureString(m_text.c_str());
		m_caret.SetPosition(m_rectangle.Left() + inputTextSize.x() + 4.f, m_rectangle.Top() + (m_font->GetLineSpacing() * 0.1f) );
		m_caret.SetSize(2.f, m_font->GetLineSpacing() * 0.98f);
	}

	void textbox::Update(float deltaTime)
	{
		m_caret.Update(deltaTime);

		// Place the caret at the end of the displayed text
		math::vector2 inputTextSize = (math::vector2)m_font->MeasureString(m_text.c_str());
		m_caret.SetLeft(m_rectangle.Left() + inputTextSize.x() + 4.f);

		if ( m_inputThrottle > 0.f )
		{
			m_inputThrottle -= deltaTime;	
		}
	
		
	}
	
	bool textbox::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		UNREFERENCED(deltaTime);

		if ( !m_visible )
			return false;

		auto keyboard = inputState.GetKeyboard();
		
		auto key = keyboard->GetKeyPress();

		if ( key == 0 )
		{
			m_lastKey = 0;
			return false;
		}

		if ( key == m_lastKey )
		{
			if ( m_inputThrottle > 0.f )
				return false;
		}

		m_lastKey = key;
		m_inputThrottle = 0.5f;

		if ( key == DIK_BACKSPACE && m_text.size() > 0 )
		{				
			m_text.pop_back();
			keyboard->Flush();
			AutoComplete(true);
			return true;
		}

		// Do not allow more text than we can fit in the textbox.
		// TODO: support scrolling
		math::vector2 textMeasurement(m_font->MeasureString(m_text.c_str()));
		if (textMeasurement.x() >= m_rectangle.Width() - m_font->GetLineSpacing())
			return false;

		// By default pressing TAB will attempt to auto complete.
		
		if (key == DIK_TAB)
		{
			if (m_autoCompleteSupported)
			{
				AutoComplete();
				return true;
			}
			else
			{
				for (int i = 0; i < kTabSpaces; ++i)
				{
					m_text.push_back(L' ');
				}
				Refresh();
				keyboard->Flush();
			}
		}

		// Translate the key code into its text representation
		std::wstring description;
		const wchar_t* ch = keyboard->GetKeyDescription(key);
		if ( ch != nullptr )
		{				
			m_text.push_back(*ch);
			keyboard->Flush();
			
			AutoComplete(true);

			return true;
		}

		// Pressing enter or return will commit the autocomplete result if it is active
		// or it will invoke the OnTextEntered event.
		if ( key == DIK_RETURN || key == DIK_NUMPADENTER )
		{
			if ( m_currentResult != nullptr )
			{
				m_text.append(m_autocompleteText);
				m_autocompleteText.clear();
				m_currentResult = nullptr;
				return true;
			}
			else
			{
				m_onTextEntered.Invoke(this, m_text);
				m_text.clear(); 
				keyboard->Flush();
				return true;
			}
		}
	
		return false;
	}

	void textbox::AutoComplete(bool reset)
	{
		if ( m_trie == nullptr )
			return;

		if ( reset || m_currentResult == nullptr )
		{
			m_currentResult = m_trie->find_prefix(m_text);
		}

		if ( m_currentResult != nullptr )
		{
			m_autocompleteText.clear();
			m_trie->get_from_prefix(m_currentResult->next(), m_autocompleteText);
		}
		else
		{
			m_autocompleteText.clear();
		}
	}

	void textbox::InternalDraw(const render::camera& camera)
	{		
		UNREFERENCED(camera);

		m_backgroundColor.A() = m_alpha;
		m_foregroundColor.A() = m_alpha;

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);

		// Draw the text
		std::wstring text = m_text.substr(m_scrollingIndex, m_text.size()-1);
		m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), m_textPosition, m_foregroundColor);
		
		if ( m_autocompleteText.size() != 0 )
		{
			math::vector2 autoCompletePosition = m_textPosition;
			math::vector2 textExtents = static_cast<math::vector2>( m_font->MeasureString(m_text.c_str()) );
			autoCompletePosition.x() += textExtents.x();
			m_font->DrawString(m_spriteBatch.get(), m_autocompleteText.c_str(), autoCompletePosition, render::color::CYAN);
		}

		m_caret.Draw(m_core->GetWhiteTexture(), m_spriteBatch);
	}

}