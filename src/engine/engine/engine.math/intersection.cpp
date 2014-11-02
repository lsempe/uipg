#include "intersection.h"
#include "vector.h"

namespace math
{

	namespace intersection
	{

		bool PointInRectangle(point p, point rectangle[4])
		{
			vector2 r0(rectangle[0].x(), rectangle[0].y());
			vector2 r1(rectangle[2].x(), rectangle[2].y());
			vector2 r2(rectangle[3].x(), rectangle[3].y());

			vector2 v(p.x(), p.y());

			vector2 dv1 = r0 - r2;
			vector2 dv2 = r1 - r2;

			vector2 twopi_p_c = (v * 2.f) - ( r0 - r2 );
	
			return	( dv2.Dot(twopi_p_c - dv2) <= 0.f && dv2.Dot(twopi_p_c + dv2) >= 0 ) && 
					( dv1.Dot(twopi_p_c - dv1) <= 0.f && dv1.Dot(twopi_p_c + dv1) >= 0 );
		}

		float PointToLineDistance(const point& a, const point& b, const point& p)
		{
			vector2 v = (b - a);
			vector2 n = v.Normalize();
			vector2 m = a - p;			
			vector2 r = m - (m * m.Dot(n));
			return r.Length();
		}

	} // intersection
} // math