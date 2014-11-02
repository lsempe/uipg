#ifndef _SAMPLES_BASE_ENGINE_FRAMEWORK_H_
#define _SAMPLES_BASE_ENGINE_FRAMEWORK_H_

#pragma once

#include <chrono>

#include "engine.core/game.h"
#include "engine.render/device_direct3d.h"
#include "engine.ui/font_library.h"

namespace samples
{
	class framework : public game::core
	{
	public:
		framework(HWND& windowHandle, int clientWidth, int clientHeight);
		virtual ~framework();

		virtual void Initialize();
		virtual void Shutdown();

		virtual void Update();
		virtual void Update(float delta);

		virtual void PreRender();
		virtual void Present();

		std::unique_ptr<ui::font_library>& FontLibrary() { return m_fontLibrary; }

	protected:

		render::device_direct3d m_device;		
		std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTimePoint;

		std::unique_ptr<ui::font_library> m_fontLibrary;
	};
}

#endif // _SAMPLES_BASE_ENGINE_FRAMEWORK_H_