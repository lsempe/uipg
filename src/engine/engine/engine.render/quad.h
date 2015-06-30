#pragma once

#include "device_direct3d.h"
#include "vertex_types.h"
#include "engine.math/vector.h"
#include "buffer.h"
#include "shader.h"
#include "texture.h"
#include "CommonStates.h"
#include "renderstatehelper.h"
#include "engine.math/intersection.h"

namespace 
{
	#include "shaders/compiled/vs_standard_main.inc"
	#include "shaders/compiled/ps_solid_main.inc"
}

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

namespace render
{

class quad
{
public:

	quad(device_direct3d* device, float u = 1.f, float v = 1.f)
		: m_device(device)
		, m_viewAlign(false)
		, m_pivot(math::vector3::Zero)
		, m_rasterizerState(eRasterizerState::CullNone)
		, m_blendState(render::eBlendState::NonPremultiplied)
		, m_depthStencilState(eDepthStencilState::Default)
		, m_transform(math::matrix::Identity)
		, m_latestTransform(math::matrix::Identity)
	{
		m_vertexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_indexBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_constantBuffer = std::unique_ptr<buffer>( new buffer(device) );
		m_psConstantBuffer = std::unique_ptr<buffer>( new buffer(device) );		

		const unsigned short indices[] = {	0, 1, 2, 0, 2, 3 };
	
		math::vector3 faceNormal(0, 0, -1);
		math::vector3 right = faceNormal.Cross( math::vector3::UnitY );
		math::vector3 up = faceNormal.Cross( right );
		
		const vertex_default vertices_gen[] =
		{	
			vertex_default( right + up, color::WHITE, math::vector2(u, 0.f)), 
			vertex_default( right - up, color::WHITE, math::vector2(u, v)),
			vertex_default(-right - up, color::WHITE, math::vector2(0.f, v)), 
			vertex_default(-right + up, color::WHITE, math::vector2(0.f, 0.f)), 
		};

		m_vertexBuffer->Data(eBufferUsage::Vertex, vertices_gen, sizeof(vertices_gen));
		m_indexBuffer->Data(eBufferUsage::Index, indices, sizeof(indices));
		
		m_vertexShader = std::shared_ptr<vertex_shader>( new vertex_shader(m_device->GetDevice()) );
		m_pixelShader = std::shared_ptr<pixel_shader>( new pixel_shader(m_device->GetDevice()) );

		if ( !m_vertexShader->Load(VS_ShadersByteCode[0].code, VS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		if (!m_pixelShader->Load(PS_ShadersByteCode[0].code, PS_ShadersByteCode[0].length) )
		{
			assert(0);
		}

		struct vsConstantBuffer
		{
			math::matrix view;
			math::matrix projection;
			math::matrix world;
		};

		const float color[] = {0.f, 1.f, 0.f, 1.f};
		m_psConstantBuffer->Data(eBufferUsage::Constant, color, sizeof(color));

		ID3D11InputLayout** inputLayout = &m_inputLayout;
		m_device->GetDevice()->CreateInputLayout(DefaultVertexDesc,
			sizeof(DefaultVertexDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC), VS_ShadersByteCode[0].code, VS_ShadersByteCode[0].length, 
			inputLayout);

	}

	~quad()
	{
		m_inputLayout->Release();
	}

	bool RayIntersect(const math::vector3& start, const math::vector3& end, math::vector3& outContactPoint)
	{
		math::ray r;
		r.position = start;
		r.direction = (end - start);
		r.length = r.direction.Length();
		r.direction.Normalize();
		math::vector3 intersectionPoint;

		math::vector3 faceNormal(0, 0, -1);
		math::vector3 right = faceNormal.Cross(math::vector3::UnitY);
		math::vector3 up = faceNormal.Cross(right);

		const math::vector3 vertices[] =
		{
			right + up,
			right - up,
			-right - up,
			-right + up
		};

		math::vector3 p0 = vertices[0] * m_latestTransform;
		math::vector3 p1 = vertices[1] * m_latestTransform;
		math::vector3 p2 = vertices[2] * m_latestTransform;
		math::vector3 p3 = vertices[3] * m_latestTransform;
		
		return (math::intersection::RayQuad(r, p0, p1, p2, p3, intersectionPoint));





	}

	void Draw(const math::matrix& world, const math::matrix& view, const math::matrix& projection, std::shared_ptr<texture> texture = nullptr, const render::color& color = render::color::WHITE)
	{	
		auto context = m_device->GetImmediateContext();

		UINT vertexStride = sizeof(vertex_default);
		UINT vertexOffset = 0;

		context->IASetInputLayout(m_inputLayout);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer->GetBuffer(), &vertexStride, &vertexOffset);
		context->IASetIndexBuffer(m_indexBuffer->GetBuffer(), DXGI_FORMAT_R16_UINT, 0);
		
		math::matrix pivotTransform = math::matrix::CreateTranslation(m_pivot);
		math::matrix transform = pivotTransform * world;
		if ( m_viewAlign )
		{
			math::matrix inverseView = math::matrix::Invert(view);
			math::vector3 v = inverseView.Translation() - world.Translation();
			v.Normalize();
			
			math::vector3 r = v.Cross(view.Up());

			transform = math::matrix::Create(r, view.Up(), v, world.Translation());			
		}

		struct vsConstantBuffer
		{
			math::matrix view;
			math::matrix projection;
			math::matrix world;
			math::vector4 color;
		};

		vsConstantBuffer constantBufferData = { view,
												projection,
												transform * m_transform, 
												color
											};

		m_latestTransform = m_transform * transform;

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
			context->PSSetShaderResources(0, 0, nullptr);
		}

		m_pixelShader->SetConstantBuffer(m_psConstantBuffer.get());

		m_vertexShader->Set();
		m_pixelShader->Set();

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		renderstatehelper renderState(m_device);
		
		auto samplerState = renderState.SamplerState(eSamplerState::LinearWrap);
		context->PSSetSamplers(0, 1, reinterpret_cast<ID3D11SamplerState* const*>(&samplerState));
				
		context->OMSetDepthStencilState(renderState.DepthStencilState(m_depthStencilState), 0);
		context->RSSetState(renderState.RasterizerState(m_rasterizerState));
		context->OMSetBlendState(renderState.BlendState(m_blendState), nullptr, 0xffffffff);
		
		context->DrawIndexed(6, 0, 0);
		
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

	eBlendState& BlendState() { return m_blendState; }
	eRasterizerState& RasterizerState() { return m_rasterizerState; }
	eDepthStencilState& DepthStencilState() { return m_depthStencilState; }

	bool& ViewAlign() { return m_viewAlign; }

	math::vector3& Pivot() { return m_pivot; }

private:

	math::matrix m_latestTransform;

	math::matrix m_transform;
	math::vector3 m_pivot;

	std::unique_ptr<buffer> m_vertexBuffer;
	std::unique_ptr<buffer> m_indexBuffer;
	std::unique_ptr<buffer> m_constantBuffer;
	std::unique_ptr<buffer> m_psConstantBuffer;
	
	ID3D11InputLayout* m_inputLayout;

	std::shared_ptr<vertex_shader> m_vertexShader;
	std::shared_ptr<pixel_shader> m_pixelShader;

	eBlendState m_blendState;
	eRasterizerState m_rasterizerState;
	eDepthStencilState m_depthStencilState;

	bool m_viewAlign;

	device_direct3d* m_device;

};


} // render