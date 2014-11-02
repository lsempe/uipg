#include "camera.h"
#include "engine.render/viewport.h"

namespace render
{

camera::camera()
	: m_view(math::matrix::Identity)
	, m_projection(math::matrix::Identity)
	, m_world(math::matrix::Identity)
	, m_near(0.1f)
	, m_far(1000.f)
	, m_fovY(math::Pi/4)
	, m_aspectRatio(1.77777f)
	, m_position(math::vector3::Zero)
	, m_rotation(math::quaternion::Identity)
{
	m_projection = math::lefthanded::PerspectiveFOV(m_fovY, m_aspectRatio, m_near, m_far);	
}

camera::camera(const render::viewport& viewport, float nearPlane/*=0.1f*/, float farPlane/* = 1000.f*/)
	: m_view(math::matrix::Identity)
	, m_world(math::matrix::Identity)
	, m_near(nearPlane)
	, m_far(farPlane)
	, m_fovY(math::Pi/4)
	, m_aspectRatio(viewport.Aspect())
	, m_position(math::vector3::Zero)
	, m_rotation(math::quaternion::Identity)
{
	m_projection = math::lefthanded::PerspectiveFOV(m_fovY, viewport.Aspect(), m_near, m_far);		
}

camera::camera(float fovy, float aspectRatio, float nearPlane/* = 0.1f*/, float farPlane /*= 1000.f*/)
	: m_view(math::matrix::Identity)
	, m_world(math::matrix::Identity)
	, m_near(nearPlane)
	, m_far(farPlane)
	, m_fovY(fovy)
	, m_aspectRatio(aspectRatio)
	, m_position(math::vector3::Zero)
	, m_rotation(math::quaternion::Identity)
{
	m_projection = math::lefthanded::PerspectiveFOV(fovy, aspectRatio, m_near, m_far);		
}

camera::~camera()
{
}

void camera::SetProjection(const math::matrix& projection)
{
	m_projection = projection;
}

void camera::SetPerspectiveProjection(float yfov, float aspectRatio, float nearPlane, float farPlane)
{
	m_near = nearPlane;
	m_far = farPlane;
	m_projection = math::lefthanded::PerspectiveFOV(yfov, aspectRatio, nearPlane, farPlane);
}

void camera::LookAt(const math::vector3& from, const math::vector3& to, const math::vector3& up)
{
	m_position = from;
	m_view = math::lefthanded::LookAt(from, to, up);
	m_world = math::matrix::Invert(m_view);
	m_rotation = math::quaternion::CreateFromRotationMatrix(m_world);
}

void camera::SetPosition(const math::vector3& position)
{
	m_position = position;
	m_world = math::matrix::CreateTranslation(position) * m_world;
	m_view = math::matrix::Invert(m_world);

}

} // render