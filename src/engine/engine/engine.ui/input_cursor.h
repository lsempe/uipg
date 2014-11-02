#pragma once

#include "control.h"

namespace ui
{

class input_cursor : public control
{
public:

	input_cursor(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font,  const std::shared_ptr<render::platform::sprite_batch>& spriteBatch, std::wstring path);

	virtual void Draw();

	std::shared_ptr<render::texture> Texture() const { return m_texture; }
	void SetTexture(std::shared_ptr<render::texture> texture);

protected:

	std::shared_ptr<render::texture> m_texture;

};


} // ui