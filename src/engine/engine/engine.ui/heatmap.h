#pragma once

#include <list>
#include <memory>

#include "engine.render/device_direct3d.h"
#include "engine.math/vector.h"
#include "engine.render/gradient_radial.h"
#include "engine.core/game.h"

namespace ui
{

	class heatmap
	{
	public:

		heatmap(render::device_direct3d* device, int size, int pointSize, const std::wstring colorMapPath)
			: m_device(device)
			, m_size(size)
			, m_data(nullptr)
			, m_gradient(pointSize, render::color(0.f, 0.f, 0.f, 0.f), render::color(1.f, 1.f, 1.0f, 1.f))		
		{
			m_data = new unsigned int[size*size];
			memset(m_data, 0, sizeof(unsigned int)*size*size);

			m_colorLUT = std::unique_ptr<color_lut>(new color_lut(device, colorMapPath));
		}

		~heatmap()
		{
			delete [] m_data;
		}

		void AddPoint(const math::point& point)
		{
			const unsigned int* gradient = m_gradient.Data();

			const auto gradientSize = m_gradient.Size();
			float gradientRadius = gradientSize * 0.5f;

			unsigned int offsetX = static_cast<int>( point.x() - gradientRadius );
			unsigned int offsetY = static_cast<int>( point.y() - gradientRadius );
			math::Clamp(offsetX, 0U, m_size);
			math::Clamp(offsetY, 0U, m_size);

			for (unsigned int y = 0; y < gradientSize; ++y )
			{
				for (unsigned int x = 0; x < gradientSize; ++x )
				{
					unsigned int indexX = (offsetX + x);
					unsigned int indexY = (offsetY + y);
					math::Clamp(indexX, 0U, m_size-1);
					math::Clamp(indexY, 0U, m_size-1);

					auto& sourcePixel = m_data[indexY * m_size + indexX ];
				 
					const render::color source = render::color(sourcePixel);
					const render::color grad = render::color(gradient[y * gradientSize + x]);
									
					const render::color destination = (grad * grad.A()) + source;

					sourcePixel = destination.ToU32();
				}
			}
		}

		void Colorize()
		{
			for (unsigned int i = 0; i < m_size*m_size; ++i )
			{
				auto& source = m_data[i];
			
				unsigned char a = ( source >> 24 );
				unsigned int finalColor =  (*m_colorLUT)[a];
				finalColor = ( (*m_colorLUT)[a] & 0x00ffffff) | ( a << 24 );
				render::color c = render::color(finalColor);
				source = c.ToU32ABGR();			
			}
		}

		void Save(std::wstring path)
		{
			std::unique_ptr<render::texture> texture = std::unique_ptr<render::texture>( render::texture::Create(m_device, m_size, m_size, 1, m_data));

			auto resource = texture->GetTexture2D();
			D3D11_MAPPED_SUBRESOURCE res;
			m_device->GetImmediateContext()->Map(*resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
		
			memcpy(res.pData, m_data, texture->Size());

			m_device->GetImmediateContext()->Unmap(*resource, 0);

			std::wstring final_path;
			path::GetPath(path.c_str(), final_path);
			path::CreateFolder(final_path.c_str());
			texture->SaveAsDDS(path);
		}

	private:

		unsigned int m_size;
		unsigned int* m_data;

		render::gradient_radial m_gradient;

		render::device_direct3d* m_device;
	
		class color_lut
		{
		public:
			color_lut(render::device_direct3d* device, const std::wstring& path)
			{
				std::unique_ptr<render::texture> texture = std::unique_ptr<render::texture>(new render::texture(device, path) );
				if ( texture->Width() != 256 && texture->Height() != 1 )
				{
					throw std::out_of_range("The color look up table texture must be 256x1");
				}

				auto& image = texture->GetImage();
				const auto& pixels = image.GetPixels();
				memcpy(m_data, pixels, image.GetPixelsSize());			
			}

			unsigned int operator [](const unsigned char i) const { return m_data[i]; }

		private:

			unsigned int m_data[256];
		 
		};

		std::unique_ptr<color_lut> m_colorLUT;
	};

} // ui