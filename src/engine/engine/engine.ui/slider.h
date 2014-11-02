#pragma once

#include "control.h"
#include "scrollbar.h"

namespace render
{
	class spritesheet;
	class texture;
}

namespace ui
{

class slider : public scrollbar
{
public:
	
	slider(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch);

	/*virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
	{
		return scrollbar::HandleInput(deltaTime, inputState);
	}*/

	int& Minimum() { return m_minimum; }
	int& Maximum() { return m_maximum; }
	int Value() const { return m_minimum + static_cast<int>(Ratio() * m_maximum); }

	void SetCursor(const std::wstring texturePath);

protected:

	virtual void Update(float deltaTime);

	virtual void InternalDraw(const render::camera& camera);

	int m_minimum;
	int m_maximum;

	std::shared_ptr<render::texture> m_cursorTex;
	//render::spritesheet
	std::unique_ptr<render::spritesheet> m_cursorSprite;

};


} // ui