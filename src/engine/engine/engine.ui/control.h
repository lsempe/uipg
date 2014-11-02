#pragma once

#include <memory>

#include "engine.math/vector.h"
#include "engine.math/rectangle.h"

#include "engine.render/platform.h"
#include "engine.render/color.h"
#include "engine.render/device.h"
#include "engine.core/input.h"
#include "engine.core/event_handler.h"
#include "engine.ui/core.h"
#include "engine.render/camera.h"

namespace ui
{

	class core_ui;

	class control
	{
	public:

		control(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch)
			: m_core(core)
			, m_font(font)
			, m_spriteBatch(spriteBatch)
			, m_foregroundColor(render::color::WHITE)
			, m_backgroundColor(render::color::BLACK)
			, m_alpha(1.f)
			, m_visible(true)
			, m_enabled(true)
			, m_focused(false)
			, m_mouseOver(false)
		{
			if ( m_spriteBatch == nullptr )
			{
				m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(core->GetDevice()->GetImmediateContext()));
			}

			SetViewport(m_core->GetDevice()->GetViewport());
		}

		virtual void Create(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch)
		{
			m_core = core;
			m_font = font;
			m_spriteBatch = spriteBatch;

			if ( m_spriteBatch == nullptr )
			{
				m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(core->GetDevice()->GetImmediateContext()));
			}

			SetViewport(m_core->GetDevice()->GetViewport());
		}

		virtual void Update(float deltaTime);		
		virtual bool HandleInput(float deltaTime, const input::input_state& inputState);
	
		void Draw(const render::camera& camera);	

		enum eUnit
		{
			Pixels,
			Percent
		};

		const render::viewport& Viewport() const;
	
		float& Alpha() { return m_alpha; }

		const math::vector2 Position() const { return m_rectangle.Position(); }
		const math::vector2 Size() const { return m_rectangle.Size(); }
	
		void SetPosition(const math::vector2& position, eUnit unit);	
		void SetSize(const math::vector2& size, eUnit unit);	
		void SetExtents(const math::vector4& extents, eUnit unit);

		const render::color& ForegroundColor() const { return m_foregroundColor; }
		const render::color& BackgroundColor() const { return m_backgroundColor; }

		void SetForegroundColor(const render::color& color) { m_foregroundColor = color; }
		void SetBackgroundColor(const render::color& color) { m_backgroundColor = color; }

		void SetViewport(const render::viewport& viewport)
		{
			m_viewport = viewport;
			Refresh();
		}

		virtual void Refresh() {}

		const math::rectangle& Rectangle() const { return m_rectangle; }

		typedef std::function<void(const control&)> custom_draw_function;

		void SetCustomDrawFunction(custom_draw_function customDrawFunction)
		{
			m_customDrawFunction = customDrawFunction;
		}

		event_handler<control&>& OnFocusReceived() { return m_onFocusReceived; }
		event_handler<control&>& OnFocusLost() { return m_onFocusLost; }
		event_handler<control&>& OnMouseEnter() { return m_onMouseEnter; }
		event_handler<control&>& OnMouseLeave() { return m_onMouseLeave; }
		event_handler<control&>& OnMousePressed() { return m_onMousePressed; }
		event_handler<control&>& OnMouseReleased() { return m_onMouseReleased; }

		void SetSpriteBatch(std::shared_ptr<render::platform::sprite_batch> spriteBatch) { m_spriteBatch = spriteBatch; }

		void SetFont(std::shared_ptr<render::platform::font>  font)
		{
			m_font = font;
		}
	
		const std::shared_ptr<render::platform::font> Font() const { return m_font; }

		std::shared_ptr<render::platform::sprite_batch> SpriteBatch() { return m_spriteBatch; }
		const std::shared_ptr<render::platform::sprite_batch> SpriteBatch() const { return m_spriteBatch; }
	
		std::shared_ptr<core_ui>& Core() { return m_core; }
		const std::shared_ptr<core_ui>& Core() const { return m_core; }
	
	protected:

		math::rectangle m_rectangle;

		render::color m_foregroundColor;
		render::color m_backgroundColor;

		render::viewport m_viewport;

		float m_alpha;
	
		bool m_visible;
		bool m_enabled;
		bool m_focused;
		bool m_mouseOver;
	
		std::shared_ptr<core_ui> m_core;
		std::shared_ptr<render::platform::font> m_font;
		std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;
		custom_draw_function m_customDrawFunction;

		event_handler<control&> m_onFocusReceived;
		event_handler<control&> m_onFocusLost;
		event_handler<control&> m_onMouseEnter;
		event_handler<control&> m_onMouseLeave;
		event_handler<control&> m_onMousePressed;
		event_handler<control&> m_onMouseReleased;
		event_handler<control&> m_onAccept;
		event_handler<control&> m_onCancel;

		virtual void InternalDraw(const render::camera& ) 
		{
		}

	

	};

} // ui