#pragma once

#include "point.h"

namespace math
{
	struct ray
	{
		vector3 position;
		vector3 direction;
		float length;
	};

	namespace intersection
	{
		bool PointInRectangle(point p, point rectangle[4]);

		float PointToLineDistance(const point& a, const point& b, const point& p);

		bool RayTriangle(const ray& r, const vector3& p0, const vector3& p1, const vector3& p2, vector3& intersectionPoint);
		bool RayQuad(const ray& r, const vector3& p0, const vector3& p1, const vector3& p2, const vector3& p3, vector3& intersectionPoint);

	} // intersection

} // math