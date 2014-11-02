#ifndef _SAMPLES_CONTROLS_H_
#define _SAMPLES_CONTROLS_H_

#include "samples/sample_framework.h"

#include "engine.ui/container.h"
#include "engine.ui/input_cursor.h"

#include "world.h"
#include "player.h"

#pragma once

namespace ui
{
	class core_ui;
	class input_cursor;
}

namespace input
{
	class input_state;
}

namespace samples
{

	class hudsample : public framework
	{
	public:

		hudsample(HWND& windowHandle, int clientWidth, int clientHeight)
			: framework(windowHandle, clientWidth, clientHeight)
		{}

		virtual ~hudsample() {}

		virtual void Initialize();

		virtual void Update();
		virtual void Update(float delta) override;
		virtual void Render();

		std::shared_ptr<ui::core_ui> m_ui;
		
		std::shared_ptr<input::input_state> m_inputState;
		std::shared_ptr<input::mouse> m_mouse;

		std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;
		std::shared_ptr<render::platform::font> m_spriteFont;

		std::unique_ptr<ui::container> m_container;

		std::unique_ptr<ui::input_cursor> m_cursor;

		std::unique_ptr<game::world> m_world;
		std::shared_ptr<game::player> m_player;

		std::shared_ptr<ui::label> m_mouseCoordinates;
		
	};

}

#endif // _SAMPLES_CONTROLS_H_