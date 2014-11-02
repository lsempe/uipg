#pragma once

#include "engine.ui/element.h"
#include "player.h"
#include "world.h"

#include "engine.render/spritesheet.h"

#include "SpriteBatch.h"

namespace ui
{

	


class View_Compass : public view
{
public:
	View_Compass(element& element) 
		: view(element) 
		, m_background(nullptr)
		, m_iconSprites(nullptr) 
		, m_spriteBatch(nullptr)
	{}
	
	virtual void Create();	
	virtual void Draw();
	virtual void Update(float deltaTime);

private:

	std::unique_ptr<render::texture> m_background;
	std::unique_ptr<render::spritesheet> m_iconSprites;

	//std::vector<render::spritesheet::animation> m_icons;

	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;

	math::rectangle m_rectangle;
	math::rectangle m_clipping;
};

class Controller_Compass : public controller
{
public:
	
	Controller_Compass(game::Player& player, element& element)
		: controller(element)
	{
		UNREFERENCED(player);
	}

	virtual void Update(float deltaTime)
	{
		UNREFERENCED(deltaTime);
	}


};

class element_compass : public element
{
public:
	element_compass(game::Player& player, const game::World& world, math::vector3 north = math::vector3(0,0,-1))
		: element()
		, m_player(player)
		, m_world(world)
		, m_north(north)
		, m_offsetX(0.f)
	{
		m_view = std::unique_ptr<View_Compass>( new View_Compass(*this) );
		m_controller = std::unique_ptr<Controller_Compass>( new Controller_Compass(player, *this) );
	}

	virtual void Create(std::shared_ptr<core_ui> core);
	
	virtual void Update(float deltaTime);
	virtual void Draw();

	element_compass& Get() { return *this; }
	const game::World& World() const { return m_world; }

	struct item
	{
		math::vector3 location;
		std::wstring name;
		render::color color;
	};

	std::vector<item> m_items;

	void AddItem(const wchar_t* name, const math::vector3& location)
	{
		item i;
		i.name = name;
		i.location = location;
		m_items.push_back(i);
	}

	const std::vector<item>& Items() const { return m_items; }

	game::Player& Player() { return m_player; }

	const math::vector3& North() const { return m_north; }

protected:

	game::Player& m_player;
	const game::World& m_world;

	math::vector3 m_north;
	float m_offsetX;
};


} // ui