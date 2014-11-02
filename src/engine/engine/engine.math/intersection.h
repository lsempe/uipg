#pragma once

#include "point.h"

namespace math
{
	namespace intersection
	{

		bool PointInRectangle(point p, point rectangle[4]);

		float PointToLineDistance(const point& a, const point& b, const point& p);
		
	} // intersection

} // math