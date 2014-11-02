#include "label.h"
#include "CommonStates.h"
#include "Effects.h"

namespace ui
{

	label::label(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_alignment(align::Left)
		, m_autoSize(true)
		, m_showBackground(true)
	{
		m_primitiveBatch = std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(core->GetDevice()->GetImmediateContext()));
	}

	void label::Refresh()
	{
		math::vector2 textSize = GetTextSize();
		m_textRectangle = math::rectangle::MakeRectangle(
							m_rectangle.Left(), 
							m_rectangle.Top(), 
							m_rectangle.Left() + textSize.x(), 
							m_rectangle.Top() + textSize.y() );

		
		align a(m_textRectangle, m_rectangle, m_alignment);	
		a.Apply();
	}

	void label::InternalDraw(const render::camera& camera)
	{	
		

		UNREFERENCED(camera);

		m_backgroundColor.A() = m_alpha;
		m_foregroundColor.A() = m_alpha;
		
		if ( m_showBackground )
		{
			m_spriteBatch->Draw(*m_core->GetWhiteTexture()->GetView(), m_autoSize ? m_textRectangle : m_rectangle, nullptr, m_backgroundColor);
		}

		m_font->DrawString(m_spriteBatch.get(), m_text.c_str(), m_textRectangle.Position(), m_foregroundColor, 0.f, math::vector4::Zero, math::vector4::One, DirectX::SpriteEffects_None, 0.f);

		m_spriteBatch->End();


		DirectX::CommonStates states(m_core->GetDevice()->GetDevice());
		m_core->GetDevice()->GetImmediateContext()->RSSetState(states.CullNone());


		std::unique_ptr<DirectX::BasicEffect> basicEffect(new DirectX::BasicEffect(m_core->GetDevice()->GetDevice()));

		
		//basicEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, screenHeight, screenWidth, 0, 0, 1));
		basicEffect->SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
		//basicEffect->SetProjection(camera.Projection());
		basicEffect->SetVertexColorEnabled(true);

		bool drawBounds = false;

		if  (drawBounds)
		{
			void const* shaderByteCode;
			size_t byteCodeLength;

			basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

			ID3D11InputLayout* inputLayout;

			m_core->GetDevice()->GetDevice()->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
									  DirectX::VertexPositionColor::InputElementCount,
									  shaderByteCode, byteCodeLength,
									  &inputLayout);

			basicEffect->Apply(m_core->GetDevice()->GetImmediateContext());
			m_core->GetDevice()->GetImmediateContext()->IASetInputLayout(inputLayout);

			m_primitiveBatch->Begin();
			{
				DirectX::VertexPositionColor v0;
				DirectX::VertexPositionColor v1;
				v0.position = DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Top(), 0.f);
				v0.color = DirectX::XMFLOAT4(1.f,1.f,1.f,1.f);
				v1.color = DirectX::XMFLOAT4(1.f,1.f,1.f,1.f);
				//m_primitiveBatch->DrawLine(v0, v1);


				/// ----
				v0.position = DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Top(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Bottom(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				/// ----
				v0.color = DirectX::XMFLOAT4(1.f,0.f,1.f,1.f);
				v1.color = DirectX::XMFLOAT4(1.f,0.f,1.f,1.f);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Left(), m_rectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Left(), m_rectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Left(), m_rectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Right(), m_rectangle.Top(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Right(), m_rectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Right(), m_rectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Left(), m_rectangle.Bottom(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Right(), m_rectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);


				// ------
				v0.color = DirectX::XMFLOAT4(1.f,0.f,0.f,1.f);
				v1.color = DirectX::XMFLOAT4(1.f,0.f,0.f,1.f);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Left(), m_rectangle.Center().y(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Right(), m_rectangle.Center().y(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);

				v0.position = DirectX::XMFLOAT3(m_rectangle.Center().x(), m_rectangle.Top(), 0.f);
				v1.position = DirectX::XMFLOAT3(m_rectangle.Center().x(), m_rectangle.Bottom(), 0.f);
				m_primitiveBatch->DrawLine(v0, v1);
				// ------

				v0.position = DirectX::XMFLOAT3(720.f, 360.f, 0.f);
				v1.position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
				//m_primitiveBatch->DrawLine( v0, v1);
				/// ----

				DirectX::VertexPositionColor v2,v3;
				v2.color = DirectX::XMFLOAT4(1.f,1.f,1.f,1.f);
				v3.color = DirectX::XMFLOAT4(1.f,1.f,1.f,1.f);

				v0.position =  DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Top(), 0.f);
				v1.position =  DirectX::XMFLOAT3(m_textRectangle.Left(), m_textRectangle.Bottom(), 0.f);
				v2.position =  DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Bottom(), 0.f);
				v3.position =  DirectX::XMFLOAT3(m_textRectangle.Right(), m_textRectangle.Top(), 0.f);
				//m_primitiveBatch->DrawQuad(v0,v1,v2,v3);

				/*p0 = m_viewport.Unproject(math::vector3(-m_textRectangle.Left(), -m_textRectangle.Top(), 0.1f), camera.Projection(), camera.View(), camera.World());
				p1 = m_viewport.Unproject(math::vector3(-m_textRectangle.Left(), -m_textRectangle.Bottom(), 0.1f), camera.Projection(), camera.View(), camera.World());
				v0.position = DirectX::XMFLOAT3(p0.x()* camera.Far(), p0.y()* camera.Far(), p0.z());
				v1.position = DirectX::XMFLOAT3(p1.x()* camera.Far(), p1.y()* camera.Far(), p1.z());
				m_primitiveBatch->DrawLine(v0, v1);

				p0 = m_viewport.Unproject(math::vector3(m_textRectangle.Right(), m_textRectangle.Top(), 0.1f), camera.Projection(), camera.View(), camera.World());
				p1 = m_viewport.Unproject(math::vector3(m_textRectangle.Right(), m_textRectangle.Bottom(), 0.1f), camera.Projection(), camera.View(), camera.World());
				v0.position = DirectX::XMFLOAT3(p0.x()* camera.Far(), p0.y()* camera.Far(), p0.z());
				v1.position = DirectX::XMFLOAT3(p1.x()* camera.Far(), p1.y()* camera.Far(), p1.z());
				m_primitiveBatch->DrawLine(v0, v1);

				p0 = m_viewport.Unproject(math::vector3(m_textRectangle.Left(), m_textRectangle.Bottom(), 0.1f), camera.Projection(), camera.View(), camera.World());
				p1 = m_viewport.Unproject(math::vector3(m_textRectangle.Right(), m_textRectangle.Bottom(), 0.1f), camera.Projection(), camera.View(), camera.World());
				v0.position = DirectX::XMFLOAT3(p0.x()* camera.Far(), p0.y()* camera.Far(), p0.z());
				v1.position = DirectX::XMFLOAT3(p1.x()* camera.Far(), p1.y()* camera.Far(), p1.z());
				m_primitiveBatch->DrawLine(v0, v1);*/
			}
			m_primitiveBatch->End();
		}
		
		m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied());
		

	}

	void label::SetText(const std::wstring text)
	{
		m_text = text;		
		Refresh();		
	}

	math::vector2 label::GetTextSize() const
	{
		return math::vector2(m_font->MeasureString(m_text.c_str()));
	}
}