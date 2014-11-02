#pragma once

#include "control.h"
#include "SpriteFont.h"
#include "helpers.h"
#include "core.h"
#include "caret.h"
#include "engine.core/trie.h"
#include "engine.core/event_handler.h"

namespace ui
{

class textbox : public control
{
public:

	textbox(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch);

	virtual void Refresh();

	virtual void Update(float deltaTime);
	
	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

	void AutoComplete(bool reset = false);

	virtual void InternalDraw(const render::camera& camera);

	const std::wstring& Text() const { return m_text; }

	void SetAutoCompleteSource(std::shared_ptr<trie> trie) { m_trie = trie; }

	event_handler<const std::wstring&>& OnTextEntered() { return m_onTextEntered; }

	void SetText(const std::wstring& text) { m_text = text; }

	void SupportAutoComplete(bool enable) { m_autoCompleteSupported = enable; }

	static const unsigned int kTabSpaces;

protected:

	std::wstring m_text;
	std::wstring m_autocompleteText;
	bool m_autoCompleteSupported;

	caret m_caret;

	math::vector2 m_textPosition;

	wchar_t m_currentKey;
	wchar_t m_lastKey;

	float m_inputSpeed;

	float s_inputThrottleLength;
	float m_inputThrottle;
	float m_lastInputTime;
	
	float m_keyStrokeTimer;

	float m_keyPressDuration;

	bool m_longPress;
	bool m_repeat;

	int m_scrollingIndex;

	event_handler<const std::wstring&> m_onTextEntered; 

	std::shared_ptr<trie> m_trie;
	std::shared_ptr<trie::node> m_currentResult;
};

//float textbox::s_inputThrottleLength = 0.5f;


} // ui