#pragma once

#include <memory>

#include "gradient.h"

namespace render
{

	class gradient_radial : public gradient
	{
	public:

		gradient_radial(unsigned int size, const color& startColor, const color& endColor, float sizeRatio=0.75f, bool reverse=false)
			: gradient(size, startColor, endColor, reverse)
		{
			m_data = new unsigned int[size*size];
			memset(m_data, 0x00000000, sizeof(unsigned int)*size*size);

			const math::vector2 center(size/2, size/2);
			const float radius = center.Length() * sizeRatio;

			for ( unsigned int y = 0; y < size; ++y )
			{
				for ( unsigned int x = 0; x < size; ++x )
				{
					float xx = x - center.x() + 0.5f;
					float yy = y - center.y() + 0.5f;
					float r = sqrtf(xx*xx + yy*yy) / radius;

					color c =  reverse ? (startColor * (1-r)) + (endColor * (r)) : (startColor * r) + (endColor * (1 - r));

					m_data[y * size + x] =  c.ToU32();
				}
			}
		}
	};

}  // ui