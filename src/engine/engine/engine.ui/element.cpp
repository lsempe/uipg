#include "element.h"
#include "core.h"

namespace ui
{

	void element::Create(std::shared_ptr<core_ui> core)
	{
		m_core = core;

		if ( m_controller != nullptr )
		{
			m_controller->Create();
		}

		if ( m_view != nullptr )
		{
			m_view->Create();
		}
	}

	void element::Update(float deltaTime)
	{
		if ( m_controller != nullptr )
		{
			m_controller->Update(deltaTime);
		}

		if ( m_view != nullptr )
		{
			m_view->Update(deltaTime);
		}
	}

	void element::Draw()
	{
		if ( m_view != nullptr )
		{
			m_view->Draw();
		}
	}

} // ui