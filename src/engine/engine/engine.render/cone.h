#pragma once

#include "device_direct3d.h"
#include "vertex_types.h"
#include "engine.math/vector.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "CommonStates.h"

namespace cone_shader
{
	#include "shaders/compiled/vs_standard_main.inc"
	#include "shaders/compiled/ps_solid_main.inc"

	struct ShaderByteCode
	{
		const BYTE * code;
		size_t length;
	};

	const ShaderByteCode VS_ShadersByteCode[] =
	{
		{ &vs_standard_main[0], sizeof(vs_standard_main)   },
	};

	const ShaderByteCode PS_ShadersByteCode[] = 
	{
		{ &ps_solid_main[0],	sizeof(ps_solid_main)        },
	};

}

namespace render
{

class cone
{
public:

	cone(device_direct3d* device)
		: m_device(device)
		, m_indexCount(0)
		, m_transform(math::matrix::Identity)
	{
		m_vertexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_indexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_constantBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_psConstantBuffer = std::unique_ptr<buffer>( new buffer(device) );			

		const int degrees = 15;

		const int numVertices = (360 / degrees) + 3;		
		vertex_default vertices[numVertices];

		const int indexCount = 2 * ((numVertices-1) * 3);
		unsigned short indices[indexCount];
		m_indexCount = indexCount;

		vertices[0] = vertex_default(math::vector3::UnitY, color::WHITE, math::vector2(0.5f, 1.f));
		vertices[numVertices - 1] = vertex_default(-math::vector3::UnitY, color::WHITE, math::vector2(0.5f, 0.f));

		for ( int i = 0, j = 1; i <= 360; i += degrees, ++j )
		{
			const float theta = math::DegreesToRadians(i);
			float x = sinf(theta);
			float z = cosf(theta);

			vertices[j] = vertex_default(math::vector3(x, -1.f, z), color::WHITE, math::vector2((j % 2) == 0 ? 0.f : 1.f, 0.f));
		}

		const int offset = (numVertices-1) * 3;
		for (unsigned short i = 0, j = 0; i < offset; i += 3, ++j )
		{
			indices[i + 0] = 0;
			indices[i + 1] = j + 1;
			indices[i + 2] = j + 2;
			
			indices[i + offset + 0] = (numVertices - 1);
			indices[i + offset + 1] = indices[i + 1];
			indices[i + offset + 2] = indices[i + 2];
		}
		
		m_vertexBuffer->Data(eBufferUsage::Vertex, vertices, numVertices *  sizeof(vertex_default));
		m_indexBuffer->Data(eBufferUsage::Index, indices, m_indexCount * sizeof(unsigned short));
		
		m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(m_device->GetDevice()) );
		m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(m_device->GetDevice()) );

		if ( !m_vertexShader->Load(cone_shader::VS_ShadersByteCode[0].code, cone_shader::VS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		if (!m_pixelShader->Load(cone_shader::PS_ShadersByteCode[0].code, cone_shader::PS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		ID3D11InputLayout** inputLayout = &m_inputLayout;
		m_device->GetDevice()->CreateInputLayout(DefaultVertexDesc,
			sizeof(DefaultVertexDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC), cone_shader::VS_ShadersByteCode[0].code, cone_shader::VS_ShadersByteCode[0].length, 
			inputLayout);

	}

	~cone()
	{
		m_inputLayout->Release();		
	}

	void Draw(const math::matrix& world, const math::matrix& view, const math::matrix& projection, std::shared_ptr<texture> texture = nullptr, const render::color color = color::WHITE)
	{	
		auto context = m_device->GetImmediateContext();

		UINT vertexStride = sizeof(vertex_default);
		UINT vertexOffset = 0;

		context->IASetInputLayout(m_inputLayout);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer->GetBuffer(), &vertexStride, &vertexOffset);
		context->IASetIndexBuffer(m_indexBuffer->GetBuffer(), DXGI_FORMAT_R16_UINT, 0);
		
		struct vsConstantBuffer
		{
			math::matrix view;
			math::matrix projection;
			math::matrix world;
			math::vector4 color;
		};

		vsConstantBuffer constantBufferData = { view,
												projection,
												world,
												color
											};

		m_constantBuffer->Data(eBufferUsage::Constant, &constantBufferData, sizeof(constantBufferData));
		ID3D11Buffer* vertexConstants = m_constantBuffer->GetBuffer();
		context->VSSetConstantBuffers(0, 1, &vertexConstants);

		if ( texture != nullptr )
		{
			ID3D11ShaderResourceView* textures[1] = { *texture->GetView() };
			context->PSSetShaderResources(0, 1, textures);			
		}
		else
		{			
			ID3D11ShaderResourceView* textures[1] = { *m_device->GetWhiteTexture()->GetView() };
			context->PSSetShaderResources(0, 1, textures);
		}

		math::vector4 solidColor = color;
		m_psConstantBuffer->Data(eBufferUsage::Constant, solidColor.AsArray(), sizeof(float)*4);

		//m_pixelShader->SetConstantBuffer(m_psConstantBuffer.get());

		m_vertexShader->Set();
		m_pixelShader->Set();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		DirectX::CommonStates states(m_device->GetDevice());
				
		context->OMSetDepthStencilState(states.DepthDefault(), 0);
		context->RSSetState(states.CullNone());
		//context->RSSetState(states.Wireframe());
		
		auto samplerState = states.LinearWrap();
  		context->PSSetSamplers(0, 1, &samplerState);

		context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
		context->DrawIndexed(m_indexCount, 0, 0);
		
	}

	math::matrix& Transform() { return m_transform; }

	void SetVertexShader(std::shared_ptr<vertex_shader> vertexShader)
	{
		m_vertexShader = vertexShader;
	}

	void SetPixelShader(std::shared_ptr<pixel_shader> pixelShader)
	{
		m_pixelShader = pixelShader;
	}

private:

	math::matrix m_transform;

	std::unique_ptr<buffer> m_vertexBuffer;
	std::unique_ptr<buffer> m_indexBuffer;
	std::unique_ptr<buffer> m_constantBuffer;
	std::unique_ptr<buffer> m_psConstantBuffer;
	
	unsigned int m_indexCount;

	ID3D11InputLayout* m_inputLayout;

	std::shared_ptr<vertex_shader> m_vertexShader;
	std::shared_ptr<pixel_shader> m_pixelShader;

	device_direct3d* m_device;

};


} // render