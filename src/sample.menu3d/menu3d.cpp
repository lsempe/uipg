#include "stdafx.h"
#include "menu3d.h"

#include "engine.core/input.h"
#include "engine.render/platform.h"

#include "engine.ui/core.h"
#include "engine.ui/label.h"
#include "engine.ui/menu_3d.h"


namespace samples
{
	menu3d::menu3d(HWND& windowHandle, int clientWidth, int clientHeight)
			: framework(windowHandle, clientWidth, clientHeight)
	{
	}

	menu3d::~menu3d()
	{
	}

	void menu3d::Initialize()
	{
		framework::Initialize();

		// Initialize the input system
		m_inputState = std::shared_ptr<input::input_state>(new input::input_state(m_windowHandle));
		m_inputState->CreateKeyboard();
		m_mouse = m_inputState->CreateMouse();

		// Setup the fonts
		m_fontLibrary->Load(L"title", game::GetContentPath(L"\\content\\fonts\\archery.font").c_str());
		m_fontLibrary->Load(L"default", game::GetContentPath(L"\\content\\fonts\\8bit.font").c_str());
	
		// Setup the UI
		render::viewport viewport(0, 0, (float)ClientWidth(), (float)ClientHeight(), 0.f, 1.f);
		m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new DirectX::SpriteBatch(m_device.GetImmediateContext()));

		m_core = std::shared_ptr<ui::core_ui>(new ui::core_ui(&m_device, *m_inputState));

		m_cursor = std::unique_ptr<ui::input_cursor>(new ui::input_cursor(m_core, nullptr, nullptr, game::GetContentPath(L"\\content\\sprites\\cursor.dds")));

		m_container = std::unique_ptr<ui::container>(new ui::container(m_core, m_fontLibrary->Get(L"title")));
		m_container->SetViewport(viewport);

		// Setup the camera
		m_camera = render::camera(viewport);
		m_camera.LookAt(math::vector3(0.f, 0.f, 2.f), math::vector3(0.f, 0.f, 0.f), math::vector3::UnitY);

		/// Mouse coordinates label
		m_mouseCoordinates = m_container->Add<ui::label>();
		m_mouseCoordinates->SetPosition(math::vector2(0.8f,0.93f), ui::control::Percent);
		m_mouseCoordinates->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels);
		m_mouseCoordinates->ShowBackground() = false;
		
		// Load textures
		auto tex0 = std::shared_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\textures\\menus\\materials.dds") ) );
		auto tex1 = std::shared_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\textures\\menus\\mine.dds") ) );
		auto tex2 = std::shared_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\textures\\menus\\missiles.dds") ) );
		auto tex3 = std::shared_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\textures\\menus\\molecule.dds") ) );

		// Configure 3D Menu
		m_menu3d = std::make_unique<ui::menu_3d>(&m_device, m_fontLibrary->Get(L"title"));
		
		// Configure the 3D menu options
		auto item0 = m_menu3d->Root()->AddChild( math::matrix::CreateTranslation(10.f, 0.f, 0.f), tex0, L"Credits" );
		item0->OnPressed() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Credits Pressed!"); };
		item0->OnReleased() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Credits"); };

		auto item1 = m_menu3d->Root()->AddChild(math::matrix::CreateTranslation(-10.f, 0.f, 0.f), tex1, L"Video Settings");
		item1->OnPressed() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Video Pressed!"); };
		item1->OnReleased() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Video Settings"); };

		auto item2 = m_menu3d->Root()->AddChild(math::matrix::CreateTranslation(0.f, 0.f, 10.f), tex2, L"Audio Settings");
		item2->OnPressed() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Audio Pressed!"); };
		item2->OnReleased() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"Audio Settings"); };

		auto item3 = m_menu3d->Root()->AddChild(math::matrix::CreateTranslation(0.f, 0.f, -10.f), tex3, L"New Game");
		item3->OnPressed() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"New Game Pressed!"); };
		item3->OnReleased() += [&](void*, ui::menu_3d::item& i) { i.SetTitle(L"New Game"); };

		auto label = m_container->Add<ui::label>();
		label->SetText(L"Press Left/Right Arrows to Cycle the 3D Menu");
		label->SetPosition(math::vector2(0.02f, 0.9f), ui::control::eUnit::Percent);
		
}

	void menu3d::Update()
	{ 
		const math::vector2& mousePosition = m_mouse->GetPosition();
		m_cursor->SetPosition(mousePosition, ui::control::Pixels);

		std::wstringstream mouseCoordinates;
		mouseCoordinates << L"X: " << mousePosition.x() << L" Y: " << mousePosition.y();
		m_mouseCoordinates->SetText(mouseCoordinates.str());

		framework::Update(); 	
	}

	void menu3d::Update(float deltaTime)
	{
		m_menu3d->Update(m_camera, deltaTime, *m_inputState);

		m_inputState->Update(deltaTime);
		m_camera.Update(deltaTime);

		m_container->HandleInput(deltaTime, *m_inputState);
		m_container->Update(deltaTime);


		m_cursor->Update(deltaTime);
	}

	void menu3d::Render()
	{
		m_container->Draw(m_camera);
		m_menu3d->Draw(m_camera);
		m_cursor->Draw();
	}
}