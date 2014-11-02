#pragma once

#include "control.h"
#include "countdown.h"
#include "label.h"
#include "scrollbar.h"

namespace ui
{

class listbox : public control
{

public:

	enum eSorting
	{
		Ascending,
		Descending
	};

	listbox(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<DirectX::SpriteBatch> spriteBatch);

	virtual ~listbox();

	virtual void Refresh();

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState);

	virtual void InternalDraw(const render::camera& camera);
	
	class listboxitem
	{
	public:

		listboxitem(listbox* owner)
			: m_owner(owner)
			, m_selected(false)
			, m_visible(false)
		{}

		virtual bool& Selected() { return m_selected; }
		virtual bool& Visible() { return m_visible; }
		
		virtual const math::rectangle& Rectangle() const { return m_rectangle; }

		virtual void Draw(const math::vector2& position, std::shared_ptr<DirectX::SpriteBatch> spriteBatch)
		{
			UNREFERENCED(position);
		}

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			UNREFERENCED(deltaTime);
			UNREFERENCED(inputState);

			return false;
		}

		event_handler<listboxitem*>& OnSelected() { return m_onSelected; }

		class data
		{
		public:

		};

		void SetData(std::shared_ptr<data> data)
		{
			m_data = data;
		}

		std::shared_ptr<data> Data() { return m_data; }

	protected:

		virtual void SetPosition(const math::vector2& position) { m_rectangle.SetPosition(position); }
		virtual void SetSize(const math::vector2& size) { m_rectangle.SetSize(size); }

		bool m_visible;
		bool m_selected;
		math::rectangle m_rectangle;
		event_handler<listboxitem*> m_onSelected;
		listbox* m_owner;

		std::shared_ptr<data> m_data;
	};

	template <typename T = control>
	class listboxitemcontrol : public virtual listboxitem, public virtual T
	{
	public:

		listboxitemcontrol(listbox* owner, std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<DirectX::SpriteBatch>& spriteBatch)
			: listboxitem(owner)
			, T(core, font, spriteBatch)
		{}

		virtual void Draw(const math::vector2& position, std::shared_ptr<DirectX::SpriteFont> font, std::shared_ptr<DirectX::SpriteBatch>)
		{
			T::SetPosition(position);
			T::SetSize(math::vector2(m_owner->Size().x(), m_font->GetLineSpacing()));

			T::Draw();
		}
		
		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			return T::HandleInput(deltaTime, inputState);
		}

	};

	typedef listboxitemcontrol<label> listboxitemlabel;

	class listboxitemtext : public listboxitem
	{
	public:

		listboxitemtext(listbox* owner, const math::vector2& size, const std::wstring& text, const render::color& color = render::color::WHITE)
			: listboxitem(owner)
			, m_size(size)
			, m_text(text)
			, m_color(color)
			, m_inputDelay(0.1f)
		{
			SetSize(math::vector2(m_owner->Rectangle().Width(), m_owner->Font()->GetLineSpacing()));
		}

		virtual void Draw(const math::vector2& position, std::shared_ptr<DirectX::SpriteBatch> spriteBatch)
		{
			if ( m_visible )
			{
				SetPosition(position);
				
							
				render::color color = m_color;
				if ( m_mouseOver )
					color = render::color::RED;

				if ( m_selected )
					color = render::color::STEELBLUE;

				m_owner->Font()->DrawString(spriteBatch.get(), m_text.c_str(), position, color );
			}
		}

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			if ( !m_visible )
				return false;

			m_inputDelay.Update(deltaTime);

			auto mouse = inputState.GetMouse();
			if ( mouse != nullptr )
			{
				math::rectangle rectangle = m_owner->Rectangle();
				rectangle.Top() = m_rectangle.Top();
				rectangle.Height() = m_size.y();
				m_mouseOver = rectangle.Contains(mouse->GetPosition());	
				if ( m_mouseOver )
				{
					if ( m_inputDelay.IsDone() && mouse->ButtonPressed(input::mouse::eButton::Left) )
					{
						m_inputDelay.Start();
						m_selected = !m_selected;
						m_onSelected.Invoke(this, this);
						return true;
					}
				}
			}
			return false;
		}

		const std::wstring& Text() const { return m_text; }

	protected:

		math::vector2 m_size;
		render::color m_color;
		std::wstring m_text;
		bool m_mouseOver;

		countdown m_inputDelay;

	};

	void UpdateList(listboxitem* item)
	{
		if ( !m_multiSelection )
		{				
			for (auto it : m_selectedItems )
			{
				it->Selected() = false;
			}
			m_selectedItems.clear();
		}
		
		if ( item->Selected() )
		{
			m_selectedItems.push_back(item);
		}
		else
		{			
			auto it = std::find(m_selectedItems.begin(), m_selectedItems.end(), item);
			if ( it != m_selectedItems.end() )
			{
				m_selectedItems.erase(it);
			}
		}
	}
	
	event_handler<listboxitem&> m_onItemSelected;
	event_handler<listboxitem&>& OnItemSelected() { return m_onItemSelected; }

	void Add(const std::wstring text, std::shared_ptr<listboxitem::data> data = nullptr)
	{
		listboxitemtext* item = new listboxitemtext(this, math::vector2(m_rectangle.Width(), m_font->GetLineSpacing()), text, render::color::WHITE);
		item->SetData(data);
		//listboxitemlabel* item = new listboxitemlabel(this, m_core, m_font, m_spriteBatch);
		//item->SetText(text);
		//item->SetForegroundColor(render::color::WHITE);
	
		item->OnSelected() += [&] (void*, listboxitem* item) 
		{ 
			UpdateList(item); 
			m_onItemSelected.Invoke(this, *item); 
		};

		if ( m_sorting == Descending ) 
		{
			m_items.push_back(item);
		}
		else
		{
			m_items.push_front(item);
		}

		Refresh();
	}

	void Clear()
	{
		for ( auto it : m_items )
		{
			delete it;
		}
		m_items.clear();
	}

	eSorting& Mode() { return m_sorting; }

	const std::list<listboxitem*>& Items() const { return m_items; }
	const std::list<listboxitem*>& SelectedItems() const { return m_selectedItems; }

	bool& MultiSelection() { return m_multiSelection; }

	scrollbar& ScrollBar() { return m_scrollBar; }
protected:

	std::list<listboxitem*> m_items;

	scrollbar m_scrollBar;

	bool m_multiSelection;

	int m_lastMouseWheelValue;

	eSorting m_sorting;
	std::list<listboxitem*> m_selectedItems;

};

} // ui