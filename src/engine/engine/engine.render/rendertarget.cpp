#include "rendertarget.h"

namespace render
{

	rendertarget* rendertarget::Create(device_direct3d* device, unsigned int width, unsigned int height)
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		HRESULT result;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		rendertarget* rt = new rendertarget(device, width, height);

		// Initialize the render target texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		result = device->GetDevice()->CreateTexture2D(&textureDesc, NULL, rt->GetTexture2D());
		if(FAILED(result))
		{
			delete rt;
			return false;
		}

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = device->GetDevice()->CreateRenderTargetView(*rt->GetTexture2D(), &renderTargetViewDesc, rt->GetRenderTargetView());
		if(FAILED(result))
		{
			delete rt;
			return nullptr;
		}

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = device->GetDevice()->CreateShaderResourceView(*rt->GetTexture2D(), &shaderResourceViewDesc, rt->GetView());
		if(FAILED(result))
		{
			delete rt;
			return nullptr;
		}

		return rt;
	}

} // render