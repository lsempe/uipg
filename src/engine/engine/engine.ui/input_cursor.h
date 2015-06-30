#pragma once

#include "control.h"
#include "engine.render/spritesheet.h"

namespace ui
{

class input_cursor : public control
{
public:

	input_cursor(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font> font,  const std::shared_ptr<render::platform::sprite_batch>& spriteBatch, std::wstring path);
	
	virtual bool HandleInput(float deltaTime, input::input_state& input);
	virtual void Update(float deltaTime);
	virtual void Draw();

	std::shared_ptr<render::texture> Texture() const { return m_texture; }
	void SetTexture(std::shared_ptr<render::texture> texture);

	void Load(const std::wstring& path);

protected:

	std::shared_ptr<render::texture> m_texture;

	std::shared_ptr<render::sprite> m_sprite;

	std::unique_ptr<render::spritesheet> m_spriteSheet;

	size_t m_pressedAnimID;
	size_t m_defaultAnimID;
	size_t m_currentAnimID;

};


} // ui