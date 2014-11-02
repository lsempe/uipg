#pragma once

#include "device.h"

#include <d3d11.h>

#include "engine.math/matrix.h"
#include "viewport.h"

namespace render
{

class rendertarget;

namespace eMapFlags
{
	enum eFlags
	{
		Discard        = 1,
		Read           = 2, // do not use
		Unsynchronized = 4, // like D3D11_MAP_NO_OVERWRITE
	};
}


namespace eBufferUsage
{
// Buffer types used for uploading geometry & constants.
enum BufferUsage
{
    Unknown  = 0,
    Vertex   = 1,
    Index    = 2,
    Constant  = 4,
    TypeMask = 0xff,
    ReadOnly = 0x100, // Buffer must be created with Data().
};
}

class device_direct3d : public device
{
public:
	device_direct3d();
	virtual ~device_direct3d();

	virtual HRESULT Create(const HWND& windowHandle);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateContext() { return m_pImmediateContext; }
	
	ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; }
	ID3D11RenderTargetView* const * RenderTargetView() const { return &m_pRenderTargetView; }
	
	IDXGISwapChain*	GetSwapChain() { return m_pSwapChain; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; }

	virtual void SetViewport(const viewport& vp);
	virtual render::viewport GetViewport(unsigned int numViewport = 0) const;

	void SetProjectionMatrix(const math::matrix& projectionMatrix)
	{
		m_projectionMatrix = projectionMatrix;
	}

	const math::matrix& GetProjectionMatrix() const { return m_projectionMatrix; }

	void SetViewMatrix(const math::matrix& viewMatrix)
	{
		m_viewnMatrix = viewMatrix;
	}

	const math::matrix& GetViewMatrix() const { return m_viewnMatrix; }

	void SetRenderTarget(std::shared_ptr<rendertarget> renderTarget);
	void ResetRenderTarget();

private:

	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	ID3D11Device*			m_device;
	ID3D11Debug*			m_debug;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthStencil;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DeviceContext*	m_pImmediateContext;

	math::matrix m_projectionMatrix;
	math::matrix m_viewnMatrix;

	viewport m_viewport;
};

// Helper sets a D3D resource name string (used by PIX and debug layer leak reporting).
template<UINT TNameLength>
inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char (&name)[TNameLength])
{
    #if defined(_DEBUG) || defined(PROFILE)
        resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
    #endif
}

} // render