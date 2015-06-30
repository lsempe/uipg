#include "stdafx.h"
#include "controls.h"

#include "engine.core/input.h"
#include "engine.ui/core.h"
#include "engine.render/platform.h"

// Controls
#include "engine.ui/slider.h"
#include "engine.ui/button.h"
#include "engine.ui/label.h"
#include "engine.ui/textbox.h"
#include "engine.ui/dropdown2.h"
#include "engine.ui/progressbar.h"
#include "engine.ui/menu.h"


namespace samples
{
	void controls::Initialize()
	{
		framework::Initialize();

		m_inputState = std::shared_ptr<input::input_state>(new input::input_state(m_windowHandle));
		m_mouse = m_inputState->CreateMouse();
		m_inputState->CreateKeyboard();

		m_core = std::shared_ptr<ui::core_ui>(new ui::core_ui(&m_device, *m_inputState));
		
		render::viewport viewport(0,0, (float)ClientWidth(), (float)ClientHeight(), 0.f, 1.f);
				
		m_camera = render::camera(viewport);
		m_camera.LookAt(math::vector3::Zero, math::vector3::UnitZ, math::vector3::UnitY);

		m_cursor = std::unique_ptr<ui::input_cursor>(new ui::input_cursor(m_core, nullptr, nullptr, L""));
		m_cursor->Load(game::GetContentPath(L"\\content\\sprites\\UIpackSheet_transparent.dds").c_str());

		auto deviceContext = m_device.GetImmediateContext();

		m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new DirectX::SpriteBatch( deviceContext ) );

		m_fontLibrary->Load(L"title", game::GetContentPath(L"\\content\\fonts\\archery.font").c_str());
		m_fontLibrary->Load(L"default", game::GetContentPath(L"\\content\\fonts\\8bit.font").c_str());
		
		m_container = std::unique_ptr<ui::container>(new ui::container(m_core, m_fontLibrary->Get(L"title")));
		m_container->SetViewport(viewport);
		
		/// Enabled button 
		auto buttonEnabled = m_container->Add<ui::button>();
		buttonEnabled->ShowText() = true;
		buttonEnabled->SetFont(m_fontLibrary->Get(L"title"));
		buttonEnabled->SetPosition(math::vector2(10.f,60.f), ui::control::Pixels);
		buttonEnabled->SetSize(math::vector2(150.f, 48.f), ui::control::Pixels);		
		buttonEnabled->SetText(L"Button 1");
		buttonEnabled->Load(game::GetContentPath(L"\\content\\sprites\\redSheet.dds").c_str());

		/// Disabled button
		auto buttonDisabled = m_container->Add<ui::button>();
		buttonDisabled->SetPosition(math::vector2(buttonEnabled->Rectangle().Right() + 10.f,60.f), ui::control::Pixels);
		buttonDisabled->SetSize(math::vector2(150.f, 48.f), ui::control::Pixels);		
		buttonDisabled->SetText(L"Button 2");
		buttonDisabled->ShowText() = true;
		buttonDisabled->SetFont(m_fontLibrary->Get(L"title"));
		//buttonDisabled->SetState(ui::button::Disabled);		
		buttonDisabled->Load(game::GetContentPath(L"\\content\\sprites\\redSheet.dds").c_str());

