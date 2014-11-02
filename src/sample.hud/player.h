#pragma once

#include <engine.render/camera.h>
#include <engine.core/game.h>
#include "entity.h"
#include "hud.h"

namespace game
{

class player : public entity
{
public:
	player(game::core&, render::device_direct3d* device);
	virtual ~player();

	render::camera& Camera() { return m_camera; }
	const render::camera& Camera() const { return m_camera; }

	virtual void Update(float deltaTime);
	virtual void Draw(render::camera& camera);

	void SetHud(std::shared_ptr<hud> hud) { m_hud = hud; }
	std::shared_ptr<hud> Hud() { return m_hud; }

	virtual bool HandleInput(float deltaTime, input::input_state& inputState);

	float Yaw() const { return m_angleY; }
	float Pitch() const { return m_angleX; }

protected:

	float m_health;
	float m_maxHealth;

	render::camera m_camera;

	float m_angleX;
	float m_angleY;

	game::core& m_game;
	std::shared_ptr<ui::core_ui> m_ui;

	std::shared_ptr<hud> m_hud;

};


}