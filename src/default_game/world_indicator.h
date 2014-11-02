#pragma once

#include "SpriteBatch.h"
#include "engine.ui/element.h"
#include "engine.math/ellipse.h"
#include "engine.core/game.h"
#include "engine.render/color.h"

#include <list>

namespace ui
{

class view_world_indicator : public view
{
public:

	view_world_indicator(element& element)
		: view(element)
		, m_spriteBatch(nullptr)
		, m_texture(nullptr)
	{

	}

	virtual void Create()
	{		
		auto device = m_element.GetCore()->GetDevice();
		m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext() ) );

		auto viewport = device->GetViewport();
		m_ellipse = math::ellipse(math::point::Zero, math::vector2(viewport.Width()/6, viewport.Height()/4));

		m_texture = std::unique_ptr<render::texture>(new render::texture(device, game::GetContentPath(TEXT("\\content\\sprites\\damageindicator3.dds"))));
	}

	virtual void Draw();

protected:

	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;
	math::ellipse m_ellipse;
	std::unique_ptr<render::texture> m_texture;

};


class controller_world_indicator : public controller
{
public:
	controller_world_indicator(element& element)
		: controller(element)
	{

	}
};


class element_world_indicator : public element
{
public:

	element_world_indicator()
		: element()
	{
		m_view = std::unique_ptr<view_world_indicator>( new view_world_indicator(*this) );
		m_controller = std::unique_ptr<controller_world_indicator>( new controller_world_indicator(*this) );
	}

	struct indicator
	{
		math::vector3 position;
		render::color color;
	};

	std::list<indicator>::const_reference Add(const math::vector3& position, render::color color = render::color::WHITE)
	{
		indicator i = { position, color };
		m_indicators.emplace_back(i);
		return m_indicators.back();
	}

	void Remove(std::list<indicator>::const_iterator it)
	{
		m_indicators.erase(it);
	}

	const std::list<indicator>& Indicators() const { return m_indicators; }

protected:

	std::list<indicator> m_indicators;

};

} // ui