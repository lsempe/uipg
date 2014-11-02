#pragma once

#include "gradient.h"

namespace render
{

	class gradient_linear : public gradient
	{
	public:

		gradient_linear(unsigned int size, const color& startColor, const color& endColor, float angle, bool reverse=false)
			: gradient(size, startColor, endColor, reverse)
			, m_angle(angle)
		{
			m_data = new unsigned int[size*size];

			const float angleSin = sinf(angle);
			const float angleCos = cosf(angle);
			for ( unsigned int y = 0; y < size; ++y )
			{
				for ( unsigned int x = 0; x < size; ++x )
				{
					float xx = x * angleSin;
					float yy = y * angleCos;

					float r = sqrtf(xx*xx + yy*yy) / size;

					color c =  reverse ? (startColor * (1-r)) + (endColor * (r)) : (startColor * r) + (endColor * (1 - r));

					m_data[y * size + x] =  c.ToU32();
				}
			}
		}

	protected:

		float m_angle;	

	};

}  // ui