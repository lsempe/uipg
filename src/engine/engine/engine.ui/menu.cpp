#include "menu.h"
#include "engine.core/event_handler.h"
#include "engine.ui/label.h"

namespace ui
{
	menu::menu(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch)
		: control(core, font, spriteBatch)
		, m_currentItem(nullptr)
		, m_currentItemIndex(SIZE_MAX)
	{
	}
	
	

	
	
	/*void menu::AddItem(std::shared_ptr<iitem> item)
	{
		item->Control()->SetSpriteBatch(m_spriteBatch);

		m_items.push_back(item);
	}*/

	void menu::InternalDraw(const render::camera& camera)
	{
		UNREFERENCED(camera);

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);

		float y = m_rectangle.Top();
		for (size_t i = 0; i < m_items.size(); ++i )
		{
			auto& item = m_items[i];
			auto control = item->Control();
						
			//control->SetPosition( math::vector2(m_rectangle.Left(), y), ui::control::Pixels );
			
			if ( i == m_currentItemIndex )
				control->SetForegroundColor(render::color::RED);
			else
				control->SetForegroundColor(render::color::WHITE);
			
			control->Draw(camera);
			y += control->Rectangle().Height();
		}
	}

	void menu::Refresh()
	{
		float y = m_rectangle.Top();
		for (size_t i = 0; i < m_items.size(); ++i )
		{
			auto& item = m_items[i];
			auto control = item->Control();
						
			control->SetPosition( math::vector2(m_rectangle.Left(), y), ui::control::Pixels );
			
			

			y += control->Rectangle().Height();
		}
	}

	void menu::Update(float deltaTime)
	{
		for ( auto it : m_items )
		{
			it->Control()->Update(deltaTime); 
		}
	}

	bool menu::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		auto keyboard = inputState.GetKeyboard();
		if ( keyboard != nullptr )
		{
			if ( keyboard->KeyPressed(DIK_UP) )
			{				
				if ( m_wrapAround && (m_currentItemIndex == SIZE_MAX || m_currentItemIndex == 0 ))
					m_currentItemIndex = m_items.size() - 1;
				else
					--m_currentItemIndex;

				auto selectedControl = m_items[m_currentItemIndex]->Control();
				m_onSelection.Invoke(this, *selectedControl);
				OnSelection(*selectedControl);
				return true;
			}
			else
			if (keyboard->KeyPressed(DIK_DOWN) )
			{
				if ( m_currentItemIndex != SIZE_MAX )
				{
					++m_currentItemIndex;
					if ( m_wrapAround && m_currentItemIndex >= m_items.size() )
						m_currentItemIndex = 0 ;

					auto selectedControl = m_items[m_currentItemIndex]->Control();
					m_onSelection.Invoke(this, *selectedControl);
					OnSelection(*selectedControl);
					return true;
				}
			}
			else
			if ( keyboard->KeyPressed(DIK_RETURN) || keyboard->KeyPressed(DIK_NUMPADENTER) )
			{
				if ( m_currentItemIndex != SIZE_MAX )
				{
					auto selectedControl = m_items[m_currentItemIndex]->Control();
					m_onAccept.Invoke(this, *selectedControl);
					OnAccept(*selectedControl);
					return true;
				}
			}
			else
			if ( keyboard->KeyPressed(DIK_ESCAPE) )
			{
				if ( m_currentItemIndex != SIZE_MAX )
				{
					auto selectedControl = m_items[m_currentItemIndex]->Control();
					m_onCancel.Invoke(this, *selectedControl);
					OnCancel(*selectedControl);
					return true;
				}
			}

		}

		int index = 0;
		for ( auto it : m_items )
		{
			if ( it->Control()->HandleInput(deltaTime, inputState) ) 
			{
				m_currentItem = it;
				m_currentItemIndex = index;

				return true;
			}

			++index;
		}

		return control::HandleInput(deltaTime, inputState);
	}
}