#pragma once

#include "gradient.h"
#include "texture.h"

namespace render
{

	class gradient_texture
	{
	public:
		gradient_texture(device_direct3d* device, gradient& gradient)
			: m_device(device)
			, m_gradient(gradient)
		{
			m_texture = std::shared_ptr<texture>( texture::Create(device, gradient.Size(), gradient.Size(), 1));

			auto context = device->GetImmediateContext();
			auto resource = m_texture->GetTexture2D();
			D3D11_MAPPED_SUBRESOURCE res;
		
			context->Map(*resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);		
			memcpy(res.pData, gradient.Data(), m_texture->Size());
			context->Unmap(*resource, 0);
		}

		void Save(const std::wstring& path)
		{	
			std::wstring folder;
			path::GetPath(path.c_str(), folder);
			path::CreateFolder(folder.c_str());
			m_texture->SaveAsDDS(path);
		}

		std::shared_ptr<texture> Texture() const { return m_texture; }

	protected:

		device_direct3d* m_device;
		gradient& m_gradient;
		std::shared_ptr<texture> m_texture;	
	};

} // render