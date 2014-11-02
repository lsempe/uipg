#pragma once

#include "engine.render/color.h"
#include "engine.render/texture.h"

namespace render
{
	class gradient
	{
	public:
		gradient(unsigned int size, const color& startColor, const color& endColor, bool reverse=false)
			: m_size(size)
			, m_startColor(startColor)
			, m_endColor(endColor)
			, m_reverse(reverse)
			, m_data(nullptr)
		{
		}

		virtual ~gradient()
		{
			if ( m_data != nullptr )
			{
				delete [] m_data;
			}
		}

		void Save(const std::wstring& path, device_direct3d* device)
		{
			std::unique_ptr<texture> tex = std::unique_ptr<texture>( texture::Create(device, m_size, m_size, 1));

			auto context = device->GetImmediateContext();
			auto resource = tex->GetTexture2D();
			D3D11_MAPPED_SUBRESOURCE res;
			
			context->Map(*resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);		
			memcpy(res.pData, m_data, tex->Size());
			context->Unmap(*resource, 0);

			std::wstring folder;
			path::GetPath(path.c_str(), folder);
			path::CreateFolder(folder.c_str());
			tex->SaveAsDDS(path);
		}

		const unsigned int* Data() const { return m_data; }
		const unsigned int Size() const { return m_size; }

	protected:

		unsigned int m_size;
		unsigned int* m_data;

		color m_startColor;
		color m_endColor;
		bool m_reverse;

	};
}