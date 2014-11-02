#pragma once

#include "engine.ui/element.h"
#include "engine.render/spritesheet.h"
#include "engine.math/ellipse.h"

#include "PrimitiveBatch.h"
#include "VertexTypes.h"

namespace game
{
	class player;
	class world;
}

namespace ui
{

class positional_view : public view
{
public:

	positional_view(element& element, const math::rectangle& rectangle) 
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
	std::shared_ptr<render::platform::font> m_font;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

	size_t m_animations[4];

	math::ellipse m_ellipse;
	math::rectangle m_rectangle;

	std::list<math::vector3> m_entities;

};

class positional_controller: public controller
{
public:

	positional_controller(game::player&, element& element)
		: controller(element)
	{		
	}

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

};

class positional_element : public element
{
public:
	positional_element(game::player& player, game::world& world)
		: element()
		, m_player(player)
		, m_world(world)
		, m_zoom(1.f)
	{
		m_view = std::unique_ptr<positional_view>( new positional_view(*this, math::rectangle(10.f, 10.f, 256.f, 256.f)) );
		m_controller = std::unique_ptr<positional_controller>( new positional_controller(player, *this) );
	}

	virtual ~positional_element() {}

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

	game::world& World() const { return m_world; }
	game::player& Player() { return m_player; }

	void IncreaseZoom() { m_zoom += 0.1f; }
	void DecreaseZoom() { m_zoom -= 0.1f; }
	float Zoom() const { return m_zoom; }

protected:

	float m_zoom;

	game::player& m_player;
	game::world& m_world;

};


} // ui