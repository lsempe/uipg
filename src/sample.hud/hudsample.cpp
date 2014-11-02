#include "stdafx.h"
#include "hudsample.h"

#include "engine.core/input.h"
#include "engine.ui/core.h"
#include "engine.render/platform.h"

#include "engine.render/axes.h"


// Controls
#include "engine.ui/slider.h"
#include "engine.ui/button.h"
#include "engine.ui/label.h"
#include "engine.ui/textbox.h"
#include "engine.ui/dropdown2.h"

#include "entity.h"

// TODO:
// - Add "world" (use cubes / plane)
// - Add "player"
// - Add "entities" (different shapes)vb. l   

namespace samples
{
	void hudsample::Initialize()
	{
		framework::Initialize();

		m_inputState = std::shared_ptr<input::input_state>(new input::input_state(m_windowHandle));
		m_mouse = m_inputState->CreateMouse();
		m_inputState->CreateKeyboard();

		m_ui = std::shared_ptr<ui::core_ui>(new ui::core_ui(&m_device, *m_inputState));
		
		render::viewport viewport(0,0, (float)ClientWidth(), (float)ClientHeight(), 0.f, 1.f);
				
		m_cursor = std::unique_ptr<ui::input_cursor>(new ui::input_cursor(m_ui, nullptr, nullptr, game::GetContentPath(L"\\content\\sprites\\cursor.dds")));

		auto deviceContext = m_device.GetImmediateContext();

		m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new DirectX::SpriteBatch( deviceContext ) );
		m_spriteFont = std::shared_ptr<DirectX::SpriteFont>(new DirectX::SpriteFont(m_device.GetDevice(), game::GetContentPath(L"\\content\\fonts\\exo.font").c_str()));
		//orbitron

		m_container = std::unique_ptr<ui::container>(new ui::container(m_ui, m_spriteFont));
		m_container->SetViewport(viewport);
				
		auto label = m_container->Add<ui::label>();
		label->SetPosition(math::vector2(0.f,0.f), ui::control::Percent);
		label->SetSize(math::vector2(1.f, 0.1f), ui::control::Percent);
		label->SetText(L"UIPG - HUD");
		label->SetAlignment(static_cast<ui::align::eAlignmentFlags>( ui::align::Center | ui::align::Middle ));
		label->ShowBackground() = false;
		


		m_world = std::unique_ptr<game::world>(new game::world(&m_device));
		m_player = std::shared_ptr<game::player>(new game::player(*this, &m_device));
		m_world->AddPlayer(m_player);



		//m_player->Camera().LookAt(math::vector3(0.f,150.f,-100.f), math::vector3::Zero, math::vector3::UnitY);

		//auto e0 = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device)));
		//e0->SetPosition(math::vector3(15.f, 0.f, 14.f));
		
		//auto e1 = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device)));
		//e1->SetPosition(math::vector3(10.f, 0.f, 7.f));
		
		//auto e2 = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device)));
		//e2->SetPosition(math::vector3(15.f, 0.f, 14.f));

		auto North = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		North->SetPosition(math::vector3(0.f, 0.f, 20.f));
		North->SetColor(render::color::DARKCYAN);

		/*auto South = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		South->SetPosition(math::vector3(0.f, 0.f, -20.f));
		South->SetColor(render::color::PURPLE);

		auto West = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		West->SetPosition(math::vector3(-20.f, 0.f, 0.f));
		West->SetColor(render::color::WHEAT);

		auto East = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		East->SetPosition(math::vector3(20.f, 0.f, 0.f));
		East->SetColor(render::color::FIREBRICK);


		auto CORNER_NW = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		CORNER_NW->SetPosition(math::vector3(-512.f, 0.f, 512.f));
		CORNER_NW->SetColor(render::color::MEDIUMBLUE);

		auto CORNER_NW2 = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		CORNER_NW2->SetPosition(math::vector3(-256.f, 0.f, 256.f));
		CORNER_NW2->SetColor(render::color::MAGENTA);

		auto CORNER_NE = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		CORNER_NE->SetPosition(math::vector3(512.f, 0.f, 512.f));
		CORNER_NE->SetColor(render::color::MEDIUMTURQUOISE);

		auto CORNER_SW = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		CORNER_SW->SetPosition(math::vector3(-512.f, 0.f, -512.f));
		CORNER_SW->SetColor(render::color::MEDIUMPURPLE);

		auto CORNER_SE = m_world->AddEntity(std::shared_ptr<game::entity>(new game::entity(&m_device, game::entity::EntityType::Virtual)));
		CORNER_SE->SetPosition(math::vector3(512.f, 0.f, -512.f));
		CORNER_SE->SetColor(render::color::MEDIUMSEAGREEN);*/

		/// Mouse coordinates label
		m_mouseCoordinates = m_container->Add<ui::label>();
		m_mouseCoordinates->SetPosition(math::vector2(0.05f, 0.93f), ui::control::Percent);
		m_mouseCoordinates->SetSize(math::vector2(256.f, 48.f), ui::control::Pixels);
		m_mouseCoordinates->ShowBackground() = false;


		m_player->SetColor(render::color::BLUE);
		
		m_world->AddEntity(m_player);



		m_player->SetHud(std::shared_ptr<hud>(new hud(m_ui, *m_player, *m_world)));

	}

	void hudsample::Update()
	{ 
		const math::vector2& mousePosition = m_mouse->GetPosition();
		m_cursor->SetPosition(mousePosition, ui::control::Pixels);

		std::wstringstream playerCoordinates;
		playerCoordinates << L"X: " << m_player->Position().x() << L" Y: " << m_player->Position().y() << L" Z: " << m_player->Position().z();
		m_mouseCoordinates->SetText(playerCoordinates.str());


		framework::Update(); 		
	}

	void hudsample::Update(float deltaTime)
	{
		
		m_inputState->Update(deltaTime);

		m_player->HandleInput(deltaTime, *m_inputState);

		m_container->HandleInput(deltaTime, *m_inputState);

		m_cursor->Update(deltaTime);
		m_container->Update(deltaTime);

		m_player->Update(deltaTime);

		m_world->Update(deltaTime);
		
		
		//m_device.SetViewMatrix(m_player->Camera().View());
		//m_device.SetProjectionMatrix(math::matrix(m_player->Camera().Projection()));
		
		//m_player->Camera().Update(deltaTime);

	}

	void hudsample::Render()
	{
		m_device.SetViewMatrix(m_player->Camera().View());
		m_device.SetProjectionMatrix(m_player->Camera().Projection());

		m_world->Draw();		

		m_container->Draw(m_player->Camera());

		m_spriteBatch->Begin();
			
		
		m_spriteBatch->End();

		m_cursor->Draw();

		

	}
}