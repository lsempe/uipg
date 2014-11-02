#pragma once

#include "device_direct3d.h"
#include "vertex_types.h"
#include "engine.math/vector.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "CommonStates.h"

namespace cube_shader
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

class cube
{
public:

	cube(device_direct3d* device)
		: m_device(device)
	{
		m_transform = math::matrix::Identity;

		m_vertexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_indexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_constantBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_psConstantBuffer = std::unique_ptr<buffer>( new buffer(device) );

		const math::vector3 normals[] = {
			math::vector3(0, 0, 1),
			math::vector3(0, 0, -1),
			math::vector3(1, 0, 0),
			math::vector3(-1, 0, 0),
			math::vector3(0, 1, 0),
			math::vector3(0, -1, 0),
		};
		const math::vector2 texcoords[] = {
			math::vector2(1, 0),
			math::vector2(1, 1),
			math::vector2(0, 1),
			math::vector2(0, 0)
		};

		vertex_default vertices[6 * 4];
		unsigned short indices[6 * 6];

		for (unsigned short i = 0; i < 6; ++i )
		{
			math::vector3 normal = normals[i];

			math::vector3 basis = (i >= 4) ? math::vector3::UnitZ : math::vector3::UnitY;
			
			math::vector3 side1 = normal.Cross( basis );
			math::vector3 side2 = normal.Cross( side1);

			const unsigned short vertexIndex0 = i * 4 + 0;
			const unsigned short vertexIndex1 = i * 4 + 1;
			const unsigned short vertexIndex2 = i * 4 + 2;
			const unsigned short vertexIndex3 = i * 4 + 3;

			vertices[vertexIndex0] = vertex_default( normal - side1 - side2,	color::WHITE, texcoords[0]);
			vertices[vertexIndex1] = vertex_default( normal - side1 + side2,	color::WHITE, texcoords[1]);
			vertices[vertexIndex2] = vertex_default( normal + side1 + side2,	color::WHITE, texcoords[2]);
			vertices[vertexIndex3] = vertex_default( normal + side1 - side2,	color::WHITE, texcoords[3]);

			indices[i* 6 + 0] = vertexIndex0;
			indices[i* 6 + 1] = vertexIndex1;
			indices[i* 6 + 2] = vertexIndex2;
			indices[i* 6 + 3] = vertexIndex0;
			indices[i* 6 + 4] = vertexIndex2;
			indices[i* 6 + 5] = vertexIndex3;
		}
	
		m_vertexBuffer->Data(eBufferUsage::Vertex, vertices, sizeof(vertices));
		m_indexBuffer->Data(eBufferUsage::Index, indices, sizeof(indices));
		
		m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(m_device->GetDevice()) );
		m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(m_device->GetDevice()) );

		if ( !m_vertexShader->Load(cube_shader::VS_ShadersByteCode[0].code, cube_shader::VS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		if (!m_pixelShader->Load(cube_shader::PS_ShadersByteCode[0].code, cube_shader::PS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		ID3D11InputLayout** inputLayout = &m_inputLayout;
		m_device->GetDevice()->CreateInputLayout(DefaultVertexDesc,
			sizeof(DefaultVertexDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC), cube_shader::VS_ShadersByteCode[0].code, cube_shader::VS_ShadersByteCode[0].length, 
			inputLayout);

	}

	~cube()
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
												world * m_transform,
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
		context->RSSetState(states.CullNone());
	//	context->RSSetState(states.Wireframe());
		auto samplerState = states.LinearWrap();
  		context->PSSetSamplers(0, 1, &samplerState);
		context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
		context->DrawIndexed(36, 0, 0);
		
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
	
	ID3D11InputLayout* m_inputLayout;

	std::shared_ptr<vertex_shader> m_vertexShader;
	std::shared_ptr<pixel_shader> m_pixelShader;

	device_direct3d* m_device;

};


} // render