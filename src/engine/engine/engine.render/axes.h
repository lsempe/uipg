#pragma once

#include "device_direct3d.h"
#include "vertex_types.h"
#include "engine.math/vector.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "CommonStates.h"

#include "cone.h"

namespace axis_shader
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

class axes
{
public:

	cone m_cone;

	axes(device_direct3d* device)
		: m_device(device)
		, m_cone(device)
		, m_indexCount(0)
	{
		m_vertexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_indexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_constantBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_psConstantBuffer = std::unique_ptr<buffer>( new buffer(device) );			

		vertex_default vertices[6];
		unsigned short indices[6] = { 0, 1, 2, 3, 4, 5 };
		m_indexCount = sizeof(indices)/sizeof(indices[0]);

		vertices[0] = vertex_default(math::vector3::Zero, color::RED);
		vertices[1] = vertex_default(math::vector3::UnitX * 2.f, color::RED);

		vertices[2] = vertex_default(math::vector3::Zero, color::GREEN);
		vertices[3] = vertex_default(math::vector3::UnitY * 2.f, color::GREEN);

		vertices[4] = vertex_default(math::vector3::Zero, color::BLUE);
		vertices[5] = vertex_default(math::vector3::UnitZ * 2.f, color::BLUE);
				
		m_vertexBuffer->Data(eBufferUsage::Vertex, vertices, sizeof(vertices));
		m_indexBuffer->Data(eBufferUsage::Index, indices, sizeof(indices));
		
		
		
		m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(m_device->GetDevice()) );
		m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(m_device->GetDevice()) );

		if ( !m_vertexShader->Load(axis_shader::VS_ShadersByteCode[0].code, axis_shader::VS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		if (!m_pixelShader->Load(axis_shader::PS_ShadersByteCode[0].code, axis_shader::PS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		ID3D11InputLayout** inputLayout = &m_inputLayout;
		m_device->GetDevice()->CreateInputLayout(DefaultVertexDesc,
			sizeof(DefaultVertexDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC), axis_shader::VS_ShadersByteCode[0].code, axis_shader::VS_ShadersByteCode[0].length, 
			inputLayout);

	}

	~axes()
	{
		m_inputLayout->Release();		
	}

	void Draw(const math::matrix& world, const math::matrix& view, const math::matrix& projection, std::shared_ptr<texture> texture = nullptr)
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
												render::color::WHITE
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

		m_pixelShader->SetConstantBuffer(m_psConstantBuffer.get());

		m_vertexShader->Set();
		m_pixelShader->Set();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		
		DirectX::CommonStates states(m_device->GetDevice());
		
		
		context->OMSetDepthStencilState(states.DepthDefault(), 0);
		context->RSSetState(states.CullNone());
		//context->RSSetState(states.Wireframe());
	
		context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
		context->DrawIndexed(m_indexCount, 0, 0);

		math::matrix coneTransform = math::matrix::Identity;
		coneTransform =  math::matrix::CreateRotationZ(math::HalfPi);
		coneTransform.Scale(0.25f);
		coneTransform.Translate(math::vector3::UnitX * 2.f);		
		m_cone.Draw( coneTransform * world, view, projection, nullptr, render::color::RED);

		coneTransform = math::matrix::Identity;
		coneTransform.Scale(0.25f);
		coneTransform.Translate(math::vector3::UnitY * 2.f);		
		m_cone.Draw( coneTransform * world, view, projection, nullptr, render::color::GREEN);

		coneTransform = math::matrix::CreateRotationX(-math::Pi/2) * math::matrix::CreateRotationZ(-math::Pi/2);
		coneTransform.Scale(0.25f);
		coneTransform.Translate(math::vector3::UnitZ * 2.f);		
		m_cone.Draw( coneTransform * world, view, projection, nullptr, render::color::BLUE);

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