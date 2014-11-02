#pragma once

#include "control.h"
#include "engine.ui/align.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

namespace ui
{

	class label : public control
	{
	public:

		label(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch);

		virtual void Refresh();

		virtual void InternalDraw(const render::camera& camera);

		void SetText(const std::wstring text);

		math::vector2 GetTextSize() const;

		void SetAlignment(align::eAlignmentFlags alignment) { m_alignment = alignment; Refresh(); }

		bool &AutoSize() { return m_autoSize; }
		bool &ShowBackground() { return m_showBackground; }

	private:
	
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
		std::wstring m_text;

		math::rectangle m_textRectangle;

		align::eAlignmentFlags m_alignment;

		bool m_autoSize;	
		bool m_showBackground;
	
	};


} // ui