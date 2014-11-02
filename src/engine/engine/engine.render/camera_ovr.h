#pragma once

#include "camera.h"

namespace render
{

	class camera_ovr : public camera
	{
	public:

		camera_ovr();
		camera_ovr(const render::viewport& viewport, float nearPlane = 0.1f, float farPlane = 1000.f);


		enum eSide
		{
			Left,
			Right,
			COUNT
		};

		const math::matrix& GetView(eSide side) const
		{
			return m_viewOVR[side];
		}

		const math::matrix& GetProjection(eSide side) const
		{
			return m_projectionOVR[side];
		}

		const render::viewport& GetViewport(eSide side) const
		{
			return m_viewport[side];
		}

		virtual void LookAt(const math::vector3& from, const math::vector3& to, const math::vector3& up);

	protected:

		math::matrix m_viewOVR[COUNT];
		math::matrix m_projectionOVR[COUNT];
		render::viewport m_viewport[COUNT];

	};

} // render