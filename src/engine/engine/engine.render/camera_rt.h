#pragma once

#include <memory>

#include "camera.h"
#include "rendertarget.h"
#include "color.h"
#include "viewport.h"

namespace render
{

	class camera_rt : public camera
	{
	public:

		camera_rt(device_direct3d* device, unsigned int width, unsigned int height)
			: m_device(device)
			, m_clearColor(render::color::PURPLE)
			, m_clear(true)
			, m_viewport(0.f, 0.f, static_cast<float>(width), static_cast<float>(height), 0.f, 1.f)
		{
			m_renderTarget = std::unique_ptr<render::rendertarget>( rendertarget::Create(device, width, height) );
		
		}

		camera_rt(device_direct3d* device, const render::viewport& viewport, float nearPlane = 0.1f, float farPlane = 1000.f)
			: camera(viewport, nearPlane, farPlane)
			, m_device(device)
			, m_clearColor(render::color::PURPLE)
			, m_clear(true)
			, m_viewport(0.f, 0.f, static_cast<float>(viewport.Width()), static_cast<float>(viewport.Height()), 0.f, 1.f)
		{
			m_renderTarget = std::unique_ptr<render::rendertarget>( rendertarget::Create(device, static_cast<unsigned int>(viewport.Width()), static_cast<unsigned int>(viewport.Height())) );
		}

		void Capture(std::function<void(void)> renderFunction)
		{		
			auto context = m_device->GetImmediateContext();

			auto* renderTargetView = m_renderTarget->GetRenderTargetView();
			context->OMSetRenderTargets(1, renderTargetView, nullptr );
		
			if ( m_clear )
			{
				context->ClearRenderTargetView(*renderTargetView, m_clearColor);
			}

			auto vp = m_device->GetViewport();
		
			m_device->SetViewport(m_viewport);
			m_device->SetViewMatrix(View());
			m_device->SetProjectionMatrix(Projection());	

		
			renderFunction();

			ID3D11RenderTargetView* nullRenderTargetView = nullptr;
			context->OMSetRenderTargets(1, &nullRenderTargetView, nullptr );

			m_device->SetViewport(vp);
		}

		std::shared_ptr<render::rendertarget> RenderTarget() const { return m_renderTarget; }
		const render::viewport& Viewport() const { return m_viewport; }

		render::color& ClearColor() { return m_clearColor; }
		bool& Clear() { return m_clear; }

	protected:

		render::viewport m_viewport;

		std::shared_ptr<render::rendertarget> m_renderTarget;
		render::color m_clearColor;
		bool m_clear;

		device_direct3d* m_device;
	};

} // render