#include "stdafx.h"
#include <random>

#include <memory>
#include "example_game.h"
#include "engine.math/math.h"

#include "engine.ui/core.h"
#include "engine.core/input.h"

#include "damage_indicator.h"
#include "world_indicator.h"
#include "compass.h"
#include "minimap.h"

#include "engine.ui/heatmap.h"

#include "engine.core/runtime_variable.h"

game::WorldProperties worldProperties(math::vector3::UnitZ);

runtime_variable_function g_testFunction(L"parse", L"function to test parsing arguments", [](std::vector<std::wstring> args) -> bool
{
	if ( args.size() != 0 )
	{
		bool value = false;
		if ( runtime_variable::ParseBool(args[0].c_str(), value) )
		{
			std::cout << "test succesful" << std::endl; 
			return true;
		}
	}
	std::cout << "test unchanged" << std::endl; 
	return false;
}
);

example_game::example_game(HWND& windowHandle, int clientWidth, int clientHeight)
	: game::core(windowHandle, clientWidth, clientHeight)
	, m_player(nullptr)
	, m_world(worldProperties)
	, m_memberFunction(L"test", L"function to test parsing arguments", std::bind(&example_game::OnTest, this, std::placeholders::_1))
{
	m_timer.Start();
	m_currentTime = m_timer.GetMilliseconds();

}

bool example_game::OnTest(std::vector<std::wstring> parameters)
{
	std::wstring s = L"Succesfully called function";
	m_console->AddItem(s, render::color::SEAGREEN);
	return true;
}

example_game::render_settings::render_settings()
	: m_stereoscopic(false) 
{}


example_game::~example_game()
{	
}

