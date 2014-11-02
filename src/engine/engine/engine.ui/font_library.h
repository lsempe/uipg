#pragma once

#include <map>
#include <memory>
#include <string>

#include "SpriteFont.h"
#include "engine.render/platform.h"
#include "engine.render/device_direct3d.h"

namespace ui
{

	class font_library
	{
	public:
		font_library(render::device_direct3d* device);
	
		void Load(const std::wstring& fontName, const std::wstring& fontPath);	
		std::shared_ptr<render::platform::font> Get(const std::wstring& name) const;

	private:
	
		typedef std::map<std::wstring, std::shared_ptr<render::platform::font>> FontMap ;
		FontMap m_fontMap;
		render::device_direct3d* m_device;
	};

} // ui