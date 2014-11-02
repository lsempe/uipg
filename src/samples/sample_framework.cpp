#include "stdafx.h"

#include "sample_framework.h"

namespace samples
{
	framework::framework(HWND& windowHandle, int clientWidth, int clientHeight)
		: game::core(windowHandle, clientWidth, clientHeight)
	{}

	framework::~framework() {}

	void framework::Initialize()
	{
		game::core::Initialize();

		m_lastTimePoint = std::chrono::system_clock::now();

		m_device.Create(m_windowHandle);

		m_fontLibrary = std::unique_ptr<ui::font_library>(new ui::font_library(&m_device));
	}

	void framework::Shutdown()
	{
	}

	void framework::Update()
	{
		auto current = std::chrono::system_clock::now();
		auto diff = current - m_lastTimePoint;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
						
		Update(static_cast<float>(ms.count() / 1000.f));

		m_lastTimePoint = current;

		PreRender();
		Render();
		Present();
	}

	void framework::Update(float delta)
	{
		UNREFERENCED(delta);
	}

	void framework::PreRender()
	{
		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
		m_device.GetImmediateContext()->ClearRenderTargetView( m_device.GetRenderTargetView(), ClearColor );
	
		//
		// Clear the depth buffer to 1.0 (max depth)
		//
		m_device.GetImmediateContext()->ClearDepthStencilView( m_device.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );	
	}

	void framework::Present()
	{
		//
		// Present our back buffer to our front buffer
		//
		m_device.GetSwapChain()->Present( 0, 0 );
	}

}
