#pragma once

#include <windows.h>

#include "point.h"
#include "vector.h"

namespace math
{

/// rectangle
/// axis-aligned rectangle in 2D space
class rectangle
{
public:

	rectangle();
	rectangle(int left, int top, int width, int height);
	rectangle(float left, float top, float width, float height);
	
	static rectangle MakeRectangle(float left, float top, float right, float bottom);

	float Top() const { return m_top; }
	float Left() const { return m_left; }
	float Width() const { return m_width; }
	float Height() const { return m_height; }

	float& Top() { return m_top; }
	float& Left() { return m_left; }
	float& Width() { return m_width; }
	float& Height() { return m_height; }
	
	float Right() const { return m_left + m_width; }
	float Bottom() const { return m_top + m_height; }

	void Right(float right) 
	{		
		float offset = right - m_left; 
		m_left += offset;
		m_left -= m_width;
	}

	void Bottom(float bottom) 
	{ 
		m_top = (bottom - m_height);
	}

	const vector2 Position() const { return vector2(m_left, m_top); }
	const vector2 Size() const { return vector2(m_width, m_height); }
	const vector4 Extents() const { return vector4(m_left, m_top, m_left + m_width, m_top + m_height); }

	void SetPosition(const vector2& position) { m_left = position.x(); m_top = position.y(); }
	void SetSize(const vector2& size) { m_width = size.x(); m_height = size.y(); }
	void SetExtents(const vector4 extents) { m_left = extents.x(); m_top = extents.y(); m_width = ( extents.z() - m_left ); m_height = (extents.w() - m_top); }

	bool operator == (const rectangle& r);
	bool operator != (const rectangle& r);

	bool IsEmpty() const;
	const point Center() const;

	virtual bool Contains(float x, float y) const;
	virtual bool Intersects(const rectangle& rect) const;

	bool Contains(const vector2& position) const;
	bool Contains(const rectangle& rect) const;
	bool Contains(const point& point) const;

	void Inflate(float x, float y);
	void Contract(float x, float y);

	void Offset(float x, float y);
	void Offset(const point& point);

	rectangle& operator + (const rectangle& rhs);
	rectangle operator + (const rectangle& rhs) const;

	void Constrain(vector2& p, const vector2& size = point::Zero) const;
	void ClipTo(const rectangle& clipping_rectangle);

	operator RECT () const
	{
		RECT rc;
		rc.left = static_cast<LONG>( m_left );
		rc.top = static_cast<LONG>( m_top );
		rc.right = static_cast<LONG>( Right() );
		rc.bottom = static_cast<LONG>( Bottom() );
		return rc;
	}	

	static rectangle Zero;

protected:

	float m_left;
	float m_top;

	float m_width;
	float m_height;
};

class rectangle_oriented : public rectangle
{
public:
	rectangle_oriented() : rectangle(), m_angle(0.f) {}
	rectangle_oriented(float left, float top, float width, float height, float angle)
		: rectangle(left, top, width, height)
		, m_angle(angle)
	{		
	}
	
	virtual bool Contains(float x, float y)
	{
		float cosine = cos(m_angle);
		float sine = sin(m_angle);

		const auto center = Center();
		
		float rx = center.x() + cosine * (x - center.x()) - sine * (y - center.y());
		float ry = center.y() + sine * ( x - center.x() ) + cosine * (y - center.y() );

		return rectangle::Contains(rx, ry);
	}

	const float Angle() const { return m_angle; }
	float& Angle() { return m_angle; }

protected:
	
	float m_angle;
};

} // math