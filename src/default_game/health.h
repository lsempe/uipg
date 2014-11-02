#pragma once

#include "engine.render/texture.h"

#include "player.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "GeometricPrimitive.h"
#include "VertexTypes.h"

#include "engine.ui/label.h"
#include "engine.ui/progressbar.h"
#include "engine.ui/button.h"
#include "engine.ui/console.h"
#include "engine.ui/listbox.h"

#include "engine.render/spritesheet.h"

#include "engine.ui/element.h"

namespace ui
{

class View_Health : public view
{
public:

	View_Health(element& element);
	virtual ~View_Health()
	{
		m_testPrim = nullptr;
	}
	
	virtual void Create();	
	virtual void Draw();
	virtual void Update(float deltaTime);
	
protected:

	std::unique_ptr<ui::label> m_label;
	std::unique_ptr<ui::progress_bar> m_progressbar;
	
	// tests
	std::unique_ptr<render::spritesheet> m_spritesheet;
	std::unique_ptr<ui::button> m_button;
	
	std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;
	std::shared_ptr<render::platform::font> m_spriteFont;

	std::unique_ptr<DirectX::GeometricPrimitive> m_testPrim;

	std::unique_ptr<ui::listbox> m_listbox;


	
};

class Controller_Health : public controller
{
public:
	Controller_Health(game::Player& player, element& element)
		: controller(element)
	{
		UNREFERENCED(player);
	}

	virtual void Update(float deltaTime)
	{
		UNREFERENCED(deltaTime);
	}

protected:

};

class element_health : public element
{
public:
	element_health(game::Player& player)
		: element()
		, m_player(player)
	{
		m_view = std::unique_ptr<View_Health>( new View_Health(*this) );
		m_controller = std::unique_ptr<Controller_Health>( new Controller_Health(player, *this) );
	}

	float GetHealth() const { return m_player.GetHealth(); }
	float GetMaxHealth() const { return m_player.GetMaxHealth(); }

	virtual void Update(float deltaTime);
	virtual void Draw();

	element_health& Get() { return *this; }

	game::Player& Player() { return m_player; }

protected:

	game::Player& m_player;

	
};


} // ui