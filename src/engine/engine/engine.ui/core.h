#pragma once

#include <memory>

#include "engine.core/input.h"

#include "engine.render/texture.h"
#include "engine.render/texture_black.h"
#include "engine.render/texture_white.h"

#include "engine.render/platform.h"
#include "engine.render/device_direct3d.h"

#include "font_library.h"

namespace ui
{

	class core_ui
	{
	public:

		core_ui(render::device_direct3d* device, const input::input_state& inputState);
		~core_ui();

		render::device_direct3d* GetDevice() const { return m_device; }

		std::shared_ptr<render::texture> GetBlackTexture() const { return m_blackTexture; }
		std::shared_ptr<render::texture> GetWhiteTexture() const { return m_whiteTexture; }

		const input::input_state& GetInputState() const { return m_inputState; }

		std::shared_ptr<font_library> FontLibrary() { return m_fontLibrary; }
	

	protected:

		render::device_direct3d* m_device;
		const input::input_state& m_inputState;

		std::shared_ptr<font_library> m_fontLibrary;

		std::shared_ptr<render::texture> m_whiteTexture;
		std::shared_ptr<render::texture> m_blackTexture;

		core_ui& operator = (core_ui&) { return *this; } // disallow assignment
	};


} //ui