		/// Title label
		auto label = m_container->Add<ui::label>();
		label->SetFont( m_fontLibrary->Get(L"title") );
		label->SetPosition(math::vector2(0.f,0.f), ui::control::Percent);
		label->SetSize(math::vector2(1.f, 0.25f), ui::control::Percent);
		label->AutoSize() = true;
		label->SetText(L"UIPG Controls");
		label->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Center | ui::align::Top));
		label->ShowBackground() = false;
		
		/// Slider
		auto slider = m_container->Add<ui::slider>();
		slider->SetPosition(math::vector2(10.f,120.f), ui::control::Pixels);
		slider->SetSize(math::vector2(128.f, 32.f), ui::control::Pixels);		
		slider->Load(game::GetContentPath(L"\\content\\sprites\\UIpackSheet_transparent.dds").c_str());

		/// Textbox
		auto textbox = m_container->Add<ui::textbox>();
		textbox->SetPosition(math::vector2(10.f,220.f), ui::control::Pixels);
		textbox->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels);
		textbox->SetText(L"text");

		/// Progress bar
		m_progressBar = m_container->Add<ui::progress_bar>();
		m_progressBar->SetPosition(math::vector2(10.f,300.f), ui::control::Pixels);
		m_progressBar->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels);
		m_progressBar->SetForegroundColor(render::color::ORANGERED);
		m_progressBar->Load(game::GetContentPath(L"\\content\\sprites\\UIpackSheet_transparent.dds").c_str());
		
		/// Dropdown
		auto dropdown = m_container->Add<ui::dropdown2>();
		dropdown->SetPosition(math::vector2(10.f, 360.f), ui::control::Pixels);
		dropdown->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels );
		dropdown->Add(L"Item 1");
		dropdown->Add(L"Item 2");
		dropdown->Add(L"Item 3");
		dropdown->Add(L"Item 4");
		dropdown->Add(L"Item 5");
		dropdown->Add(L"Item 6");

		/// Mouse coordinates label
		m_mouseCoordinates = m_container->Add<ui::label>();
		m_mouseCoordinates->SetPosition(math::vector2(0.8f,0.93f), ui::control::Percent);
		m_mouseCoordinates->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels);
		m_mouseCoordinates->ShowBackground() = false;
		
		/// Menu
		auto menu = m_container->Add<ui::menu>();
	
		menu->SetPosition(math::vector2(400.f, 100.f), ui::control::Pixels);
		menu->SetSize(math::vector2(300.f, 200.f), ui::control::Pixels);

		menu->OnSelection() += [](void*, ui::control& control) { control.SetForegroundColor(render::color::RED); };

		auto itemLabel0 = menu->CreateItem<ui::label>(m_core, m_fontLibrary->Get(L"title"), m_spriteBatch);
		itemLabel0->SetSize(math::vector2(300.f, itemLabel0->Font()->GetLineSpacing()), ui::control::Pixels);
		itemLabel0->SetText(L"Option 1");				
		itemLabel0->OnMousePressed() += [&](void*, ui::control& c) { c.Alpha() = 0.45f; };
		itemLabel0->OnMouseReleased() += [&](void*, ui::control& c) { c.Alpha() = 1.f; };

		auto itemLabel1 = menu->CreateItem<ui::label>(m_core, m_fontLibrary->Get(L"title"), m_spriteBatch);
		itemLabel1->SetSize(math::vector2(300.f, itemLabel1->Font()->GetLineSpacing()), ui::control::Pixels);
		itemLabel1->SetText(L"Option 2");
		itemLabel1->OnMousePressed() += [&](void*, ui::control& c) { c.Alpha() = 0.45f; };
		itemLabel1->OnMouseReleased() += [&](void*, ui::control& c) { c.Alpha() = 1.f; };
		
		auto itemLabel2 = menu->CreateItem<ui::label>(m_core, m_fontLibrary->Get(L"title"), m_spriteBatch);
		itemLabel2->SetSize(math::vector2(300.f, itemLabel2->Font()->GetLineSpacing()), ui::control::Pixels);
		itemLabel2->SetText(L"Option 3");
		itemLabel2->OnMousePressed() += [&](void*, ui::control& c) { c.Alpha() = 0.45f; };
		itemLabel2->OnMouseReleased() += [&](void*, ui::control& c) { c.Alpha() = 1.f; };

		auto itemSlider = menu->CreateItem<ui::slider>(m_core, m_fontLibrary->Get(L"title"), m_spriteBatch);
		itemSlider->SetSize(math::vector2(300.f, itemLabel2->Font()->GetLineSpacing()), ui::control::Pixels);		
		itemSlider->Load(game::GetContentPath(L"\\content\\sprites\\UIpackSheet_transparent.dds").c_str());
		itemSlider->SetBackgroundColor(render::color::SLATEGRAY);

		// Label that displays which listbox items are selected.
		m_listBoxSelection = m_container->Add<ui::label>();
		m_listBoxSelection->SetFont( m_fontLibrary->Get(L"title") );
		m_listBoxSelection->SetPosition(math::vector2(400.f,520.f), ui::control::Pixels);
		m_listBoxSelection->AutoSize() = true;
		m_listBoxSelection->SetText(L"");
		m_listBoxSelection->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Left | ui::align::Bottom ));
		m_listBoxSelection->ShowBackground() = false;

		/// Listbox
		auto listBox = m_container->Add<ui::listbox>();
		listBox->ScrollBar().Load(game::GetContentPath(L"\\content\\sprites\\redSheet.dds").c_str());
		listBox->SetPosition(math::vector2(400.f, 360.f), ui::control::Pixels);
		listBox->SetSize(math::vector2(256.f, listBox->Font()->GetLineSpacing() * 3.1f), ui::control::Pixels );
		listBox->OnItemSelected() += [&](void* owner, ui::listbox::listboxitem& )
		{
			ui::listbox* listBox = reinterpret_cast<ui::listbox*>(owner);
			m_listBoxSelection->SetText(L"");
			std::wstring items(L"");
			unsigned int count = 0;
			auto& selectedItems = listBox->SelectedItems();
			for (auto item : selectedItems )
			{
				items.append(static_cast<ui::listbox::listboxitemtext*>(item)->Text());
				
				if ( count++ < selectedItems.size() - 1)
					items.append(L", ");
			}

			m_listBoxSelection->SetText(items);
		};
		
		listBox->Add(L"Item 1");
		listBox->Add(L"Item 2");
		listBox->Add(L"Item 3");
		listBox->Add(L"Item 4");
		listBox->Add(L"Item 5");
		listBox->Add(L"Item 6");
		listBox->Add(L"Item x5");
		listBox->Add(L"Itemx x6");
		listBox->Add(L"Item x5");
		listBox->Add(L"Item x6");
		listBox->Add(L"Itemxx 5");
		listBox->Add(L"Itemx 6");


		// Alignment
		auto labelNW = m_container->Add<ui::label>();
		labelNW->SetFont( m_fontLibrary->Get(L"title") );
		labelNW->SetPosition(math::vector2(800.f,100.f), ui::control::Pixels);
		labelNW->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelNW->AutoSize() = false;
		labelNW->SetText(L"NW");
		labelNW->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Left | ui::align::Top ));
		labelNW->ShowBackground() = true;

		auto labelN = m_container->Add<ui::label>();
		labelN->SetFont( m_fontLibrary->Get(L"title") );
		labelN->SetPosition(math::vector2(900.f,100.f), ui::control::Pixels);
		labelN->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelN->AutoSize() = false;
		labelN->SetText(L"N");
		labelN->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Center | ui::align::Top ));
		labelN->ShowBackground() = true;

		auto labelNE = m_container->Add<ui::label>();
		labelNE->SetFont( m_fontLibrary->Get(L"title") );
		labelNE->SetPosition(math::vector2(1000.f,100.f), ui::control::Pixels);
		labelNE->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelNE->AutoSize() = false;
		labelNE->SetText(L"NE");
		labelNE->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Right | ui::align::Top ));
		labelNE->ShowBackground() = true;

		auto labelW = m_container->Add<ui::label>();
		labelW->SetFont( m_fontLibrary->Get(L"title") );
		labelW->SetPosition(math::vector2(800.f,200.f), ui::control::Pixels);
		labelW->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelW->AutoSize() = false;
		labelW->SetText(L"W");
		labelW->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Left | ui::align::Middle));
		labelW->ShowBackground() = true;

		auto labelC = m_container->Add<ui::label>();
		labelC->SetFont( m_fontLibrary->Get(L"title") );
		labelC->SetPosition(math::vector2(900.f,200.f), ui::control::Pixels);
		labelC->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelC->AutoSize() = false;
		labelC->SetText(L"C");
		labelC->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Center | ui::align::Middle));
		labelC->ShowBackground() = true;

		auto labelE = m_container->Add<ui::label>();
		labelE->SetFont( m_fontLibrary->Get(L"title") );
		labelE->SetPosition(math::vector2(1000.f,200.f), ui::control::Pixels);
		labelE->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelE->AutoSize() = false;
		labelE->SetText(L"E");
		labelE->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Right | ui::align::Middle));
		labelE->ShowBackground() = true;

		auto labelSW = m_container->Add<ui::label>();
		labelSW->SetFont( m_fontLibrary->Get(L"title") );
		labelSW->SetPosition(math::vector2(800.f,300.f), ui::control::Pixels);
		labelSW->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelSW->AutoSize() = false;
		labelSW->SetText(L"SW");
		labelSW->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Left | ui::align::Bottom ));
		labelSW->ShowBackground() = true;

		auto labelS = m_container->Add<ui::label>();
		labelS->SetFont( m_fontLibrary->Get(L"title") );
		labelS->SetPosition(math::vector2(900.f,300.f), ui::control::Pixels);
		labelS->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelS->AutoSize() = false;
		labelS->SetText(L"S");
		labelS->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Center | ui::align::Bottom ));
		labelS->ShowBackground() = true;

		auto labelSE = m_container->Add<ui::label>();
		labelSE->SetFont( m_fontLibrary->Get(L"title") );
		labelSE->SetPosition(math::vector2(1000.f,300.f), ui::control::Pixels);
		labelSE->SetSize(math::vector2(80.f, 80.f), ui::control::Pixels);
		labelSE->AutoSize() = false;
		labelSE->SetText(L"SE");
		labelSE->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Right | ui::align::Bottom ));
		labelSE->ShowBackground() = true;
	}

	void controls::Update()
	{ 
		

		const math::vector2& mousePosition = m_mouse->GetPosition();
		m_cursor->SetPosition(mousePosition, ui::control::Pixels);

		std::wstringstream mouseCoordinates;
		mouseCoordinates << L"X: " << mousePosition.x() << L" Y: " << mousePosition.y();
		m_mouseCoordinates->SetText(mouseCoordinates.str());

		framework::Update(); 	
	}

	void controls::Update(float deltaTime)
	{
		m_inputState->Update(deltaTime);

		m_container->HandleInput(deltaTime, *m_inputState);
		m_cursor->HandleInput(deltaTime, *m_inputState);

		m_cursor->Update(deltaTime);		
		m_container->Update(deltaTime);

		// Oscillate the progress bar
		static float ratio = 0.f;

		ratio += deltaTime;
		if (ratio >= 1.f) ratio = 0.f;

		m_progressBar->SetRatio(ratio);

	}

	void controls::Render()
	{

		m_container->Draw(m_camera);

		m_spriteBatch->Begin();
			
		
		m_spriteBatch->End();

		m_cursor->Draw();

	}
}