void example_game::Initialize()
{
	m_device.Create(m_windowHandle);

	m_player = std::unique_ptr<game::Player>( new game::Player(*this) );

	render::viewport vp(0,0, (float)ClientWidth(), (float)ClientHeight(), 0.f, 1.f);
	SetViewport(vp);

	m_camera = render::camera(vp);
	m_cameraOVR = render::camera_ovr(vp);	

	m_inputState = std::shared_ptr<input::input_state>(new input::input_state(m_windowHandle));
	m_mouse = m_inputState->CreateMouse();
	m_keyboard = m_inputState->CreateKeyboard();

	m_coreUI = std::shared_ptr<ui::core_ui>(new ui::core_ui(&m_device, *m_inputState));

	m_console = std::unique_ptr<ui::console>(new ui::console(m_coreUI));

	m_testCube = std::unique_ptr<render::cube>(new render::cube(&m_device));


	m_primitiveBatch = std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>( new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(m_device.GetImmediateContext()) );
	m_spriteBatch = std::shared_ptr<render::platform::sprite_batch> ( new render::platform::sprite_batch(m_device.GetImmediateContext()) );

//	DirectX::CreateDDSTextureFromFile(m_device.GetDevice(), TEXT("d:\\axe.dds"), &m_texture, &m_textureView);

	m_health = std::unique_ptr<ui::element_health>(new ui::element_health(*m_player));
	m_health->Create(m_coreUI);

	m_cursor = std::unique_ptr<ui::input_cursor>(new ui::input_cursor(m_coreUI, nullptr, nullptr, game::GetContentPath(L"\\content\\sprites\\cursor.dds")));
	

	m_testQuad = std::unique_ptr<render::quad>(new render::quad(&m_device));

	m_ovr = std::unique_ptr<render::ovr>(new render::ovr(&m_device));
	
	m_worldIndicators = std::unique_ptr<ui::element_world_indicator>(new ui::element_world_indicator());
	m_worldIndicators->Create(m_coreUI);

	m_worldIndicators->Add(math::vector3(100.f, 0.f, -10.f));
	m_worldIndicators->Add(math::vector3(0.f, 0.f, 10.f));

	m_worldIndicators->Add(math::vector3(100.f, 0.f, -10.f));
	m_worldIndicators->Add(math::vector3(0.f, 0.f, -3.f));

	m_damageIndicator = std::unique_ptr<ui::element_damage_indicator>(new ui::element_damage_indicator());
	m_damageIndicator->Create(m_coreUI);

	m_damageIndicator->DamageEvent(math::vector3(0.f,0.f,-1.f));
	m_damageIndicator->DamageEvent(math::vector3(0.f,0.f, 1.f));

//	m_damageIndicator->DamageEvent(math::vector3(-1.f,0.f,0.f));
//	m_damageIndicator->DamageEvent(math::vector3(1.f,0.f,0.f));

	m_damageIndicator->DamageEvent(math::vector3(0.f,1.f,0.f));
	m_damageIndicator->DamageEvent(math::vector3(0.f,-1.f,0.f));

	/*m_damageIndicator->DamageEvent(math::vector3(-0.5,0,0));
	m_damageIndicator->DamageEvent(math::vector3(1,0,0));
	m_damageIndicator->DamageEvent(math::vector3(-1,0,0));
	m_damageIndicator->DamageEvent(math::vector3(0,1,0));
	m_damageIndicator->DamageEvent(math::vector3(0,-1,0));

	m_damageIndicator->DamageEvent(math::vector3(0,1,1));
	m_damageIndicator->DamageEvent(math::vector3(0,-1,0));

	m_damageIndicator->DamageEvent(math::vector3(0.43,0.22,1));*/

	m_minimap = std::unique_ptr<ui::element_minimap>( new ui::element_minimap(*m_player, m_world) );
	m_minimap->Create(m_coreUI);

	m_compass = std::unique_ptr<ui::element_compass>( new ui::element_compass(*m_player, m_world) );
	m_compass->Create(m_coreUI);
	//m_compass->AddItem(L"south", math::vector3(0.4f, 0.f, 1.f));
	//m_compass->AddItem(L"south", math::vector3(-0.4f, 0.f, -1.f));
	//
	//m_compass->AddItem(L"south", math::vector3(-44.f, 4444.f, -343.f));
	//m_compass->AddItem(L"south", math::vector3(100.f, 0.f, 120.f));
	//m_compass->AddItem(L"south", math::vector3(500.f, 10.f, 20.f));
	
	m_compass->AddItem(L"east", math::vector3( 10.f, 0.f, 0.f));

	m_compass->AddItem(L"north", math::vector3(0.f, 0.f, -10.f));
	
	m_compass->AddItem(L"west", math::vector3(-10.f, 0.f, 0.f));
		
	m_compass->AddItem(L"south", math::vector3(0.f, 0.f, 10.f));
	
	m_renderSettings.m_stereoscopic = false;
	
	m_cameraRT = std::unique_ptr<render::camera_rt>(new render::camera_rt(&m_device, vp));
	//m_cameraRT->ClearColor() = render::color::TRANSPARENT;
	m_cameraRT->LookAt( math::vector3(15.f, 4.f, -2.6f), math::vector3(0,0,0), math::vector3(0,1,0));

	
	std::shared_ptr<render::platform::font> spriteFont = std::shared_ptr<render::platform::font>(new render::platform::font(m_device.GetDevice(), game::GetContentPath(L"\\content\\fonts\\8bit.font").c_str()));
	
	m_uiContainer = std::unique_ptr<ui::container>((new ui::container(m_coreUI, spriteFont)));
	m_uiContainer->SetPosition(math::vector2(0.5f, 0.5f), ui::control::Percent);
	m_uiContainer->SetSize(math::vector2(200.f, 200.f), ui::control::Pixels);

	auto lbl = m_uiContainer->Add<ui::label>();	
	lbl->SetText(L"Label Test");

	auto lbl2 = m_uiContainer->Add<ui::label>();	
	lbl2->SetText(L"Label Test 2");
	lbl2->SetPosition(math::vector2(-0.1f, 0.5f), ui::control::Percent);
	lbl2->SetSize(math::vector2(300.f, 300.f), ui::control::Pixels);
	lbl2->SetAlignment((ui::align::eAlignmentFlags)(ui::align::Left | ui::align::Middle));

	auto btn = m_uiContainer->Add<ui::button>();
//	btn->Load(game::GetContentPath(L"\\content\\sprites\\button.dds").c_str(), 103, 93);
	btn->SetText(L"Hello Button!");
	btn->SetPosition(math::vector2(0.f, 0.3f), ui::control::Percent);
	btn->SetSize(math::vector2(200.f, 60.f), ui::control::Pixels);

	m_text = std::unique_ptr<render::text>( new render::text(&m_device, L"Hello World!", spriteFont, math::vector3(0,0,10) ));
	m_text->ViewAlign() = false;
	//m_text->Position() = math::vector3::Zero;
	m_text->Normal() = math::vector3::UnitX;

	// TODO:
	// Create a render target
	// Set the projection parameters to orthographic?
	// Render a sprite batch (that uses a sprite font) into it
	// Apply render target as input to quad, resized to fit text.
	// Will need to measure the text, and create the nearest power of two texture
	// Calculate UVs to properly fit the text only.

	

	ui::heatmap heatmap(&m_device, 256, 8, game::GetContentPath(L"\\content\\sprites\\heatmap\\heatmap_color_0.dds"));
	for (int i = 0; i < 3000; ++i )
	{
		int y = rand()%255;
		int x = rand()%255;
		heatmap.AddPoint(math::point(x, y));
	}
	//heatmap.AddPoint(math::vector2(-16, 10));

	heatmap.Colorize();

	heatmap.Save(L"f:\\heatmap.dds");
	

	m_transform = math::matrix::Identity;

	auto analogTexture = std::shared_ptr<render::texture>( std::unique_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\sprites\\analog\\arrow.dds"))));
	
		//auto analogTexture = std::shared_ptr<render::texture>( std::unique_ptr<render::texture>( new render::texture(&m_device, game::GetContentPath(L"\\content\\sprites\\uvtest.dds"))));
	m_analog = std::unique_ptr<ui::analog>( new ui::analog(&m_device, analogTexture) );
	m_analog->ScreenPosition() = math::vector3(5.f, 100.f, 0.f);
}


void example_game::SetProjection(const math::matrix& projection)
{
	m_projection = projection;
}

void example_game::Update()
{

	double newTime = m_timer.GetMilliseconds();
	double frameTime = newTime - m_currentTime;

	m_console->Update(static_cast<float>(frameTime));
	m_console->HandleInput(static_cast<float>(frameTime), m_coreUI->GetInputState());

	m_uiContainer->HandleInput(static_cast<float>(frameTime), m_coreUI->GetInputState());
	m_uiContainer->Update(static_cast<float>(frameTime));

	
	static float y = 0.f;

	
	static bool up = true;
	static float r = 0.f;
	m_analog->Ratio() = r;
	r += (up) ? 0.001f : -0.001f;
	if ( r >= 1.f || r <= -1.f ) 
		up = !up;

	auto& inputState = m_coreUI->GetInputState();
	auto keyboard = inputState.GetKeyboard();
	if ( keyboard != nullptr )
	{
		if ( keyboard->KeyDown(DIK_UP) )
		{
			//m_player->Position() += ( m_player->Camera().Forward() * 0.1f );
			m_player->Camera().MoveForward(1.f);
		}

		if ( keyboard->KeyDown(DIK_DOWN) )
		{
			//m_player->Position() -= ( m_player->Camera().Forward() * 0.1f );
			m_player->Camera().MoveBack(1.f);
		}

		if ( keyboard->KeyDown(DIK_LEFT) )
		{
			y += frameTime * 1.2f;
			m_player->Camera().Rotate(0.f, y, 0.f);
			//m_player->Rotate(y);
		}
		else
		if ( keyboard->KeyDown(DIK_RIGHT) )
		{
			y -= frameTime * 1.2f;
			m_player->Camera().Rotate(0.f, y, 0.f);
			//m_player->Rotate(y);
		}
	}

  	m_player->Update((float)frameTime);
	//m_player->Rotate(y);

	

	static float xx = 0.f;
	float x = -1 + ((float)rand()/RAND_MAX)*2;
	float z = -1 + ((float)rand()/RAND_MAX)*2;
	//xx += 0.001f;
	//x = sin(x);
	//z = cos(z);
	
	// bug: if I set z, on y this works- I suspect it's the vector/matrix transform
	m_damageIndicator->DamageEvent(math::vector3(x, 0.f, z));
	xx += 0.1f;

	static float yy = -1.f;
	yy += (float)frameTime * 0.5f;
	if ( yy > 5.f ) yy = -5.f;
	//m_player->Rotate(y);

	m_text->Normal() = math::vector3(0.f, 0.f, 1.f);

	m_currentTime = newTime;

	m_analog->Update(static_cast<float>(frameTime));

	m_damageIndicator->Update((float)frameTime);
	m_worldIndicators->Update((float)frameTime);
	m_compass->Update((float)frameTime);
	m_minimap->Update((float)frameTime);
	m_minimap->HandleInput((float)frameTime, m_coreUI->GetInputState());

	Update(frameTime);

	PreRender();

	if ( m_renderSettings.m_stereoscopic )
	{
		render::viewport vp(0,0, (float)ClientWidth() / 2, (float)ClientHeight(), 0.01f, 1.f);
		m_device.SetViewport(vp);

		const float hScreenSize = 0.14976f;
		const float vScreenSize = 0.936f;

		const float eyeToScreenDistance = 0.041f;
		const float interpupillaryDistance = 0.064f;

		float aspectRatio = ((float)ClientWidth() * 0.5f) / (float)ClientHeight();
		float halfScreenDistance = vScreenSize / 2;
		float yfov = 2.f * atan(halfScreenDistance / eyeToScreenDistance);

		float viewCenter = hScreenSize * 0.25f;
		float eyeProjectionShift = viewCenter - interpupillaryDistance*0.5f;
		float projectionCenterOffset = 4.f * eyeProjectionShift / hScreenSize;

		math::matrix projectionCenter = math::matrix( DirectX::XMMatrixPerspectiveFovLH(yfov, aspectRatio, 0.1f, 1000.f) );
		math::matrix projectionLeft = math::matrix::CreateTranslation(projectionCenterOffset, 0.f, 0.f) * projectionCenter;
		math::matrix projectionRight = math::matrix::CreateTranslation(-projectionCenterOffset, 0.f, 0.f) * projectionCenter;

		float halfIPD = interpupillaryDistance * 0.5f;
		math::matrix viewLeft = math::matrix::CreateTranslation(halfIPD, 0.f, 0.f) * viewCenter;
		math::matrix viewRight = math::matrix::CreateTranslation(-halfIPD, 0.f, 0.f) * viewCenter;

		/*struct vsConstantBuffer {
			math::vector2 m_lensCenter;
			math::vector2 m_screenCenter;
			math::vector2 m_scale;
			math::vector2 m_scaleIn;
			math::vector4 m_distortionAmount;
			math::matrix m_view;
			math::matrix m_tex;

		};
		auto view = m_cameraOVR.GetView(render::camera_ovr::Right) * m_player->Camera().view();
		vsConstantBuffer constantBufferData = { 
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector4::Zero,
			view,
			math::matrix::Identity
		};

		std::unique_ptr<render::buffer> constantBuffer = std::unique_ptr<render::buffer>( new render::buffer(&m_device) );
		constantBuffer->Data(render::eBufferUsage::Constant, &constantBufferData, sizeof(constantBufferData));

		auto& vertexShader = m_ovr->GetVertexShader();
		vertexShader->SetConstantBuffer(constantBuffer.get(), 0);
		m_ovr->GetVertexShader()->Set();
		m_ovr->GetPixelShader()->Set();*/

		m_device.SetViewport(m_cameraOVR.GetViewport(render::camera_ovr::Right));
		m_device.SetProjectionMatrix(m_cameraOVR.GetProjection(render::camera_ovr::Right));
		m_device.SetViewMatrix(m_cameraOVR.GetView(render::camera_ovr::Right) * m_player->Camera().View() );

		Render();

		render::viewport vp2( (float)ClientWidth() / 2, 0, (float)ClientWidth(), (float)ClientHeight(), 0.01f, 1.f);
		SetViewport(vp2);

		m_device.SetViewport(m_cameraOVR.GetViewport(render::camera_ovr::Left));
		m_device.SetProjectionMatrix(m_cameraOVR.GetProjection(render::camera_ovr::Left));
		m_device.SetViewMatrix(m_cameraOVR.GetView(render::camera_ovr::Left)* m_player->Camera().View());

		Render();
	}
	else
	{

		m_device.SetViewMatrix(m_player->Camera().View());
		m_device.SetProjectionMatrix(m_player->Camera().Projection());


		Render();
	}

	Present();

}

void example_game::Draw()
{
	m_damageIndicator->Draw();
	m_worldIndicators->Draw();
	m_compass->Draw();
}

void example_game::Update(double deltaTime)
{
	static math::vector3 campos = math::vector3(0.f, 0.f, 10.f);
	//m_camera.LookAt(campos, math::vector3(0.f, 0.5f, 0.f), math::vector3::UnitY);
	m_cameraOVR.LookAt(campos, math::vector3(0.f, 0.0f, 0.f), math::vector3::UnitY);


	//m_player->SetCamera(m_camera);

	static float y = 0.f;
	y += (float)deltaTime;
	//if ( y >= math::TwoPi )
	//	y = 0.f;

	//m_player->Camera().LookAt(campos, math::vector3::Zero, math::vector3::UnitY);
	//m_player->Rotate(y);

	//m_camera.LookAt(m_transform.Translation(), m_transform.Translation() + (m_transform.Forward() * 2.f), math::vector3::UnitY);

	m_inputState->Update((float)deltaTime);
	
	m_health->Update((float)deltaTime);
	
	static math::vector2 test;

	int mouseX, mouseY, mouseZ;
	m_mouse->GetRelativePosition(mouseX, mouseY, mouseZ);
	math::vector2 mousePosition((float)mouseX, (float)mouseY);

	test += mousePosition;

	m_cursor->SetPosition(test, ui::control::Pixels);

	
}

void example_game::PreRender()
{
	
	if ( m_renderSettings.m_stereoscopic )
	{
		
		m_ovr->GetRenderTarget()->Set(m_device.GetDepthStencilView());
		m_ovr->GetRenderTarget()->Clear(render::color::PURPLE);
	}
	else
	{
		auto context = m_device.GetImmediateContext();

		auto renderTargetView = m_device.GetRenderTargetView();
		context->OMSetRenderTargets(1, &renderTargetView, m_device.GetDepthStencilView());

    // Clear the back buffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
		context->ClearRenderTargetView( m_device.GetRenderTargetView(), ClearColor );
	
    // Clear the depth buffer to 1.0 (max depth)
		context->ClearDepthStencilView( m_device.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

	}
	//else
	//{	
	//	//
	//	// Clear the back buffer
	//	//
	//	float ClearColor[4] = { 1.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	//	m_device.GetImmediateContext()->ClearRenderTargetView( m_device.GetRenderTargetView(), ClearColor );
	//
	//	//
	//	// Clear the depth buffer to 1.0 (max depth)
	//	//
	//	m_device.GetImmediateContext()->ClearDepthStencilView( m_device.GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	//}
	
}



void example_game::Render()
{
	// TODO: This is the main loop, this should be separated into the proper stages (or even parallelized if desired), the rendering
	// should not happen directly here, but it's like that as an example.
	
	//m_testQuad->Draw();
	
	/*m_health->Draw();

	m_text->Draw(m_player->Camera().View(), m_player->Camera().Projection());
	
	m_damageIndicator->Draw();
	m_worldIndicators->Draw();
	m_compass->Draw();

	m_uiContainer->Draw();*/

	//m_analog->Draw(m_camera);

	m_uiContainer->Draw(m_camera);

	m_console->Draw(m_camera);

	m_minimap->Draw();

	//m_cursor->Draw();
	/*if ( m_cameraRT->RenderTarget() != nullptr )
	{
		m_cursor->SetTexture(m_text->RenderTarget());
	}*/

	//m_cursor->SetTexture(m_text->RenderTarget());
	m_cursor->Draw();


	m_analog->Draw(m_player->Camera());

	m_testCube->Draw(math::matrix::CreateTranslation(0,0,-10), m_player->Camera().View(), m_player->Camera().Projection(), nullptr);
	m_testCube->Draw(math::matrix::CreateTranslation(0,0, 10), m_player->Camera().View(), m_player->Camera().Projection(), nullptr);
	m_testCube->Draw(math::matrix::CreateTranslation(10,0,0), m_player->Camera().View(), m_player->Camera().Projection(), nullptr);
	m_testCube->Draw(math::matrix::CreateTranslation(-10,0,0), m_player->Camera().View(), m_player->Camera().Projection(), nullptr);
	


	/*if ( m_cameraRT->RenderTarget() != nullptr )
	{
		m_cursor->SetTexture(nullptr);
	}*/

	//m_cameraRT->Capture(std::bind(&example_game::Draw, this) );

}

struct vsConstantBuffer {
			math::vector2 m_lensCenter;
			math::vector2 m_screenCenter;
			math::vector2 m_scale;
			math::vector2 m_scaleIn;
			math::vector4 m_distortionAmount;
			math::matrix m_view;
			math::matrix m_world;
			math::matrix m_projection;
			math::matrix m_tex;
};

void example_game::Present()
{


	if ( m_renderSettings.m_stereoscopic )
	{
	m_device.GetImmediateContext()->OMSetRenderTargets(1, m_device.RenderTargetView(), m_device.GetDepthStencilView());

//	float ClearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // red, green, blue, alpha
	m_device.GetImmediateContext()->ClearRenderTargetView( m_device.GetRenderTargetView(), render::color::LIMEGREEN);

	math::rectangle r(0, 0, 1280, 720);
	//	m_device.ResetRenderTarget();

		m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [&]
	{
			//m_device.GetImmediateContext()->ClearRenderTargetView( m_device.GetRenderTargetView(), render::color::RED );
		
		vsConstantBuffer constantBufferData = { 
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector2::Zero,
			math::vector4::Zero,			
			m_device.GetViewMatrix(),
			math::matrix::Identity,
			m_device.GetProjectionMatrix(),
			math::matrix::Identity
		};

		std::unique_ptr<render::buffer> constantBuffer = std::unique_ptr<render::buffer>( new render::buffer(&m_device) );
		constantBuffer->Data(render::eBufferUsage::Constant, &constantBufferData, sizeof(constantBufferData));

		auto& vertexShader = m_ovr->GetVertexShader();




		m_device.GetImmediateContext()->PSSetShaderResources(0, 1, m_ovr->GetRenderTarget()->GetView());
		m_device.GetImmediateContext()->VSSetShader(m_ovr->GetVertexShader()->Get(), nullptr, 0);
		m_device.GetImmediateContext()->PSSetShader(m_ovr->GetPixelShader()->Get(), nullptr, 0);

		vertexShader->SetConstantBuffer(constantBuffer.get(), 0);

}
);
		m_spriteBatch->Draw(*m_ovr->GetRenderTarget()->GetView(), (RECT)r);

		math::rectangle r2(10, 10, 64, 64);
		m_spriteBatch->Draw(*m_cursor->Texture()->GetView(), (RECT)r2);
		m_spriteBatch->End();
		//m_testQuad->SetVertexShader(m_ovr->GetVertexShader());
		//m_testQuad->SetPixelShader(m_ovr->GetPixelShader());
		//m_testQuad->Draw(math::matrix::Identity, m_device.GetViewMatrix(), m_device.GetProjectionMatrix(), m_ovr->GetRenderTarget());
	}

	//m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=]
	//{
	//	m_device.GetImmediateContext()->PSSetShaderResources(0, 1, m_ovr->GetRenderTarget()->GetView());
	//	m_device.GetImmediateContext()->VSSetShader(m_ovr->GetVertexShader()->Get(), nullptr, 0);
	//	m_device.GetImmediateContext()->PSSetShader(m_ovr->GetPixelShader()->Get(), nullptr, 0);
	//}	
	//);
	/*m_spriteBatch->Begin();
	m_spriteBatch->Draw(*m_ovr->GetRenderTarget()->GetView(), math::vector2::Zero);
	m_spriteBatch->End();*/


	//
    // Present our back buffer to our front buffer
    //
	m_device.GetSwapChain()->Present( 0, 0 );
}

void example_game::SetViewport(const render::viewport& viewport)
{
	m_device.SetViewport(viewport);
}

