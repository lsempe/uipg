#include "player.h"

#include "engine.render/axes.h"

namespace game
{

player::player(game::core& gameCore, render::device_direct3d* device)
	: m_game(gameCore)	
	, entity(device, EntityType::Player)
	, m_angleY(0.f)
	, m_angleX(0.f)
{
	m_position = math::vector3(0.f, 2.f, 0.f);
	m_camera.LookAt(m_position, math::vector3::Zero, math::vector3::UnitY);	
}

void player::Update(float deltaTime)
{
	entity::Update(deltaTime);


	m_camera.Rotate(m_angleX, m_angleY, 0.f);
	m_camera.Update(deltaTime);
	m_hud->Update(deltaTime);
}

void player::Draw(render::camera& camera)
{
	//entity::Draw(camera);

	m_hud->Draw(camera);

	//render::axes a(m_device);
	//a.Draw(math::matrix::CreateFromQuaternion(m_rotation), m_camera.View(), m_camera.Projection());
}


player::~player()
{
}

bool player::HandleInput(float deltaTime, input::input_state& inputState)
{
	auto keyboard = inputState.GetKeyboard();
	if ( keyboard != nullptr )
	{
		bool fast = keyboard->KeyDown(DIK_LSHIFT);
		if ( keyboard->KeyDown(DIK_W) )
		{
			auto fwd = m_camera.Forward();
			//m_position = m_position + ( m_camera.Forward() * 0.1f);
			//m_camera.SetPosition(m_position);
			m_camera.MoveForward(fast ? 1.f : 0.1f);
			m_position = m_camera.Position();
			return true;
		}
		else
		if ( keyboard->KeyDown(DIK_S) )
		{
			//m_position = m_position - ( m_camera.Forward() * 0.1f);
			//m_camera.SetPosition(m_position);
			m_camera.MoveBack(fast ? 1.f : 0.1f);
			m_position = m_camera.Position();
			return true;
		}

		if ( keyboard->KeyDown(DIK_A) )
		{
			m_angleY += math::HalfPi / (fast ? 400.f : 600.f);
			m_rotation.Rotate(math::vector3::UnitY, m_angleY);
			if (m_angleY > math::TwoPi) m_angleY = 0;

			m_camera.Rotate(0.f, m_angleY, 0.f);
			return true;
		}
		else
		if ( keyboard->KeyDown(DIK_D) )
		{
			m_angleY -= math::HalfPi / (fast ? 400.f : 600.f);
			if (m_angleY < 0.f) m_angleY = math::TwoPi;

			m_rotation.Rotate(math::vector3::UnitY, m_angleY);
			m_camera.Rotate(0.f, m_angleY, 0.f);
			return true;
		}
		else
		if (keyboard->KeyDown(DIK_Q))
		{
			m_angleX -= math::HalfPi / (fast ? 400.f : 600.f);
			if (m_angleX < 0.f) m_angleX = math::TwoPi;

			m_rotation.Rotate(math::vector3::UnitX, m_angleX);
			m_camera.Rotate(m_angleX, m_angleY, 0.f);
			return true;
		}
		else
		if (keyboard->KeyDown(DIK_E))
		{
			m_angleX += math::HalfPi / (fast ? 400.f : 600.f);
			if (m_angleX < 0.f) m_angleX = math::TwoPi;

			m_rotation.Rotate(math::vector3::UnitX, m_angleX);
			m_camera.Rotate(m_angleX, m_angleY, 0.f);
			return true;
		}
	}

	if (m_hud->HandleInput(deltaTime, inputState))
	{
		return true;
	}

	return false;
}

}