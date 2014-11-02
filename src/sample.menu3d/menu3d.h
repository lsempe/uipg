#ifndef _SAMPLES_MENU3D_H_
#define _SAMPLES_MENU3D_H_

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
	class menu_3d;
}

namespace input
{
	class input_state;
}


namespace samples
{

	class menu3d : public framework
	{
	public:

		menu3d(HWND& windowHandle, int clientWidth, int clientHeight);
		virtual ~menu3d();

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
		std::shared_ptr<ui::label> m_mouseCoordinates;

		std::unique_ptr<ui::menu_3d> m_menu3d;

		std::unique_ptr<ui::input_cursor> m_cursor;
	};

}

#endif // _SAMPLES_MENU3D_H_