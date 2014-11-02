#pragma once

#include "vector.h"
#include "point.h"

namespace math
{

class ellipse
{
public:

	ellipse(const vector2& center = point::Zero, const vector2& axes = vector2::One, float angleRadians = 0.f)
		: m_center(center)
		, m_axes(axes)
		, m_angle(angleRadians)
	{
	}

	void GetPoints(int count, std::vector<vector2>& points)
	{
        for (int angle = 0; angle < 360; angle += 360 / count)
        {
			vector2 p = GetPointOnEllipse(static_cast<float>(angle));
			points.push_back(p);
        }
	}

	vector2 GetPointOnEllipse(float angle)
	{
		float sinAlpha = sin(angle);
		float cosAlpha = cos(angle);

		float beta = -m_angle;
        float sinBeta = sin(beta);
        float cosBeta = cos(beta);

		float X = (m_axes.x() * cosAlpha * cosBeta - m_axes.y() * sinAlpha * sinBeta);
		float Y = (m_axes.x() * cosAlpha * sinBeta + m_axes.y() * sinAlpha * cosBeta);

		return vector2(X,Y) + m_center;
	}

	vector2 GetPointFromAngle(float angleRadians)
	{
		return GetPointOnEllipse(angleRadians + HalfPi);
	}

	vector2 GetPointOnEllipse(const vector2& direction)
	{
		float alpha = atan2(direction.y(), direction.x()) + HalfPi;
		return GetPointOnEllipse(alpha);
	}
	
	vector2 GetPointInEllipse(const vector2& position)
    {
		vector2 p1 = GetPointOnEllipse(position);
		vector2 v1 = p1 - m_center;
		
		if (position.LengthSquared() < v1.LengthSquared())
			return m_center + position;

		return p1;
	}

	float& Angle() { return m_angle; }
	const float Angle() const { return m_angle; }
	const vector2& Axes() const { return m_axes; }
	const vector2& Center() const { return m_center; }

protected:

	float m_angle;
	vector2 m_axes;
	vector2 m_center;
};



} // math