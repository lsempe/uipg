#pragma once

#include "engine.core/game.h"
#include "engine.math/matrix.h"
#include "engine.render/camera.h"

namespace game
{

class Player
{
public:

	Player(game::core* game)
		: m_game(game)
		, m_health(100)
		, m_maxHealth(100)
		, m_position(math::vector3::Zero)
	{
		m_camera = render::camera(math::Pi/4.f, static_cast<float>(game->ClientWidth())/static_cast<float>(game->ClientHeight()));
	}

	float GetHealth() const { return m_health; }
	float GetMaxHealth() const { return m_maxHealth; }

	void Rotate(float y)
	{
		static int axis = 0;
		math::matrix m = math::matrix::CreateRotationY(y);
		if ( axis == 1 ) 
			m = math::matrix::CreateRotationX(y);
		else
		if ( axis == 2 ) 
			m = math::matrix::CreateRotationZ(y);

		m_camera.Rotate(m);
	}

	virtual void Update(float deltaTime)
	{
		m_camera.Update(deltaTime);
		//m_camera.SetPosition(m_position);
	}

	void SetCamera(const render::camera& camera)
	{
		m_camera = camera;
	}

	render::camera& Camera() { return m_camera; }
	const math::vector3& Position() const { return m_camera.Position(); } // TODO: decouple from camera

protected:

	float m_health;
	float m_maxHealth;

	math::vector3 m_position;

	render::camera m_camera;

	game::core* m_game;

};

} // game