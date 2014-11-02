#pragma once

#include <memory>
#include "engine.render/camera.h"
#include "engine.ui/core.h"
#include "minimap.h"
#include "positional.h"

namespace game
{
	class player;
	class world;
}

namespace ui
{
	class font_library;
	class label;
}

class hud
{
public:

	hud(std::shared_ptr<ui::core_ui> core, game::player& player, game::world& world);

	virtual void Update(float deltaTime);
	virtual void Draw(render::camera& camera);

	virtual bool HandleInput(float deltaTime, input::input_state& inputState);

protected:

	game::player& m_player;
	game::world& m_world;

	std::unique_ptr<ui::minimap_element> m_minimap;
	std::unique_ptr<ui::positional_element> m_positional;

	std::unique_ptr<ui::label> m_orientation;

	std::unique_ptr<ui::font_library> m_fontLibrary;

	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;

	std::shared_ptr<ui::core_ui> m_core;

};