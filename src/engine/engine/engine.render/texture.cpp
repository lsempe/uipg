#include "texture.h"

namespace render
{

	/*static */texture* texture::Create(device_direct3d* device, unsigned int width, unsigned int height, int mipCount, unsigned int* bytes /*= nullptr*/)
	{
		UNREFERENCED(bytes);
		UNREFERENCED(mipCount);

		texture* t = new texture(device, width, height);
	
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;

		auto texture = t->GetTexture2D();
		device->GetDevice()->CreateTexture2D( &desc, nullptr, texture);
	
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset( &SRVDesc, 0, sizeof( SRVDesc ) );
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		auto shaderResourceView = t->GetView();
		device->GetDevice()->CreateShaderResourceView( *texture, &SRVDesc, shaderResourceView );

		return t;
	}

	bool texture::SaveAsDDS(const std::wstring& path)
	{
		DirectX::ScratchImage img;
		DirectX::CaptureTexture(m_device->GetDevice(), m_device->GetImmediateContext(), m_texture2d, img);
	
		const DirectX::Image* i = img.GetImages();
		size_t nimg = img.GetImageCount();
		DirectX::SaveToDDSFile( i, nimg, img.GetMetadata(), DirectX::DDS_FLAGS_NONE, path.c_str() );

		return false;
	}

} // render