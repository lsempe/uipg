#pragma once

#include "device_direct3d.h"

namespace render
{

	class buffer;

	enum eShaderStage
	{
		Shader_Vertex   = 0,
		Shader_Fragment = 2,
		Shader_Pixel    = 2,
		Shader_Count    = 3,
	};

	template <eShaderStage, typename ShaderType>
	class shader
	{
	public:

		shader(ID3D11Device* device)
			: m_shader(nullptr)
			, m_device(device)
		{}

		~shader()
		{
			if ( m_shader != nullptr )
			{
				m_shader->Release();
			}
		}

		bool Load(const void* data, size_t size);
		void Set();
		void SetConstantBuffer(buffer* constantBuffer, int index = 0);
	
		ShaderType* Get() { return m_shader; }

	protected:

		ShaderType* m_shader;
		ID3D11Device* m_device;
	};

	typedef shader<Shader_Vertex,  ID3D11VertexShader> vertex_shader;
	typedef shader<Shader_Fragment, ID3D11PixelShader> pixel_shader;

} // render