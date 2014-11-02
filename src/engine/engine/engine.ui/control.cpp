#include "control.h"
#include "engine.render/camera.h"

namespace ui
{

void control::Update(float deltaTime)
{
	UNREFERENCED(deltaTime);

	m_backgroundColor.A() = m_foregroundColor.A() = m_alpha;
}

void control::Draw(const render::camera& camera)
{
	if ( m_customDrawFunction != nullptr )
	{
		m_customDrawFunction(*this);
	}
	else
	{
  		InternalDraw(camera);
	}
}

bool control::HandleInput(float deltaTime, const input::input_state& inputState)
{			
	UNREFERENCED(deltaTime);
		
	auto mouse = inputState.GetMouse();
	if ( mouse != nullptr )
	{
			
		if ( m_rectangle.Contains( mouse->GetPosition() ) )
		{
			if ( !m_mouseOver )
			{
				m_onMouseEnter.Invoke(this, *this);
				m_mouseOver = true;
			}

			if ( mouse->ButtonPressed(input::mouse::eButton::Left) )
			{
				if ( !m_focused )
				{
					m_onFocusReceived.Invoke(this, *this);
					m_focused = true;					
				}

				m_onMousePressed.Invoke(this, *this);
			}

			if ( m_focused && mouse->ButtonReleased(input::mouse::eButton::Left) )
			{				
				m_onMouseReleased.Invoke(this, *this);
				return true;
			}

		}
		else
		{
			if ( m_mouseOver )
			{
				m_onMouseLeave.Invoke(this, *this);
				m_mouseOver = false;				
			}

			if ( m_focused && mouse->ButtonPressed(input::mouse::eButton::Left) )
			{
				m_onFocusLost.Invoke(this, *this);
				m_focused = false;
			}
		}
	}

	return false;
}

const render::viewport& control::Viewport() const
{
	return m_viewport;
}

void control::SetPosition(const math::vector2& position, eUnit unit) 
{ 
	auto viewport = Viewport();
	switch ( unit )
	{
	case Percent:
		m_rectangle.SetPosition( math::vector2(viewport.Left() + position.x() * viewport.Width(), viewport.Top() + position.y() * viewport.Height()) );
		break;
	default:
	case Pixels:
		m_rectangle.SetPosition(position); 
		break;
	}

	Refresh(); 		
}

void control::SetSize(const math::vector2& size, eUnit unit /*=Pixels*/) 
{ 
	auto viewport = Viewport();
	switch ( unit )
	{
	case Percent:
		m_rectangle.SetSize(math::vector2(size.x() * viewport.Width(), size.y() * viewport.Height()) );
		break;
	default:
	case Pixels:
		m_rectangle.SetSize(size);
		break;
	}

	Refresh(); 		
	
}

void control::SetExtents(const math::vector4& extents, eUnit unit/*=Percent*/) 
{ 
	auto viewport = m_core->GetDevice()->GetViewport();
	const auto width = viewport.Width();
	const auto height = viewport.Height();
	
	switch ( unit )
	{
	case Percent:
		m_rectangle.SetPosition(math::vector2(extents.x() / width, extents.y() / height) );
		m_rectangle.SetSize(math::vector2(extents.z() / width, extents.w() / height) );
		break;
	default:
	case Pixels:
		m_rectangle.SetExtents(extents);
		break;
	}

	Refresh(); 		
}


} // ui