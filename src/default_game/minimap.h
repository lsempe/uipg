#pragma once

#include "engine.ui/element.h"
#include "player.h"
#include "world.h"

#include "engine.render/spritesheet.h"

#include "engine.math/ellipse.h"

namespace ui
{

class view_minimap : public view
{
public:

	view_minimap(element& element, const math::rectangle& rectangle) 
		: view(element)
		, m_rectangle(rectangle)
	{
	}

	virtual void Create();	
	virtual void Draw();
	virtual void Update(float /*deltaTime*/) {}

private:

	std::unique_ptr<render::spritesheet> m_iconSpriteSheet;
	std::shared_ptr<render::spritesheet::sprite> m_iconSprite;

	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;
	std::unique_ptr<render::texture> m_background;

	size_t m_animations[4];

	math::ellipse m_ellipse;
	math::rectangle m_rectangle;

	std::list<math::vector3> entities;
};

class controller_minimap : public controller
{
public:

	controller_minimap(game::Player&, element& element)
		: controller(element)
	{		
	}

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

};

class element_minimap : public element
{
public:
	element_minimap(game::Player& player, game::World& world)
		: element()
		, m_player(player)
		, m_world(world)
		, m_zoom(1.f)
	{
		m_view = std::unique_ptr<view_minimap>( new view_minimap(*this, math::rectangle(10.f, 10.f, 256.f, 256.f)) );
		m_controller = std::unique_ptr<controller_minimap>( new controller_minimap(player, *this) );
	}

	virtual ~element_minimap() {}

	virtual void Create(std::shared_ptr<core_ui> core)
	{
		element::Create(core);
	}
	
	virtual void Update(float deltaTime) { element::Update(deltaTime); }
	virtual void Draw() { element::Draw(); }
	virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
	{
		return m_controller->HandleInput(deltaTime, inputState);
	}

	game::World& World() { return m_world; }
	const game::World& World() const { return m_world; }

	game::Player& Player() { return m_player; }

	void IncreaseZoom() { m_zoom += 0.1f; }
	void DecreaseZoom() { m_zoom -= 0.1f; }
	float Zoom() const { return m_zoom; }

protected:

	float m_zoom;

	game::Player& m_player;
	game::World& m_world;

};


} // ui