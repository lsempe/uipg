#include "world_indicator.h"
#include "CommonStates.h"

namespace ui
{

void view_world_indicator::Draw()
{
	const element_world_indicator& element = static_cast<const element_world_indicator&>(m_element);

	auto device = element.GetCore()->GetDevice();
	const auto& viewport = device->GetViewport();

	DirectX::CommonStates states(device->GetDevice());
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	for ( auto& i : element.Indicators() )
	{
		//math::vector3 screenPosition = viewport.Project(i.position, device->GetProjectionMatrix(), device->GetViewMatrix(), math::matrix::Identity);

		math::vector3 screenPosition = viewport.Unproject(math::vector3(320.f, 360.f, 0.f), device->GetProjectionMatrix(), device->GetViewMatrix(), math::matrix::Identity);

		math::vector3 screenPosition2 = viewport.Unproject(math::vector3(320.f, 360.f, 1.f), device->GetProjectionMatrix(), device->GetViewMatrix(), math::matrix::Identity);

		//math::point p = m_ellipse.GetPointInEllipse(screenPosition);

		//auto screenPoint = math::vector3(p.x()+ viewport.Width()/2, p.y()+ viewport.Height()/2, 1.f);
		auto screenPoint = math::vector2(screenPosition.x(), screenPosition.y() );
		viewport.TitleSafeArea().Constrain(screenPoint, math::point((float)m_texture->Width(), (float)m_texture->Height()));

		m_spriteBatch->Draw(*m_texture->GetView(), math::vector2(screenPoint.x(), screenPoint.y()), nullptr, i.color);

	}

	m_spriteBatch->End();
}


} // ui