#pragma once

#include "control.h"
#include "engine.ui/control.h"
#include "engine.ui/helpers.h"
#include "engine.render/texture.h"

namespace ui
{

class progress_bar : public control
{
public:

	virtual void Update(float deltaTime)
	{
	}

	virtual void Draw()
	{
		RECT rc = helpers::MakeRect(	m_position.x, 
										m_position.y, 
										m_position.x + m_size.x, 
										m_position.y + m_size.y );
		m_backgroundColor.A() = m_alpha;
		m_foregroundColor.A()= m_alpha;


		m_spriteBatch->Draw(m_textures->GetBlackTexture()->GetView(), rc, nullptr, m_backgroundColor);

	}

protected:

	float m_maximum;
	float m_increment;

	std::unique_ptr<render::texture> m_texture;

};


} // ui