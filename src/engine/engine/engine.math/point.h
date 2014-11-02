#pragma once

#include "math.h"
#include "vector.h"

namespace math
{


///
/// point
/// Represents a point in 2D space
class point
{
public:

	point(int x, int y);
	point(float x, float y);

	static const point Zero;

	bool operator == (const point& rhs) const;
	bool operator != (const point& rhs) const;

	float& x() { return m_x; }
	float& y() { return m_y; }

	const float x() const { return m_x; }
	const float y() const { return m_y; }

	operator vector2() const
	{
		return vector2(m_x, m_y);
	}

	const point operator - (const point &p) const
	{
		return point(m_x - p.x(), m_y - p.y());
	}

	const point operator + (const point &p) const
	{
		return point(m_x + p.x(), m_y + p.y());
	}

protected:

	float m_x;
	float m_y;

};

} // math