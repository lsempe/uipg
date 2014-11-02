#pragma once

#include "control.h"

namespace ui
{

	class label;

	class menu : public control
	{
	public:

		menu(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<render::platform::sprite_batch>& spriteBatch);

		

		class iitem 
		{
		public:
			virtual std::shared_ptr<control> Control() = 0;
		};

		template <typename control_type>
		class menuitem : public iitem
		{
		public:

			menuitem(std::shared_ptr<control_type> item) { m_control = item; }
			
			std::shared_ptr<control_type> m_control;

			virtual std::shared_ptr<control> Control() { return m_control; }
			 
		};
		
		template <typename control_type>
		std::shared_ptr<control_type> CreateItem(std::shared_ptr<core_ui> core, const std::shared_ptr<render::platform::font>& font, const std::shared_ptr<render::platform::sprite_batch> spriteBatch)
		{
			auto item = std::shared_ptr<control_type>(new control_type(core, font, m_spriteBatch));
			
			std::shared_ptr<menuitem<control_type>> menuItem = std::shared_ptr<menuitem<control_type>>( new menuitem<control_type>(item) );
			m_items.emplace_back(menuItem);

			Refresh();

			return item;
		}

		template <typename T = control>
		class item : public iitem
		{
		public:	

			item(std::shared_ptr<T> control)
			{
				m_control = std::move(control);
			}

			virtual std::shared_ptr<control> Control() {  return m_control; }

		protected:

			std::shared_ptr<T> m_control;
		};

		typedef item<label> itemlabel;

		void AddItem(std::shared_ptr<iitem> item);
	
		virtual void Refresh();
		virtual void Update(float deltaTime);
		virtual void InternalDraw(const render::camera& camera);

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState);
	
		typedef event_handler<control&> menu_event;

		menu_event& OnSelection() { return m_onSelection; }
		menu_event& OnAccept() { return m_onAccept; }
		menu_event& OnCancel() { return m_onCancel; }

		virtual void OnAccept(control&) { }
		virtual void OnCancel(control&) { }
		virtual void OnSelection(control&) { }

	protected:

		bool m_wrapAround;

		menu_event m_onSelection;
		menu_event m_onAccept;
		menu_event m_onCancel;

		std::vector<std::shared_ptr<iitem>> m_items;

		size_t m_currentItemIndex;
		std::shared_ptr<iitem> m_currentItem;

	};

} // ui