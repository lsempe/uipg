#pragma once

#include <string>
#include "device_direct3d.h"
#include "DDSTextureLoader.h"
#include "DirectXTex/DirectXTex.h"
#include "engine.core/path.h"

namespace render
{

	class texture
	{

	public:

	
		texture(device_direct3d* device, unsigned int width, unsigned int height)
			: m_device(device)
			, m_view(nullptr)
			, m_width(width)
			, m_height(height)
		{
		}

		texture(device_direct3d* device, const std::wstring& filename)
			: m_device(device)
			, m_view(nullptr)
			, m_width(0)
			, m_height(0)
		{
			if ( path::FileExists(filename.c_str()) )
			{
				DirectX::CreateDDSTextureFromFile(device->GetDevice(), filename.c_str(), reinterpret_cast<ID3D11Resource**>(&m_texture2d), &m_view);
			}
			else
			{
				throw new std::exception("could not find texture");
			}
			Init();
		}

		texture(device_direct3d* device, unsigned char* ddsData, unsigned int ddsDataSize)
			: m_device(device)
			, m_view(nullptr)
			, m_width(0)
			, m_height(0)
		{
			DirectX::CreateDDSTextureFromMemory(device->GetDevice(), ddsData, ddsDataSize, reinterpret_cast<ID3D11Resource**>(&m_texture2d), &m_view);
			Init();
		}

		virtual ~texture()
		{
			if ( m_texture2d )
			{ 
				m_texture2d->Release();
			}

			if ( m_view )
			{
				m_view->Release();
			}
		}

		bool SaveAsDDS(const std::wstring& path);

		unsigned int Width() const { return m_width; }
		unsigned int Height() const { return m_height; }

		unsigned int Size() const 
		{
			auto desc = GetDesc();
			size_t rowPitch = 0, slicePitch = 0;
			DirectX::ComputePitch(desc.Format, desc.Width, desc.Height, rowPitch, slicePitch);
			return (unsigned int)(desc.Height * rowPitch);
		}

		ID3D11ShaderResourceView** GetView() { return &m_view; }
		ID3D11Texture2D** GetTexture2D() { return &m_texture2d; }

		static texture* Create(device_direct3d* device, unsigned int width, unsigned int height, int mipCount, unsigned int* data = nullptr);
	
		DirectX::ScratchImage& GetImage() 
		{ 
			DirectX::ScratchImage img;
			DirectX::CaptureTexture(m_device->GetDevice(), m_device->GetImmediateContext(), m_texture2d, m_image);
			return m_image; 
		}

		D3D11_TEXTURE2D_DESC GetDesc() const { D3D11_TEXTURE2D_DESC desc; m_texture2d->GetDesc(&desc); return desc; }

	private:

		void Init()
		{
			D3D11_TEXTURE2D_DESC desc = GetDesc();
			m_width = desc.Width;
			m_height = desc.Height;
		}

		D3D11_TEXTURE2D_DESC m_desc;

		unsigned int m_width;
		unsigned int m_height;

		DirectX::ScratchImage m_image;
	
	protected:

		device_direct3d* m_device;
		ID3D11ShaderResourceView* m_view;
		ID3D11Texture2D* m_texture2d;


	};

} // render