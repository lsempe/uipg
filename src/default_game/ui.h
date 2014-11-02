#include "engine.ui/helpers.h"
#include "engine.render/texture.h"
#include "engine.render/texture_black.h"

namespace ui
{

class fixed_textures
{
public:
	fixed_textures(render::device* device)
		: m_blackTexture(nullptr)
	{
		m_blackTexture = std::shared_ptr<render::texture>(new render::texture(static_cast<render::device_direct3d*>(device)->GetDevice(), render::textures::black::data, render::textures::black::size));
	}

	~fixed_textures()
	{
		
	}

	std::shared_ptr<render::texture> m_blackTexture;
	std::shared_ptr<render::texture> GetBlackTexture() const { return m_blackTexture; }
};

}