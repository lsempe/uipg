#pragma once

#include "engine.ui/element.h"
#include "engine.render/spritesheet.h"
#include "engine.math/ellipse.h"

namespace game
{
	class player;
	class world;
}

namespace ui
{

class minimap_view : public view
{
public:

	minimap_view(element& element, const math::rectangle& rectangle) 
		: view(element)
		, m_rectangle(rectangle)
	{
	}

	virtual void Create();	
	virtual void Draw();
	virtual void Update(float /*deltaTime*/) {}

private:

	std::unique_ptr<render::spritesheet> m_iconSpriteSheet;
	std::shared_ptr<render::sprite> m_iconSprite;

	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;
	std::unique_ptr<render::texture> m_background;
	std::unique_ptr<render::texture> m_mask;

	size_t m_animations[4];

	math::ellipse m_ellipse;
	math::rectangle m_rectangle;

	std::list<math::vector3> m_entities;
};

class minimap_controller: public controller
{
public:

	minimap_controller(game::player&, element& element)
		: controller(element)
	{		
	}

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

};

class minimap_element : public element
{
public:
	minimap_element(game::player& player, game::world& world)
		: element()
		, m_player(player)
		, m_world(world)
		, m_zoom(1.f)
	{
		m_view = std::unique_ptr<minimap_view>( new minimap_view(*this, math::rectangle(10.f, 10.f, 256.f, 256.f)) );
		m_controller = std::unique_ptr<minimap_controller>( new minimap_controller(player, *this) );
	}

	virtual ~minimap_element() {}

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

	//game::world& World() const { return m_world; }
	game::world& World() const { return m_world; }

	game::player& Player() { return m_player; }

	void IncreaseZoom() { m_zoom += 0.1f; if (m_zoom > 6.f) m_zoom = 6.f; }
	void DecreaseZoom() { m_zoom -= 0.1f; if (m_zoom < 2.f) m_zoom = 2.f; }
	float Zoom() const { return m_zoom; }

protected:

	float m_zoom;

	game::player& m_player;
	game::world& m_world;

};


} // ui