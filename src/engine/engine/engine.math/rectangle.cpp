#include "rectangle.h"
#include "math.h"
#include "vector.h"

namespace math
{

rectangle rectangle::Zero(0.f, 0.f, 0.f, 0.f);

rectangle::rectangle()
	: m_left(0.f)
	, m_top(0.f)
	, m_width(0.f)
	, m_height(0.f)
{}

rectangle::rectangle(int left, int top, int width, int height)
	: m_left(static_cast<float>(left))
	, m_top(static_cast<float>(top))
	, m_width(static_cast<float>(width))
	, m_height(static_cast<float>(height))
{}

rectangle::rectangle(float left, float top, float width, float height)
	: m_left(left)
	, m_top(top)
	, m_width(width)
	, m_height(height)
{}

/*static*/ rectangle rectangle::MakeRectangle(float left, float top, float right, float bottom)
{
	rectangle r(left, top, right - left, bottom - top);
	return r;
}

bool rectangle::operator == (const rectangle& r)
{
	return (IsEqual(m_left, r.Left()) && IsEqual(m_top, r.Top()) && IsEqual(m_width, r.Width()) && IsEqual(m_height, r.Height()));
}

bool rectangle::operator != (const rectangle& r)
{
	return (!IsEqual(m_left, r.Left()) || !IsEqual(m_top, r.Top()) || !IsEqual(m_width, r.Width()) || !IsEqual(m_height, r.Height()));
}

bool rectangle::IsEmpty() const
{
	return (IsEqual(m_width, 0.f) || IsEqual(m_height, 0.f));
}

const point rectangle::Center() const
{	
	return point(m_left + (m_width*0.5f), m_top + (m_height*0.5f));
}

bool rectangle::Contains(float x, float y) const
{	
	return ( x >= Left() && x <= Right() && y >= Top() && y <= Bottom() );	
}

bool rectangle::Contains(const rectangle& rect) const
{
	if ( rect.Top() < Top() || rect.Bottom() > Bottom() || rect.Left() < Left() || rect.Right() > Right() )
		return false;

	return true;
}

bool rectangle::Contains(const point& point) const
{
	return Contains(point.x(), point.y());
}

bool rectangle::Contains(const vector2& position) const
{
	return Contains(position.x(), position.y());
}

bool rectangle::Intersects(const rectangle& rect) const
{
	return (Left() < rect.Right() && Right() > rect.Left() &&
			Top() < rect.Bottom() && Bottom() > rect.Top());
}

void rectangle::Constrain(vector2& p, const vector2& size/*=point::Zero*/) const
{
	float halfWidth = size.x() * 0.5f;
	float halfHeight = size.y() * 0.5f;

	if ( p.x() - halfWidth < m_left )
	{
		p.x() = m_left;
	}

	if ( p.x() + halfWidth > Right() )
	{
		p.x() = Right() - size.x();
	}

	if ( p.y() - halfHeight < m_top )
	{
		p.y() = m_top;
	}

	if ( p.y() + halfHeight >= Bottom() )
	{
		p.y() = Bottom() - size.y();
	}
}

void rectangle::ClipTo(const rectangle& clipping_rectangle)
{
	if ( m_left < clipping_rectangle.Left() )
	{
		m_width -= clipping_rectangle.Left() - m_left;
		m_left = clipping_rectangle.Left();
	}

	if ( m_top < clipping_rectangle.Top() )
	{
		m_height -= clipping_rectangle.Top() - m_top;
		m_top = clipping_rectangle.Top();
	}

	if ( Right() > clipping_rectangle.Right() )
	{
		m_width -= ( Right() - clipping_rectangle.Right() );
	}

	if ( Bottom() > clipping_rectangle.Bottom() )
	{
		m_height -= ( Bottom() - clipping_rectangle.Bottom() );
	}
}

void rectangle::Inflate(float x, float y)
{
	m_left -= x * 0.5f;
	m_width += x;

	m_top -= y * 0.5f;
	m_height += y;
}

void rectangle::Contract(float x, float y)
{
	m_left += x * 0.5f;
	m_width -= x;

	m_top += y * 0.5f;
	m_height -= y;
}

void rectangle::Offset(float x, float y)
{
	m_left += x;
	m_top += y;
}

void rectangle::Offset(const point& point)
{
	Offset(point.x(), point.y());
}

rectangle& rectangle::operator + (const rectangle& rhs)
{
	m_left += rhs.Left();
	m_top += rhs.Top();
	m_width += rhs.Width();
	m_height += rhs.Height();
	return *this;
}

rectangle rectangle::operator + (const rectangle& rhs) const
{	
	return rectangle(m_left + rhs.Left(), m_top + rhs.Top(), m_width + rhs.Width(), m_height +  rhs.Height());
}

} // math