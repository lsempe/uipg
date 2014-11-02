#include "shader.h"
#include "buffer.h"

namespace render
{

	template <> bool shader<Shader_Vertex, ID3D11VertexShader>::Load(const void* data, size_t size)
	{
		return SUCCEEDED(m_device->CreateVertexShader(data, size, nullptr, &m_shader));
	}

	template <> bool shader<Shader_Pixel, ID3D11PixelShader>::Load(const void* data, size_t size)
	{
		return SUCCEEDED(m_device->CreatePixelShader(data, size, nullptr, &m_shader));
	}

	template <> void shader<Shader_Vertex, ID3D11VertexShader>::Set()
	{
		ID3D11DeviceContext* context = nullptr;
		m_device->GetImmediateContext(&context);
		context->VSSetShader(m_shader, nullptr, 0);
	}

	template <> void shader<Shader_Pixel, ID3D11PixelShader>::Set()
	{
		ID3D11DeviceContext* context = nullptr;
		m_device->GetImmediateContext(&context);
		context->PSSetShader(m_shader, nullptr, 0);
	}

	template <> void shader<Shader_Vertex, ID3D11VertexShader>::SetConstantBuffer(buffer* constantBuffer, int index /*=0*/)
	{
		ID3D11DeviceContext* context = nullptr;
		m_device->GetImmediateContext(&context);
		ID3D11Buffer*  b = constantBuffer->GetBuffer();
		context->VSSetConstantBuffers(index, 1, &b);
	}

	template <> void shader<Shader_Pixel, ID3D11PixelShader>::SetConstantBuffer(buffer* constantBuffer, int index /*=0*/)
	{
		ID3D11DeviceContext* context = nullptr;
		m_device->GetImmediateContext(&context);
		ID3D11Buffer*  b = constantBuffer->GetBuffer();
		context->PSSetConstantBuffers(index, 1, &b);
	}

} // render