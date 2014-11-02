#pragma once

#include "device_direct3d.h"
#include "vertex_types.h"
#include "engine.math/vector.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "CommonStates.h"

#include <vector>

namespace ellipsoid_shader
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

class ellipsoid
{
public:

	ellipsoid(device_direct3d* device)
		: m_device(device)
	{
		m_vertexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_indexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_constantBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_psConstantBuffer = std::unique_ptr<buffer>( new buffer(device) );

	
		std::vector<vertex_default> vertices;

		const float radius = 0.5f;

		const unsigned short verticalSegments = 20;
		const unsigned short horizontalSegments = verticalSegments * 2;		

		for (size_t i = 0; i <= verticalSegments; ++i)
		{
			const float v = 1 - static_cast<float>(i) / verticalSegments;

			const float latitude = (i * math::Pi / verticalSegments) - math::HalfPi;

			const float dy = sinf(latitude);
			const float dxz = cosf(latitude);

			// Create a single ring of vertices at this latitude.
			for (size_t j = 0; j <= horizontalSegments; ++j)
			{
				const float u = static_cast<float>(j) / horizontalSegments;

				const float longitude = j * math::TwoPi / horizontalSegments;
				
				const float dx = sinf(longitude) * dxz;
				const float dz = cosf(longitude) * dxz;            

				math::vector3 normal = math::vector3(dx, dy, dz);            
				math::vector2 textureCoordinate = math::vector2(u, v);

				vertex_default v(normal, render::color::WHITE, textureCoordinate, normal * radius);
				vertices.push_back(v);
			}
		}

		std::vector<unsigned short> indices;
		
		// Fill the index buffer with triangles joining each pair of latitude rings.
		unsigned short stride = horizontalSegments + 1;
		for (unsigned short i = 0; i < verticalSegments; ++i)
		{
			for (unsigned short j = 0; j <= horizontalSegments; ++j)
			{
				const unsigned short i1 = i + 1;
				const unsigned short j1 = (j + 1) % stride;

				const unsigned short ip = i * stride;
				const unsigned short np = i1 * stride;

				indices.push_back(ip + j);
				indices.push_back(np + j);
				indices.push_back(ip + j1);

				indices.push_back(ip + j1);
				indices.push_back(np + j);
				indices.push_back(np + j1);
			}
		}
		m_indexCount = indices.size();

		/*for ( int j = 0; j <= 90 - (int)angle; j += (int)angle )
		{
			for ( int i = 0; i < 360; i += (int)angle )
			{
				float x = (axes.x() * cosAlpha * cosBeta - axes.y() * sinAlpha * sinBeta);
				float z = (axes.x() * cosAlpha * sinBeta + axes.y() * sinAlpha * cosBeta);
				float y = cosf((float)j);

				math::vector3 p = math::vector3(x, y, z);

				vertices.push_back(vertex_default(p));

				x = (axes.x() * cosAlpha * cosBeta - axes.y() * sinAlpha * sinBeta);
				z = (axes.x() * cosAlpha * sinBeta + axes.y() * sinAlpha * cosBeta);
				y = cosf((float)j);

				p = math::vector3(x, y, z);

				vertices.push_back(vertex_default(p));

				x = (axes.x() * cosAlpha * cosBeta - axes.y() * sinAlpha * sinBeta);
				z = (axes.x() * cosAlpha * sinBeta + axes.y() * sinAlpha * cosBeta);
				y = cosf((float)j);

				p = math::vector3(x, y, z);

			}
		}
*/
		
		/*std::vector<unsigned short> indices;
		for ( 

*/
		
	const void* data = vertices.data();
	
	m_vertexBuffer->Data(eBufferUsage::Vertex, vertices.data(), vertices.size() * sizeof(vertex_default));
		m_indexBuffer->Data(eBufferUsage::Index, indices.data(), indices.size() * sizeof(unsigned short));
		
		m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(m_device->GetDevice()) );
		m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(m_device->GetDevice()) );

		if ( !m_vertexShader->Load(ellipsoid_shader::VS_ShadersByteCode[0].code, ellipsoid_shader::VS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		if (!m_pixelShader->Load(ellipsoid_shader::PS_ShadersByteCode[0].code, ellipsoid_shader::PS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		ID3D11InputLayout** inputLayout = &m_inputLayout;
		m_device->GetDevice()->CreateInputLayout(DefaultVertexDesc,
			sizeof(DefaultVertexDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC), ellipsoid_shader::VS_ShadersByteCode[0].code, ellipsoid_shader::VS_ShadersByteCode[0].length, 
			inputLayout);

	}

	~ellipsoid()
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
		};

		vsConstantBuffer constantBufferData = { view,
												projection,
												world
											};

		m_constantBuffer->Data(eBufferUsage::Constant, &constantBufferData, sizeof(constantBufferData));
		ID3D11Buffer* vertexConstants = m_constantBuffer->GetBuffer();
		context->VSSetConstantBuffers(0, 1, &vertexConstants);

		if ( texture != nullptr )
		{
			ID3D11ShaderResourceView* textures[1] = { *texture->GetView() };
			context->PSSetShaderResources(0, 1, textures);
		}
		m_pixelShader->SetConstantBuffer(m_psConstantBuffer.get());

		m_vertexShader->Set();
		m_pixelShader->Set();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		DirectX::CommonStates states(m_device->GetDevice());
		
		
		context->RSSetState(states.CullClockwise());
		
		if ( m_wireframe )
			context->RSSetState(states.Wireframe());

		 auto samplerState = states.LinearWrap();
   		 context->PSSetSamplers(0, 1, &samplerState);

		context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
		context->DrawIndexed((UINT)m_indexCount, 0, 0);
		//context->Draw((UINT)m_vertexBuffer->GetSize(), 0);
		
	}
	bool m_wireframe;
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

	size_t m_indexCount;

	std::shared_ptr<vertex_shader> m_vertexShader;
	std::shared_ptr<pixel_shader> m_pixelShader;

	device_direct3d* m_device;

};


} // render