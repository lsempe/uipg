#pragma once

#include <list>
#include <sstream>
#include <string>

#include "core.h"
#include "engine.core/input.h"
#include "SpriteFont.h"
#include "engine.render/color.h"
#include "engine.math/rectangle.h"
#include "caret.h"
#include "engine.core/runtime_variable.h"

namespace render
{
	class camera;
}

namespace ui
{

	class listbox;
	class textbox;

	class console
	{
	public:
		console(std::shared_ptr<core_ui> core);
		~console();

		// navigation
		void ScrollUp();
		void ScrollDown();
		void ScrollTop();
		void ScrollBottom();

		void Update(float deltaTime);

		void Draw(const render::camera& camera);

		bool HandleInput(float deltaTime, const input::input_state& inputState);

		void AddItem(const std::wstring& text, const render::color& color);
	
		void Clear();

	private:

		class item
		{
		public:
			std::wstring line;
			render::color color;
		};

		// items
		std::list<item> m_items;
		std::list<item>::iterator m_currentItem;
		std::list<item>::iterator m_scrollPosition;

		// history
		std::list<std::wstring> m_history;
		std::list<std::wstring>::iterator m_currentHistoryItem;
		void AddHistory(const std::wstring& text);
		void NextHistoryItem();
		void PreviousHistoryItem();

		// drawing	
		void DrawItems();
		void ResetCaretBlink();
		void DrawInputCharacter(const char* input);

	
		void Execute(const std::wstring& text);

	
	
		bool m_visible;

		std::shared_ptr<render::platform::font> m_font;
		std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

	
		math::rectangle m_rectangle;
		render::color m_backgroundColor;
	
		std::unique_ptr<textbox> m_textBox;
		std::unique_ptr<listbox> m_listBox;

		std::shared_ptr<core_ui> m_core;

		runtime_variable_function m_clear;
		
		wchar_t m_lastKey;

		static float s_inputThrottleLength;
		float m_inputThrottle;
	};

	class console_parser
	{
	public:
		console_parser(const wchar_t* text)
			: m_source(text)
		{
			helper::stringutils::SplitW(text, ' ', m_split);
		}

		const std::wstring GetParametersFromIndex(size_t argIndex)
		{
			UNREFERENCED(argIndex);

			size_t index = m_source.find_first_of(L' ');
			if ( index != std::wstring::npos )
			{
				return m_source.c_str() + index + 1;
			}

			return std::wstring();
		}

		const std::wstring& GetString(size_t argIndex)
		{
			if ( m_split.size() > argIndex )
			{
				return m_split[argIndex];
			}

			throw new std::out_of_range("index argument is out of range");
		}
	
	private:

		std::wstring m_source;
		std::vector<std::wstring> m_split;

	};

} // ui