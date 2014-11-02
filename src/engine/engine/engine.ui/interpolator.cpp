#include "interpolator.h"

namespace ui
{

	namespace curves
	{

		float none(float t)
		{
			UNREFERENCED(t);
			return 0;
		}

		float linear(float t)
		{
			return t;
		}

		float quadratic(float t)
		{
			return t * t;
		}

		float cubic(float t)
		{
			return t * t * t;
		}

		float quart(float t)
		{
			return t * t * t * t;
		}

		float quint(float t)
		{
			return t * t * t * t * t;
		}

		float sine(float t)
		{
			return 1.f - cos(t * math::HalfPi);
		}

		float expo(float t)
		{
			return t == 0.f ? 0.f : powf(2, 10.f * (t - 1.f));
		}

		float elastic(float t)
		{
			const float v(t - 1.0f);
			const float p(0.3f);
			return -powf(2, 10.0f * v) * sinf( (v - p / 4.0f) * math::TwoPi / p );
		}

		float circular(float t)
		{
			return 1 - sqrt(1 - t * t);
		}

	} // curves

} // ui