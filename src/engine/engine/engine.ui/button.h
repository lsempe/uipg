#pragma once

#include "control.h"
#include "engine.render/spritesheet.h"

namespace render
{
	class spritesheet;
//	class spritesheet::sprite;
}

namespace ui
{

class button : public control
{
public:

	button(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch);
	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

	virtual void Update(float deltaTime);

	virtual void InternalDraw(const render::camera& camera);

	enum eState
	{
		Default,
		Pressed,
		Hover,
		Disabled,
		StateCount
	};

	eState m_state;

	void SetText(const std::wstring& text);

	virtual void Refresh();
	const std::wstring& Text() const { return m_text; }

	void Load(const std::wstring& path);
	
	bool& ShowText() { return m_showText; }

	event_handler<button&>& OnPressed() { return m_onPressed; }
	event_handler<button&>& OnReleased() { return m_onReleased; }

protected:

	event_handler<button&> m_onPressed;
	event_handler<button&> m_onReleased;

	std::wstring m_text;
	bool m_showText;

	std::unique_ptr<render::spritesheet> m_spriteSheet;
	std::shared_ptr<render::sprite> m_sprite;

	std::unique_ptr<render::spritesheet::animation> m_states[StateCount];
	
	std::shared_ptr<DirectX::SpriteFont> m_spriteFont;
};

}// ui