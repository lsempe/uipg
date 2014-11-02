#pragma once

#include "engine.math/ellipse.h"
#include "engine.math/point.h"
#include "engine.render/quad.h"
#include "engine.render/texture.h"
#include "engine.ui/element.h"
#include "engine.core/game.h"

#include "SpriteBatch.h"

#include <list>

namespace ui
{

class view_damage_indicator : public view 
{
public:
	view_damage_indicator(element& element)
		: view(element)
	{}

	virtual ~view_damage_indicator() {}

	virtual void Create() {}
	virtual void Draw() {}
	virtual void Update(float deltaTime) 
	{
		UNREFERENCED(deltaTime);
	}
};

class controller_damage_indicator : public controller
{
public:

	controller_damage_indicator(element& element)
		: controller(element)
	{
	}
};

class element_damage_indicator : public element
{

public:
	element_damage_indicator()
		: element()
		, m_ellipse()
		, m_spriteBatch()
	{
		m_view = std::unique_ptr<view_damage_indicator>( new view_damage_indicator(*this) );
		m_controller = std::unique_ptr<controller_damage_indicator>( new controller_damage_indicator(*this) );
		
		m_transform = math::matrix::CreateRotationX(math::HalfPi);
	}
	
	struct DamageIndicator
	{
		math::vector3 direction;
		float duration;
		float angle;
		render::color color;
	};

	std::list<DamageIndicator> m_indicators;

	void DamageEvent(const math::vector3& direction)
	{
		DamageIndicator indicator;
		indicator.direction = direction * m_transform;
		indicator.duration = 0.5f;
		indicator.color = render::color::RED;

		indicator.angle = atan2(indicator.direction.y(), indicator.direction.x()) + 3*(math::Pi/4);
		m_indicators.push_back(indicator);
	}

	virtual void Create(std::shared_ptr<core_ui> core)
	{
		element::Create(core);
		m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(m_core->GetDevice()->GetImmediateContext() ) );

		auto viewport = m_core->GetDevice()->GetViewport();
		m_ellipse = math::ellipse(math::point::Zero, math::vector2(viewport.Width()/6, viewport.Height()/4));

		m_texture = std::unique_ptr<render::texture>(new render::texture(m_core->GetDevice(), game::GetContentPath(TEXT("\\content\\sprites\\damageindicator3.dds"))));

	}

	virtual void Update(float deltaTime)
	{
		auto it = std::begin(m_indicators);
		while (it != std::end(m_indicators)) 
		{
			it->duration -= deltaTime;
		    if (it->duration <= 0.f)
				it = m_indicators.erase(it);
			else
				++it;
		}
	}
	
	virtual void Draw()
	{		
		auto device = m_core->GetDevice();
		auto viewport = device->GetViewport();

		DirectX::CommonStates states(device->GetDevice());

		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.Additive());

		for (auto& it : m_indicators)
		{
			const math::vector2& direction = it.direction;
			math::vector2 p = m_ellipse.GetPointOnEllipse(direction);
			auto screenPoint = math::vector3(p.x()+ viewport.Width()/2, p.y()+ viewport.Height()/2, 1.f);
			m_spriteBatch->Draw(*m_texture->GetView(), math::vector2(screenPoint.x(), screenPoint.y()), nullptr, it.color, it.angle, math::vector2(m_texture->Width()/2, m_texture->Height()/2));
		}

		m_spriteBatch->End();		
	}

	math::matrix m_transform;
	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;
	math::ellipse m_ellipse;
	std::unique_ptr<render::texture> m_texture;
};

} // ui