#pragma once

#include <vector>
#include <memory>

#include "engine.core/singleton.h"
#include "engine.render/platform.h"
#include "engine.ui/core.h"
#include "engine.ui/control.h"

namespace ui
{

	class factory 
	{
	public:
		template <class T>
		static std::shared_ptr<T> Create(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		{
			return std::shared_ptr<T>( new T(core, font, spriteBatch) );
		}
	};

	class container : public control
	{
	public:

		container(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font)
			: control(core, font, nullptr)
		{}

		template <typename T>
		std::shared_ptr<T> Add()
		{
			auto ctrl = factory::Create<T>(m_core, m_font, m_spriteBatch);
		
			ctrl->SetViewport(m_viewport);
			ctrl->SetPosition(math::vector2::Zero, ui::control::Pixels);
		
			m_controls.push_back(ctrl);
		
			return ctrl;
		}
	
		virtual bool HandleInput(float deltaTime, const input::input_state& inputState);
		virtual void Update(float deltaTime);
		virtual void Draw(const render::camera& camera);	
	
		virtual void Refresh()
		{		
		}

		virtual render::viewport Viewport() const
		{
			return m_viewport;
		}

	protected:

		std::vector<std::shared_ptr<control>> m_controls;
	
	};


} // ui