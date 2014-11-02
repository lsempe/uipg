#include <d3d11.h>
#include "device_direct3d.h"
#include "texture_black.h"
#include "texture_white.h"
#include "rendertarget.h"

using namespace render;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib") 

device_direct3d::device_direct3d()
	: m_driverType(D3D_DRIVER_TYPE_NULL)
	, m_featureLevel(D3D_FEATURE_LEVEL_11_0)
	, m_device(nullptr)
	, m_pImmediateContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pDepthStencil(nullptr)
	, m_pDepthStencilView(nullptr)
{	
}	 

device_direct3d::~device_direct3d()
{
    if( m_pDepthStencil ) m_pDepthStencil->Release();
    if( m_pDepthStencilView ) m_pDepthStencilView->Release();
    if( m_pRenderTargetView ) m_pRenderTargetView->Release();
    if( m_pSwapChain ) m_pSwapChain->Release();
	if( m_device ) m_device->Release();

	if( m_pImmediateContext ) 
	{
		m_pImmediateContext->ClearState();
		m_pImmediateContext->Flush();
		m_pImmediateContext->Release();
	}
	
	if ( m_debug ) 
	{
		m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL); //D3D11_RLDO_DETAIL,D3D11_RLDO_SUMMARY
		m_debug->Release();
	}

}

ID3D11Device* device_direct3d::GetDevice() 
{ 
	return m_device; 
}

HRESULT device_direct3d::Create(const HWND& windowHandle)
{
	RECT rc;
	GetWindowRect( windowHandle, &rc );

    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = windowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
	
	HRESULT hr = S_OK;
    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_device, &m_featureLevel, &m_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = m_device->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_device->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = m_device->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;

    m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

#ifdef _DEBUG
	m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));
#endif 

	m_blackTexture = std::shared_ptr<texture>(new texture(this, textures::black::data, textures::black::size));
	m_whiteTexture = std::shared_ptr<texture>(new texture(this, textures::white::data, textures::white::size));


	SetViewport(render::viewport(0.f, 0.f, (float)width, (float)height, 0.f, 1.f));

	return S_OK;
}

void device_direct3d::SetViewport(const viewport& vp)
{
	D3D11_VIEWPORT d3dvp;
	d3dvp.TopLeftX = vp.Left();
	d3dvp.TopLeftY = vp.Top();
	d3dvp.Width = vp.Width();
	d3dvp.Height = vp.Height();
	d3dvp.MinDepth = vp.MinDepth();
	d3dvp.MaxDepth = vp.MaxDepth();

	GetImmediateContext()->RSSetViewports(1, &d3dvp);

	m_viewport = vp;
}

render::viewport device_direct3d::GetViewport(unsigned int numViewport/*=0*/) const
{
	unsigned int numViewports = 0;
	D3D11_VIEWPORT viewport;
	m_pImmediateContext->RSGetViewports(&numViewports, &viewport);
	if ( numViewports > 0 && numViewport < numViewports )
	{		
		return render::viewport(viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth);
	}

	return m_viewport;
}

void device_direct3d::SetRenderTarget(std::shared_ptr<rendertarget> renderTarget)
{
	auto renderTargetView = renderTarget->GetRenderTargetView();
	GetImmediateContext()->OMSetRenderTargets(1, renderTargetView, *renderTarget->GetDepthStencilView());
}

void device_direct3d::ResetRenderTarget()
{
	GetImmediateContext()->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}