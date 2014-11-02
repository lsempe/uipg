#include "core.h"

namespace ui
{

	core_ui::core_ui(render::device_direct3d* device, const input::input_state& inputState)
		: m_device(device)
		, m_inputState(inputState)
	{
		m_blackTexture = std::shared_ptr<render::texture>(new render::texture(device, render::textures::black::data, render::textures::black::size));
		m_whiteTexture = std::shared_ptr<render::texture>(new render::texture(device, render::textures::white::data, render::textures::white::size));

		m_fontLibrary = std::shared_ptr<ui::font_library>(new ui::font_library(device));

	}

	core_ui::~core_ui()
	{
	}

} // ui