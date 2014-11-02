#ifndef _SAMPLES_CONTROLS_H_
#define _SAMPLES_CONTROLS_H_

#include "samples/sample_framework.h"
#include "engine.ui/container.h"
#include "engine.ui/input_cursor.h"
#include "engine.render/camera.h"

#pragma once

namespace ui
{
	class core_ui;
	class label;
	class input_cursor;
	class progress_bar;
}

namespace input
{
	class input_state;
}


namespace samples
{

	class controls : public framework
	{
	public:

		controls(HWND& windowHandle, int clientWidth, int clientHeight)
			: framework(windowHandle, clientWidth, clientHeight)
		{}

		virtual ~controls() {}

		virtual void Initialize();

		virtual void Update();
		virtual void Update(float delta) override;
		virtual void Render();

		std::shared_ptr<ui::core_ui> m_core;
		render::camera m_camera;
		
		std::shared_ptr<input::input_state> m_inputState;
		std::shared_ptr<input::mouse> m_mouse;

		std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

		std::unique_ptr<ui::container> m_container;
		std::shared_ptr<ui::progress_bar> m_progressBar;
		std::shared_ptr<ui::label> m_mouseCoordinates;

		std::shared_ptr<ui::label> m_listBoxSelection;

		std::unique_ptr<ui::input_cursor> m_cursor;
	};

}

#endif // _SAMPLES_CONTROLS_H_