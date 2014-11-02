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

	
	/*void SetTexture(eState state, const std::wstring& path)
	{
		m_textures[state] = std::unique_ptr<render::texture>( new render::texture(m_core->GetDevice()->GetDevice(), path));
	}*/

	/*void SetStateAnimation(eState state, render::spritesheet::animation anim)
	{
		m_states[state] = anim;
	}*/

	void SetText(const std::wstring& text);

	virtual void Refresh();
	const std::wstring& Text() const { return m_text; }

	std::shared_ptr<render::sprite> m_sprite;

	void Load(const wchar_t* spritePath, int frameWidth, int frameHeight);



	void SetState(eState state);
	
	bool& ShowText() { return m_showText; }

	event_handler<button&>& OnPressed() { return m_onPressed; }
	event_handler<button&>& OnReleased() { return m_onReleased; }

protected:

	event_handler<button&> m_onPressed;
	event_handler<button&> m_onReleased;

	std::wstring m_text;
	bool m_showText;

	render::color m_colors[StateCount];

	std::unique_ptr<render::spritesheet> m_spriteSheet;
	std::unique_ptr<render::spritesheet::animation> m_states[StateCount];
	
	std::shared_ptr<DirectX::SpriteFont> m_spriteFont;
};

}// ui