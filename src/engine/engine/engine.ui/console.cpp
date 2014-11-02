#include "console.h"
#include "engine.render/texture_black.h"
#include "CommonStates.h"
#include "engine.core/runtime_variable.h"
#include "engine.core/game.h"
#include "textbox.h"
#include "listbox.h"

runtime_variable_float console_transparency(L"console_transparency", L"controls the alpha of the console", 0.6f, 0.f, 1.f);
runtime_variable_float console_height(L"console_height", L"controls the size of the console from the top", 200.f, 0.f, 10000.f);


namespace ui
{

	float console::s_inputThrottleLength = 0.5f;

	console::console(std::shared_ptr<core_ui> core)
		: m_core(core)
		, m_inputThrottle(s_inputThrottleLength)
		, m_lastKey(0)
		, m_textBox(nullptr)
		, m_clear(L"clear", L"Clears the console", [&](std::vector<std::wstring>) -> bool { Clear(); return true; })
	{
		m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch( m_core->GetDevice()->GetImmediateContext() ) );
		m_font = std::shared_ptr<render::platform::font>(new render::platform::font(m_core->GetDevice()->GetDevice(), game::GetContentPath(L"\\content\\fonts\\greyscale.font").c_str()));

		auto viewport = m_core->GetDevice()->GetViewport();
		m_rectangle = math::rectangle(viewport.Left(), viewport.Top(), viewport.Width(), viewport.Height() * 0.4f);

		m_backgroundColor = render::color::DARKSLATEGRAY;
		m_backgroundColor.A() = 0.4f;

		math::rectangle textRectangle = m_rectangle;
		textRectangle.Top() = m_rectangle.Bottom() - m_font->GetLineSpacing();
		textRectangle.Height() = m_font->GetLineSpacing();

		m_textBox = std::unique_ptr<textbox>(new textbox(m_core, m_font, m_spriteBatch));
		m_textBox->SetPosition(textRectangle.Position(), control::Pixels);
		m_textBox->SetSize(textRectangle.Size(), control::Pixels);
		m_textBox->SetBackgroundColor(m_backgroundColor);
		m_textBox->SetForegroundColor(render::color::WHITE);
		m_textBox->OnTextEntered() += [this](void*, const std::wstring& text) { Execute(text); };
		m_textBox->SetAutoCompleteSource(runtime_variable::GetDatabase().GetTrie());
	
		m_listBox = std::unique_ptr<listbox>(new listbox(m_core, m_font, m_spriteBatch));
		m_listBox->SetPosition(m_rectangle.Position(), ui::control::Pixels );
		m_listBox->SetSize(math::vector2(m_rectangle.Size().x(), m_rectangle.Size().y() - m_font->GetLineSpacing()), ui::control::Pixels );
		m_listBox->SetBackgroundColor(m_backgroundColor);
		m_listBox->SetForegroundColor(render::color::WHITE);
		m_listBox->Mode() = ui::listbox::Ascending;

		console_transparency.OnChange() += [&] (void*, int) { m_backgroundColor.A() = console_transparency; m_listBox->Alpha() = console_transparency; m_textBox->Alpha() = console_transparency; };
		console_height.OnChange() += [&](void*,int) { m_rectangle.Height() = console_height; m_listBox->SetSize(m_rectangle.Size(), ui::control::Pixels ); };
	}

	console::~console()
	{
	}

	void console::Clear()
	{
		m_listBox->Clear();
		m_items.clear();
	}

	void console::Update(float deltaTime)
	{
		m_textBox->Update(deltaTime);
		m_listBox->Update(deltaTime);
	}

	bool console::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		if ( m_listBox->HandleInput(deltaTime, inputState) )
		{
			return true;
		}

		if ( m_textBox->HandleInput(deltaTime, inputState) )
		{
			return true;
		}

		auto keyboard = inputState.GetKeyboard();
		if ( keyboard->KeyPressed(DIK_DOWN) )
		{
			NextHistoryItem();
		}

		if ( keyboard->KeyPressed(DIK_UP) )
		{
			PreviousHistoryItem();
		}

		return false;
	}

	void console::AddItem(const std::wstring& text, const render::color& color)
	{
		ui::listbox::listboxitemtext item(m_listBox.get(), math::vector2(m_rectangle.Width(), m_font->GetLineSpacing()), text, color);
		m_listBox->Add(text);
	}

	void console::Execute(const std::wstring& text)
	{
		if ( text.size() == 0 )
			return;

		console_parser params(text.c_str());

		auto& database = runtime_variable::GetDatabase();
		if ( !database.GetTrie()->contains_word(params.GetString(0).c_str()) )
		{
			AddHistory(text);
			AddItem(text + L" not found.", render::color::RED);
			return;
		}
	
		for ( auto it : database.GetVariables() )
		{		
			if ( wcscmp(it->Name().c_str(), params.GetString(0).c_str()) == 0 )
			{			
				if (it->Parse(params.GetParametersFromIndex(1).c_str()))
				{			
					AddItem(text, m_textBox->ForegroundColor());				
				}
				else
				{
					auto color = m_textBox->ForegroundColor();
					AddItem(it->Name() + L" " + it->Value(), color);
					AddItem(it->Description(), color * 0.75f);
				}

				AddHistory(text);
			
				break;
			}
		}
	}

	void console::DrawItems()
	{
		math::vector2 p = math::vector2(m_rectangle.Left(), m_rectangle.Bottom() - m_textBox->Size().y() - m_font->GetLineSpacing());
		for ( item& i : m_items )
		{
			i.color.A() = console_transparency;
			m_font->DrawString(m_spriteBatch.get(), i.line.c_str(), p, i.color);
			p.y() -= m_font->GetLineSpacing();
			if ( p.y() < m_rectangle.Top() )
				break;
		}
	}

	void console::Draw(const render::camera& camera)
	{
		DirectX::CommonStates states(m_core->GetDevice()->GetDevice());

		m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront, states.NonPremultiplied());

	
		m_listBox->Draw(camera);

		m_textBox->SetPosition(math::vector2(m_rectangle.Left(), m_rectangle.Bottom() - m_font->GetLineSpacing()), ui::control::Pixels );

		auto& textureView = *m_core->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(textureView, m_rectangle, nullptr, m_backgroundColor);
		m_textBox->Draw(camera); 
	
		// thin white line between console and text input box
		math::rectangle divisorRectangle = m_rectangle;
		divisorRectangle.Height() = 1;
		divisorRectangle.Top() = m_rectangle.Bottom() - m_font->GetLineSpacing();
		m_spriteBatch->Draw(textureView, divisorRectangle, nullptr, render::color::WHITE);	

		m_spriteBatch->End();
	}

	void console::AddHistory(const std::wstring& text)
	{
		m_history.push_front(text);
		m_currentHistoryItem = m_history.begin();
	}

	void console::NextHistoryItem()
	{
		if ( m_history.size() == 0 ||  m_currentHistoryItem == m_history.begin() )
			return;

		--m_currentHistoryItem;

		m_textBox->SetText((*m_currentHistoryItem).c_str());	
	}

	void console::PreviousHistoryItem()
	{
		if ( m_history.size() == 0 )
			return;

		m_textBox->SetText((*m_currentHistoryItem).c_str());
	
		if ( ++m_currentHistoryItem == m_history.end() )
		{
			--m_currentHistoryItem;
		}
	}

} // ui