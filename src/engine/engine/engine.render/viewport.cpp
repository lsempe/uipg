#include "viewport.h"

namespace render
{

	viewport::viewport()
		: m_left(0.f)
		, m_top(0.f)
		, m_right(0.f)
		, m_bottom(0.f)
		, m_minDepth(0.f)
		, m_maxDepth(0.f)
		, m_titleSafeRatio(0.f)
	{}

	viewport::viewport(float left, float top, float right, float bottom, float minDepth, float maxDepth, float titleSafeRatio /*= 0.8f*/)
	: m_left(left)
	, m_top(top)
	, m_right(right)
	, m_bottom(bottom)
	, m_minDepth(minDepth)
	, m_maxDepth(maxDepth)
	, m_titleSafeRatio(titleSafeRatio)
	{		
		CalculateTitleSafeArea();
	}

	viewport::viewport(const viewport& rhs)
		: m_left(rhs.m_left)
		, m_top(rhs.m_top)
		, m_right(rhs.m_right)
		, m_bottom(rhs.m_bottom)
		, m_minDepth(rhs.m_minDepth)
		, m_maxDepth(rhs.m_maxDepth)
		, m_titleSafeRatio(rhs.m_titleSafeRatio)
	{
		CalculateTitleSafeArea();
	}

	viewport& viewport::operator = (const viewport& rhs)
	{
		m_left = rhs.m_left;
		m_top = rhs.m_top;
		m_right = rhs.m_right;
		m_bottom = rhs.m_bottom;
		m_minDepth = rhs.m_minDepth;
		m_maxDepth = rhs.m_maxDepth;
		m_titleSafeRatio = rhs.m_titleSafeRatio;
	
		CalculateTitleSafeArea();

		return *this;
	}

	const float viewport::Left() const  { return m_left; }
	const float viewport::Right() const { return m_right; }
	const float viewport::Top() const { return m_top; }
	const float viewport::Bottom() const { return m_bottom; }

	const float viewport::Width() const { return m_right - m_left; }
	const float viewport::Height() const { return m_bottom - m_top; }

	const float viewport::Aspect() const { return Width() / Height(); }

	const float viewport::MinDepth() const { return m_minDepth; }
	const float viewport::MaxDepth() const { return m_maxDepth; }

	const math::rectangle& viewport::TitleSafeArea() const { return m_titleSafeArea; }

	// Project
	// Projects a world space (3D) position into screen space (2D).
	math::vector3 viewport::Project(const math::vector3& worldPosition, const math::matrix& projection, const math::matrix& view, const math::matrix& world) const
	{
		math::matrix wvp = world * view * projection;
		math::vector4 result = math::vector4::Transform(worldPosition, wvp);

		result.z() = result.z() * (m_maxDepth - m_minDepth);
    
		if ( result.w() > 0.f )
			result /= result.w();

		math::vector3 finalResult = result;

		finalResult.x() = m_left + ( 1.f + finalResult.x()) * (Width() * 0.5f);
		finalResult.y() = m_top + ( 1.f - finalResult.y()) * (Height() * 0.5f);
		finalResult.z() = finalResult.z() + m_minDepth;

		return finalResult;
	}

	// Unproject
	// Projects a screen space position (2D) into a world space position (3D).
	// The Z component of screenPosition will be used as a ratio of the depth of the view frustum, using 0.f means the world space position
	// returned will be at the near plane, using 1.f will place it at the far plane.
	math::vector3 viewport::Unproject(const math::vector3& screenPosition, const math::matrix& projection, const math::matrix& view, const math::matrix& world) const
	{
		math::vector4 result;
		result.x() = (2.f * (screenPosition.x() - m_left) / Width()) - 1.f;
		result.y() = (2.f * (screenPosition.y() - m_top) / Height()) - 1.f;
		result.z() = screenPosition.z() - m_minDepth;

		if ( math::IsEqual(m_maxDepth - m_minDepth, 0.f ) )
			result.z() = 0.f;
		else
			result.z() = result.z() / (m_maxDepth - m_minDepth);

		result.w() = 1.f;	
	
		math::matrix iwvp = math::matrix::Invert(world * view * projection);

		math::vector4 finalResult = math::vector4::Transform(result, iwvp);

		finalResult /= finalResult.w();

		return finalResult;
	}

	void viewport::CalculateTitleSafeArea()
	{
		float xwidth = (Width() * (1.f - m_titleSafeRatio));
		float yheight = (Height() * (1.f - m_titleSafeRatio));

		m_titleSafeArea = math::rectangle(m_left + (xwidth * 0.5f), m_top + (yheight * 0.5f), Width() - xwidth, Height() - yheight);
	}

} // render