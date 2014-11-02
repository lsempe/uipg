#include "point.h"
#include "math.h"

namespace math
{

const point point::Zero(0.f, 0.f);

point::point(int x, int y)
	: m_x(static_cast<float>(x))
	, m_y(static_cast<float>(y))
{}

point::point(float x, float y)
	: m_x(x)
	, m_y(y)
{}

bool point::operator == (const point& rhs) const
{
	return ( IsEqual(m_x, rhs.x()) && IsEqual(m_y, rhs.y()) );
}

bool point::operator != (const point& rhs) const
{
	return ( !IsEqual(m_x, rhs.x()) || !IsEqual(m_y, rhs.y()) );
}

} // math