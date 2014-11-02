#pragma once

#include "shader.h"
#include "rendertarget.h"

namespace render
{

	 // Points to a precompiled vertex or pixel shader program.
	struct ShaderByteCode
	{
		void const* code;
		size_t length;
	};

	namespace 
	{
		#include "shaders/compiled/ovr_DistortionCorrectionPixelShader.inc"
		#include "shaders/compiled/ovr_OVR_VertexShader.inc"
	}

	const ShaderByteCode OVRShadersByteCode[] =
	{
		{ ovr_OVR_VertexShader,					sizeof(ovr_OVR_VertexShader)   },
		{ ovr_DistortionCorrectionPixelShader,	sizeof(ovr_DistortionCorrectionPixelShader)        },
	};

	class ovr
	{
	public:

		ovr(device_direct3d* device)		
		{
			m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(device->GetDevice()) );
			m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(device->GetDevice()) );

			m_vertexShader->Load(OVRShadersByteCode[0].code, OVRShadersByteCode[0].length);
			m_pixelShader->Load(OVRShadersByteCode[1].code, OVRShadersByteCode[1].length);

			m_renderTarget = std::shared_ptr<rendertarget>(rendertarget::Create(device, static_cast<unsigned int>(device->GetViewport().Width()), static_cast<unsigned int>(device->GetViewport().Height())));
		}

		std::shared_ptr<vertex_shader> GetVertexShader() { return m_vertexShader; }
		std::shared_ptr<pixel_shader> GetPixelShader() { return m_pixelShader; }
		std::shared_ptr<rendertarget> GetRenderTarget() { return m_renderTarget; }

	private:

		std::shared_ptr<vertex_shader> m_vertexShader;
		std::shared_ptr<pixel_shader> m_pixelShader;
		std::shared_ptr<rendertarget> m_renderTarget;

	};

} // render