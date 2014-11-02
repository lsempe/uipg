#pragma once

#include "control.h"

namespace ui
{

class scrollbar : public control
{
public:

	
	enum eType
	{
		Horizontal,
		Vertical
	};

	scrollbar(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch);

	virtual bool HandleInput(float, const input::input_state& inputState);

	void ApplyMovement(float delta);

	virtual void Update(float);

	virtual void InternalDraw(const render::camera& camera);

	virtual void Refresh();

	float Ratio() const;

	eType& Type() { return m_type; }
	bool Visible() const { return m_visible; }
	bool& Visible() { return m_visible; }

protected:

	enum eState
	{
		Default,
		Pressed
	};

	math::rectangle m_cursor;
	eState m_state;
	eType m_type;
	float m_delta;
	int m_lastMouseWheelValue;
	bool m_visible;

};


} // ui