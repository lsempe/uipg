#pragma once

#include "engine.render/device_direct3d.h"
#include "engine.render/quad.h"
#include "engine.render/camera.h"
#include "engine.render/texture.h"

namespace ui
{

class analog
{
public:

	analog(render::device_direct3d* device, std::shared_ptr<render::texture> texture)
		: m_quad(device)
		, m_device(device)
		, m_texture(texture)
		, m_ratio(math::TwoPi)
		, m_minimum(math::Pi/2)
		, m_range(math::TwoPi)
	{
		m_quad.ViewAlign() = false;
		m_quad.Transform() = math::matrix::CreateRotationZ(math::Pi);
		m_quad.Pivot() = math::vector3(0.f, 0.25f, 0.f);
	}

	virtual void Update(float deltaTime)
	{
		UNREFERENCED(deltaTime);
	}
	
	void Draw(const render::camera& camera)
	{
		float angle = m_minimum - (m_ratio * m_range );
		math::matrix rotation = math::matrix::CreateRotationZ( angle );

		math::vector3 worldPosition = m_device->GetViewport().Unproject( math::vector3(120.f, 360.f, 0.05f), camera.Projection(), camera.View(), math::matrix::Identity);

		auto transform = math::matrix::CreateViewAligned(camera.View(), worldPosition);
		transform = math::matrix::CreateUniformScale(0.01f) * rotation * transform;
						
		m_quad.Draw(transform, camera.View(), camera.Projection(), m_texture);
	}

	float& Ratio() { return m_ratio; }
	float& Minimum() { return m_minimum; }
	float& Range() { return m_range; }

	math::vector3& ScreenPosition() { return m_screenPosition; }

private:

	float m_ratio;
	float m_minimum;
	float m_range;

	math::vector3 m_screenPosition;
	math::vector3 m_position;
	//math::matrix m_transform;

	std::shared_ptr<render::texture> m_texture;
	render::quad m_quad;

	render::device_direct3d* m_device;
};


} // ui