#include "font_library.h"

namespace ui
{

	font_library::font_library(render::device_direct3d* device)
		: m_device(device)
	{}
	
	void font_library::Load(const std::wstring& fontName, const std::wstring& fontPath)
	{
		FontMap::const_iterator it = m_fontMap.find(fontName);
		if ( it == m_fontMap.end() )
		{
			m_fontMap.emplace(fontName, std::shared_ptr<render::platform::font>(new render::platform::font(m_device->GetDevice(), fontPath.c_str())));
		}
	}

	std::shared_ptr<render::platform::font> font_library::Get(const std::wstring& name) const
	{
		FontMap::const_iterator it = m_fontMap.find(name);
		if ( it != m_fontMap.end() )
		{
			return it->second;
		}

		return nullptr;
	}

} // ui