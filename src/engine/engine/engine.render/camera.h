#pragma once

#include "engine.math/vector.h"
#include "engine.math/matrix.h"
#include "engine.render/viewport.h"
#include "engine.math/quaternion.h"

namespace render
{
	
class camera
{
public:
	camera();
	camera(const viewport& viewport, float nearPlane = 0.1f, float farPlane = 1000.f);
	camera(float fovy, float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.f);
	virtual ~camera();
	
	const math::vector3& Position() const
	{
		return m_position;
	}
	
	const math::vector3 Forward() const
	{
		return m_view.Forward();
	}

	math::vector3 Backward() const
	{
		return m_view.Backward();
	}

	math::vector3 Left() const
	{
		return m_view.Left();
	}

	math::vector3 Right() const
	{
		return m_view.Right();
	}

	math::vector3 Up() const
	{
		return m_view.Up();
	}

	math::vector3 Down() const
	{
		return m_view.Down();
	}

	const math::quaternion& Rotation() const { return m_rotation; }

	float Near() const { return m_near; }
	float Far() const { return m_far; }

	const math::matrix& Projection() const { return m_projection; }
	const math::matrix& View() const { return m_view; }
	const math::matrix& World() const { return m_world; }

	float VerticalFieldOfView() const
	{
		return m_fovY;
	}

	float HorizontalFieldOfView() const
	{
		return 2.f * atan(m_aspectRatio * tan(m_fovY * 0.5f) );
	}

	void SetView(const math::matrix& view)
	{
		m_view = view;
	}

	void SetWorld(const math::matrix& world)
	{
		m_world = world;
		m_view = math::matrix::Invert(m_world);
	}

	void Rotate(const math::matrix& rotation)
	{
		m_world = rotation * m_world;
		m_view = math::matrix::Invert(m_world);
	}

	void MoveForward(float distance)
	{
		m_position += m_view.Forward() * distance;
	}

	void MoveBack(float distance)
	{
		m_position -= m_view.Forward() * distance;
	}

	void MoveLeft(float distance)
	{
		m_position -= m_view.Right() * distance;
	}

	void MoveRight(float distance)
	{
		m_position += m_view.Right() * distance;
	}

	void MoveUp(float distance)
	{
		m_position -= m_view.Up() * distance;
	}

	void MoveDown(float distance)
	{
		m_position += m_view.Up() * distance;
	}

	const math::matrix InverseView() const
	{
		return math::matrix::Invert(m_view);
	}

	void Rotate(float pitch, float yaw, float roll)
	{
		math::quaternion q1 = math::quaternion::CreateFromAxisAngle(math::vector3::UnitY, yaw);
		math::quaternion q2 = math::quaternion::CreateFromAxisAngle(math::vector3::UnitX, pitch);
		math::quaternion q3 = math::quaternion::CreateFromAxisAngle(math::vector3::UnitZ, roll);

		m_rotation = q1 * q2 * q3;		

	}

	virtual void Update(float deltaTime)  
	{
		UNREFERENCED(deltaTime);

		m_world = math::matrix::CreateFromQuaternion(m_rotation) *  math::matrix::CreateTranslation(m_position);
		m_view = math::matrix::Invert(m_world);
	}

	void SetProjection(const math::matrix& projection);
	void SetPerspectiveProjection(float yfov, float aspectRatio, float nearPlane, float farPlane);
	virtual void LookAt(const math::vector3& from, const math::vector3& to, const math::vector3& up);
	void SetPosition(const math::vector3& position);

protected:

	math::matrix m_projection;
	math::matrix m_view;
	math::matrix m_world;

	math::quaternion m_rotation;
	math::vector3 m_position;

	float m_near;
	float m_far;
	float m_fovY;
	float m_aspectRatio;
};

} // render