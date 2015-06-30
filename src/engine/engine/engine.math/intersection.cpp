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

		bool RayTriangle(const ray& r, const vector3& p0, const vector3& p1, const vector3& p2, vector3& intersectionPoint)
		{
			intersectionPoint = vector3::Zero;

			// Get the triangle edge vectors
			vector3 e0 = p1 - p0;
			//e0.Normalize();

			vector3 e1 = p2 - p0;
			//e1.Normalize();

			// Get the triangle's normal
			vector3 normal = e0.Cross(e1);

			// If the normal is a 0 vector, the triangle is degenerate
			if (normal.LengthSquared() <= Epsilon)
				return false;

			// Direction from point on triangle to ray origin
			vector3 d0 = r.position - p0;
			float a = normal.Dot(d0);
			float b = normal.Dot(r.direction);

			if (Abs(b) < Epsilon)
			{
				if (IsEqual(a, 0.f)) // Ray lies on the triangle's plane
					return true; // 2
				else // ray is disjoint from the plane
					return false; // 0 
			}

			// Calculate intersection point between ray and triangle's plane
			float d = a / b;
			if (d < 0.f)
				return false; // ray goes away from triangle

			intersectionPoint = r.position + d * r.direction;

			float uu = e0.Dot(e0);
			float uv = e0.Dot(e1);
			float vv = e1.Dot(e1);

			vector3 w = intersectionPoint - p0;
			float wu = w.Dot(e0);
			float wv = w.Dot(e1);

			float D = uv * uv - uu * vv;

			// Test parametric coordinates
			float s = (uv * wv - vv * wu) / D;
			if (s < 0.f || s > 1.f)
				return false; // intersectionPoint is outside of the triangle

			float t = (uv * wu - uu * wv) / D;
			if (t < 0.f || (s + t) > 1.f)
				return false; // intersectionPoint is outside of the triangle


			return true;
		}

		// Test single sided quad, assumes counter clockwise winding order
		bool RayQuad(const ray& r, const vector3& p0, const vector3& p1, const vector3& p2, const vector3& p3, vector3& intersectionPoint)
		{
			intersectionPoint = vector3::Zero;

			// Test triangle 1 
			if (RayTriangle(r, p0, p1, p2, intersectionPoint))
				return true;

			// Test triangle 2
			if (RayTriangle(r, p2, p1, p3, intersectionPoint))
				return true;

			return false;
		}

	} // intersection
} // math