#pragma once

#include "control.h"
#include "button.h"
#include "listbox.h"

namespace ui
{

class dropdown2 : public control
{
public:

	dropdown2(std::shared_ptr<core_ui> core, const std::shared_ptr<DirectX::SpriteFont>& font, const std::shared_ptr<DirectX::SpriteBatch> spriteBatch)
		: control(core, font, spriteBatch)
		, m_open(false)
		, m_inputDelay(0.185f)
	{
		m_button = std::shared_ptr<button>(new button(core, font, spriteBatch));
		m_contents = std::shared_ptr<listbox>(new listbox(core, font, spriteBatch));	
		m_contents->MultiSelection() = false;
		m_contents->OnItemSelected() += [&](void*, listbox::listboxitem&) { m_open = false; Refresh(); };
		m_button->OnReleased() += [&](void*,button&) { m_open = !m_open; Refresh(); };	
	}

	virtual void Refresh()
	{
		math::vector2 position = m_rectangle.Position();
		position.y() += m_rectangle.Height();

		m_contents->SetPosition(position, ui::control::Pixels );
		float totalWidth = m_rectangle.Size().x() + m_button->Size().x();
		m_contents->SetSize(math::vector2(totalWidth, m_open ? m_rectangle.Size().y() * 2.f : m_rectangle.Size().y()), ui::control::Pixels  );

		m_button->SetPosition(math::vector2(m_rectangle.Position().x() + m_rectangle.Size().x(), m_rectangle.Position().y()), ui::control::Pixels );
		m_button->SetSize(math::vector2(m_rectangle.Size().x() * 0.1f, m_rectangle.Size().y()), ui::control::Pixels );
		m_button->SetBackgroundColor(render::color::GREEN);

		m_contents->ScrollBar().SetSize(math::vector2(m_button->Size().x(), m_contents->Size().y()), ui::control::Pixels);

	}

	virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
	{
		if ( !m_inputDelay.IsDone() )
			return false;

		if ( m_button->HandleInput(deltaTime, inputState) )
		{
			m_inputDelay.Start();
			return true;
		}

		auto mouse = inputState.GetMouse();
		if (mouse != nullptr)
		{
			if (!m_open && m_mouseOver )
			{
				if ( mouse->ButtonReleased(input::mouse::Left) )
				{
					m_open = !m_open;
					Refresh();
					m_inputDelay.Start();
					return true;
				}
			}
		}

		if ( m_open && m_contents->HandleInput(deltaTime, inputState) )
		{
			m_inputDelay.Start();
			return true;
		}

		return control::HandleInput(deltaTime, inputState);
	}

	virtual void Update(float deltaTime)
	{
		m_inputDelay.Update(deltaTime);

		m_button->Update(deltaTime);
		if ( m_open )
		{
			m_contents->Update(deltaTime);
		}
	}

	virtual void InternalDraw(const render::camera& camera)
	{
		m_button->Draw(camera);
		
		auto& textureView = *m_core->GetWhiteTexture()->GetView();

		m_spriteBatch->Draw(textureView, m_rectangle, m_backgroundColor);

		listbox::listboxitemtext* selection = static_cast<listbox::listboxitemtext*>((*m_contents->Items().begin()));
		if ( m_contents->SelectedItems().size() == 1 )
		{
			selection = static_cast<listbox::listboxitemtext*>((*m_contents->SelectedItems().begin()));
		}

		m_font->DrawString(m_spriteBatch.get(), selection->Text().c_str(), m_rectangle.Position());

		if ( m_open )
		{
			m_contents->Draw(camera);
		}
	}

	void Add(const std::wstring& item)
	{
		m_contents->Add(item);		
	}

private:

	std::shared_ptr<button> m_button;
	std::shared_ptr<listbox> m_contents;
	
	bool m_open;
	countdown m_inputDelay;
};


} // ui