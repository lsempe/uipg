#pragma once

#include "engine.core/sys.h"
#include "core.h"
#include <memory>

namespace render
{
	class device;
}

namespace ui
{

	class view;
	class controller;

	class element
	{
	public:

		element()
			: m_view(nullptr)
			, m_controller(nullptr)
		{
		}

		virtual ~element() 
		{ 
			m_view = nullptr;
			m_controller = nullptr;
			m_core = nullptr;
		}

		virtual void Create(std::shared_ptr<core_ui> core);

		virtual void Update(float deltaTime);
	
		virtual void Draw();
	
		std::shared_ptr<core_ui> GetCore() const { return m_core; }

	protected:

		std::unique_ptr<view> m_view;
		std::unique_ptr<controller> m_controller;
		std::shared_ptr<core_ui> m_core;

	};

	class view
	{
	public:

		view(element& element)
			: m_element(element)
		{
		}

		virtual ~view() {}

		virtual void Create()
		{
		}

		virtual void Update(float deltaTime)
		{
			UNREFERENCED(deltaTime);
		}

		virtual void Draw()
		{
		}

	protected:

		element& m_element;

	private:

		view& operator = (const view&) = delete;
		view(const view&) = delete;
		view(const view&&) = delete;
	};

	class controller
	{
	public:
		controller(element& element)		
			: m_element(element)
		{
		}

		virtual void Create()
		{
		}

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			UNREFERENCED(deltaTime);
			UNREFERENCED(inputState);
			return false;
		}

		virtual void Update(float deltaTime)
		{
			UNREFERENCED(deltaTime);
		}

	protected:

		element& m_element;

	private:

		controller(const controller&) = delete;
		controller(const controller&&) = delete;
		controller& operator = (const controller&) = delete;
	};

} // ui