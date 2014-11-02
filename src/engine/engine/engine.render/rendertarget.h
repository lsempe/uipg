#pragma once

#include "texture.h"
#include <d3d11.h>
#include "color.h"

namespace render
{

	class rendertarget : public texture
	{
	public:

		rendertarget(device_direct3d* device, unsigned int width, unsigned int height)
			: texture(device, width, height)
		{
		}

		virtual ~rendertarget()
		{
			if ( m_renderTargetView )
			{
				m_renderTargetView->Release();
			}

			if ( m_depthStencilView )
			{
				m_depthStencilView->Release();
			}
		}

		static rendertarget* Create(device_direct3d* device, unsigned int width, unsigned int height);

		void Set(ID3D11DepthStencilView* depthStencilView = nullptr)
		{
			if ( depthStencilView != nullptr )
			{
				m_depthStencilView = depthStencilView;
				m_device->GetImmediateContext()->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
			}
			else
			{
				m_device->GetImmediateContext()->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
			}
		}

		void Clear(render::color color)
		{
			// Clear the render target
			m_device->GetImmediateContext()->ClearRenderTargetView(m_renderTargetView, color);
    
			//// Clear the depth buffer.
			if ( m_depthStencilView != nullptr )
			{
				m_device->GetImmediateContext()->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
		}

		ID3D11RenderTargetView** GetRenderTargetView() { return &m_renderTargetView; }
		ID3D11DepthStencilView** GetDepthStencilView() { return &m_depthStencilView; }

	protected:

		ID3D11RenderTargetView* m_renderTargetView;
		ID3D11DepthStencilView* m_depthStencilView;
	};

} // render