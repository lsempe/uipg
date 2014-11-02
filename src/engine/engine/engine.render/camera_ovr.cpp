#include "camera_ovr.h"
#include "engine.core/runtime_variable.h"

namespace render
{

	runtime_variable_float ovr_horizontal_screen_size(TEXT("ovr_hscreen_size"), TEXT("Physical dimensions of the entire HMD screen in meters. The current physical screen size is 149.76 x 93.6mm, which will be reported as 0.14976f x 0.0935f."), 0.14976f, 0.f, FLT_MAX);
	runtime_variable_float ovr_vertical_screen_size(TEXT("ovr_vscreen_size"), TEXT("Physical dimensions of the entire HMD screen in meters. The current physical screen size is 149.76 x 93.6mm, which will be reported as 0.14976f x 0.0935f."), 0.936f, 0.f, FLT_MAX);

	runtime_variable_float ovr_eye_screen_distance(TEXT("ovr_eye_screen_distance"), TEXT("Distance from the eye to the screen, in meters. This combines distances from the eye to the lens, and from the lens to the screen. This value is needed to compute the fov-y angle correctly."), 0.041f, 0.f, FLT_MAX);
	runtime_variable_float ovr_interpupillary_distance(TEXT("ovr_interpupillary_distance"), TEXT("Distance from one pupil to another."), 0.064f, 0.f, FLT_MAX);

	camera_ovr::camera_ovr()
	: camera()
	{
	}

	camera_ovr::camera_ovr(const render::viewport& viewport, float nearPlane/*=0.1f*/, float farPlane/* = 1000.f*/)
		: camera(viewport, nearPlane, farPlane)
	{
	
		const float hScreenSize = ovr_horizontal_screen_size;
		const float vScreenSize = ovr_vertical_screen_size;

		const float eyeToScreenDistance = ovr_eye_screen_distance;
		const float interpupillaryDistance = ovr_interpupillary_distance;

		float aspectRatio = (viewport.Width() * 0.5f) / viewport.Height();
		float halfScreenDistance = vScreenSize / 2;
		float yfov = 2.f * atan(halfScreenDistance / eyeToScreenDistance);

		float viewCenter = hScreenSize * 0.25f;
		float eyeProjectionShift = viewCenter - interpupillaryDistance*0.5f;
		float projectionCenterOffset = 4.f * eyeProjectionShift / hScreenSize;

		math::matrix projectionCenter = math::matrix( DirectX::XMMatrixPerspectiveFovLH(yfov, aspectRatio, 0.1f, 1000.f) );
		m_projectionOVR[Left]  = math::matrix::CreateTranslation(projectionCenterOffset, 0.f, 0.f) * projectionCenter;
		m_projectionOVR[Right] = math::matrix::CreateTranslation(-projectionCenterOffset, 0.f, 0.f) * projectionCenter;

		float halfIPD = interpupillaryDistance * 0.5f;
		m_viewOVR[Left] = math::matrix::CreateTranslation(halfIPD, 0.f, 0.f) * m_world * viewCenter;
		m_viewOVR[Right] = math::matrix::CreateTranslation(-halfIPD, 0.f, 0.f) * m_world * viewCenter;

		m_viewport[Left] = render::viewport(0.f, viewport.Top(), viewport.Center().x(), viewport.Bottom(), nearPlane, farPlane);
		m_viewport[Right] = render::viewport(viewport.Center().x(), viewport.Top(), viewport.Right(), viewport.Bottom(), nearPlane, farPlane);
	}

	void camera_ovr::LookAt(const math::vector3& from, const math::vector3& to, const math::vector3& up)
	{
	

		const float interpupillaryDistance = ovr_interpupillary_distance;
		const float hScreenSize = ovr_horizontal_screen_size;
		float viewCenter = hScreenSize * 0.25f;
		float halfIPD = interpupillaryDistance * 0.5f;
	
		m_viewOVR[Left] = math::matrix::CreateTranslation(halfIPD, 0.f, 0.f) * viewCenter;
		m_viewOVR[Right] = math::matrix::CreateTranslation(-halfIPD, 0.f, 0.f) * viewCenter;
	
		m_viewOVR[Left] =  m_viewOVR[Left] * m_view;

		camera::LookAt(from, to, up);

		m_viewOVR[Left] = m_view;
		m_viewOVR[Right] = m_view;

		math::vector3 offsetLeft = math::vector3(halfIPD, 0.f, 0.f) * viewCenter;
		math::vector3 offsetRight = math::vector3(-halfIPD, 0.f, 0.f) * viewCenter;
		m_viewOVR[Left].Translate(m_view.Translation() + offsetLeft );
		m_viewOVR[Right].Translate( m_view.Translation() + offsetRight);

	}

} // render