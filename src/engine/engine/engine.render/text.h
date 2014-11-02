#pragma once

#include <memory>

#include "rendertarget.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

namespace render
{

class text
{
public:

	text(device_direct3d* device, const std::wstring text, std::shared_ptr<DirectX::SpriteFont> font, const math::vector3& position)
		: m_device(device)
		, m_font(font)
		, m_text(text)
		, m_position(position)		
		, m_viewAlign(false)		
		, m_normal(math::vector3::UnitZ)
		, m_color(render::color::WHITE)
	{
		m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>(new DirectX::SpriteBatch(device->GetImmediateContext()));
		
		math::vector2 size = static_cast<math::vector2>( m_font->MeasureString(text.c_str()) );
		unsigned int size_pow2 = static_cast<unsigned int>( math::NearestPowerOfTwo<size_t>(static_cast<size_t>(size.x())) );

		m_textureCoordinates.x() = 1.f;
		m_textureCoordinates.y() = 1.f;

		m_quad = std::unique_ptr<quad>(new quad(device, m_textureCoordinates.x() , m_textureCoordinates.y()));

		m_renderTarget = std::shared_ptr<rendertarget>( render::rendertarget::Create(device, size.x(), size.y()) ); //std::shared_ptr<rendertarget>(new rendertarget(device, size_pow2, size_pow2));
	}

	void Draw(const math::matrix& view, const math::matrix& projection)
	{
		if ( m_renderTarget != nullptr )
		{
			m_device->SetRenderTarget(m_renderTarget);
		}
		
		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
		{
			m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), math::vector2::Zero, m_color, 0);
		}
		m_spriteBatch->End();

		if ( m_renderTarget != nullptr )
		{
			m_device->ResetRenderTarget();
		}

		static int saved = 0;
		if ( saved == 0 )
		{
			m_renderTarget->SaveAsDDS(L"f:\\test.dds");
			saved = 1;
		}
				
		math::matrix transform = math::matrix::Identity;
		math::vector3 up = math::vector3::UnitY;
		math::vector3 right = math::vector3::UnitX;
		math::vector3 forward = -math::vector3::UnitZ;

		m_quad->ViewAlign() = m_viewAlign;
		if ( m_viewAlign )
		{
			transform = math::matrix::CreateTranslation(m_position);
		}
		else
		{
			forward = m_normal;
			forward.Normalize();
			right = forward.Cross(math::vector3::UnitY);
			up = right.Cross(forward);
			transform = math::matrix::Create(right, up, forward, m_position);
		}

		
		m_quad->Draw(transform, view, projection, m_renderTarget);
	}

	std::shared_ptr<rendertarget> RenderTarget() { return m_renderTarget; }
	const math::vector2& TextureCoordinates() { return m_textureCoordinates; }

	bool& ViewAlign() { return m_viewAlign; }
	math::vector3& Position() { return m_position; }
	math::vector3& Normal() { return m_normal; }
	render::color& Color() { return m_color; }

private:

	std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;
	std::shared_ptr<DirectX::SpriteFont> m_font;
	std::shared_ptr<rendertarget> m_renderTarget;
	std::wstring m_text;

	std::unique_ptr<render::quad> m_quad;

	bool m_viewAlign;
	render::color m_color;

	math::vector3 m_position;
	math::vector3 m_normal;

	device_direct3d* m_device;
	math::vector2 m_textureCoordinates;
};

} // render