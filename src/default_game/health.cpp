#include "health.h"

#include "CommonStates.h"

#include "engine.ui/label.h"
#include "engine.ui/progressbar.h"

#include "engine.math/vector.h"

#include "engine.core/game.h"

namespace ui
{

void element_health::Update(float deltaTime)
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

void element_health::Draw()
{
	if ( m_view != nullptr )
	{
		m_view->Draw();
	}
}

View_Health::View_Health(element& element)
: view(element)
, m_spriteBatch(nullptr)
, m_spriteFont(nullptr)
, m_label(nullptr)
{
	
}

void View_Health::Create()
{
	auto device = m_element.GetCore()->GetDevice()->GetDevice();
	auto deviceContext = m_element.GetCore()->GetDevice()->GetImmediateContext();


	m_spriteBatch = std::shared_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch( deviceContext ) );
	m_spriteFont = std::shared_ptr<render::platform::font>(new render::platform::font(device, game::GetContentPath(L"\\content\\fonts\\8bit.font").c_str()));

	m_label = std::unique_ptr<label>(new label( m_element.GetCore(), m_spriteFont, m_spriteBatch));
	m_label->Alpha() = 0.8f;
	m_label->SetText(TEXT("Health"));
	m_label->SetPosition(math::vector2(0.01f, 0.01f));

	math::vector2 labelSize = m_label->GetTextSize();
	math::vector2 v = math::vector2( m_label->Position().x() + labelSize.x() + 8.f, m_label->Position().y());

	m_progressbar = std::unique_ptr<progress_bar>(new progress_bar(m_element.GetCore(), nullptr, m_spriteBatch));
	m_progressbar->SetPosition(v, control::Pixels);
	m_progressbar->SetSize(math::vector2(100.f, labelSize.y()));
	m_progressbar->SetBackgroundColor(render::color::BLACK);
	m_progressbar->SetForegroundColor(render::color::GREEN);
	m_progressbar->SetMaximum(100.f);
	m_progressbar->SetValue(25.f);
	
	m_progressbar->OnFocusReceived() += [&] (void*, const control&) { m_progressbar->SetForegroundColor(render::color::RED); };
	m_progressbar->OnFocusLost() += [&] (void*, const control&) { m_progressbar->SetForegroundColor(render::color::GREEN); };
	m_progressbar->OnMouseEnter() += [&](void*, const control&) { m_progressbar->SetForegroundColor(render::color::YELLOW); };
	m_progressbar->OnMouseLeave() += [&](void*, const control&) { m_progressbar->SetForegroundColor(render::color::CYAN); };

	m_progressbar->SetCustomDrawFunction(
	[=](const control& c){
		const progress_bar& progressbar = static_cast<const progress_bar&>(c);
		math::rectangle r = progressbar.Rectangle() + math::rectangle(5.f,0,0.f, 0.f);
		math::rectangle rc = progressbar.Rectangle();
		rc.Left() -= 10.f;
		rc.Width() += 20.f;
		auto whiteTexture = *m_element.GetCore()->GetWhiteTexture()->GetView();
		m_spriteBatch->Draw(whiteTexture, r, nullptr, render::color::DARKCYAN);
		m_spriteBatch->Draw(whiteTexture, r, nullptr, progressbar.ForegroundColor());
	});

	//m_button = std::unique_ptr<button>(new button(m_element.GetCore(), m_spriteFont, m_spriteBatch));
	//m_button->SetPosition(math::vector2(100.f,100.f));
	//m_button->SetSize(math::vector2(128.f, 64.f));
	//m_button->SetTexture(ui::button::eState::Default, TEXT("D:\\development\\2014\\trunk\\spheregames\\ui_book\\art\\button_00.dds"));
	 
	m_spritesheet = std::unique_ptr<render::spritesheet>( new render::spritesheet(m_element.GetCore()->GetDevice(), game::GetContentPath(L"\\content\\sprites\\sprite00.dds")));
	
	m_button = std::unique_ptr<ui::button>(new ui::button(m_element.GetCore(), m_spriteFont, m_spriteBatch));	
	m_button->SetSize(math::vector2(160.f, 160.f), ui::control::Pixels);
	m_button->SetPosition(math::vector2(1000.f, 200.f), control::Pixels);
	m_button->SetText(L"Start Game!");
	m_button->ShowText() = false;
//	m_button->OnReleased() += [&](const ui::button&) { Update(0.f); };
	//m_button->Load(game::GetContentPath(L"\\content\\sprites\\buttons\\fancy_button.dds").c_str(), 200, 200);

	m_listbox = std::unique_ptr<ui::listbox>(new ui::listbox(m_element.GetCore(), m_spriteFont, m_spriteBatch));
	m_listbox->SetSize(math::vector2(400.f, 230.f), ui::control::Pixels);
	m_listbox->SetPosition(math::vector2(200.f, 300.f), control::Pixels);
//	m_listbox->MultipleSelection() = true;

	//ui::listbox::listboxitem<ui::label> test(m_element.GetCore(), m_spriteFont, m_spriteBatch);
	//auto test = m_listbox->Create<ui::label>();
	//test->SetText(L"Label 1");
	//test->SetSize(math::vector2(80.f, 40.f));

	//auto test2 = m_listbox->Create<ui::button>();
	//test2->SetText(L"Button 1");
	//test2->SetSize(math::vector2(150.f, 40.f));

	wchar_t buf[200];
	for (int i =0; i < 30; ++i )
	{
		swprintf_s(buf, 200, L"hello %d", i);
		//auto test3 = m_listbox->CreateText(buf);
		//auto test3 = m_listbox->Create<ui::label>();
		//test3->SetText(L"Label 1");
		//test3->SetSize(math::vector2(80.f, 40.f));
	}

	/*m_listbox->Add(L"Item 1");
	m_listbox->Add(L"Item 2");
	m_listbox->Add(L"Item 3");
	m_listbox->Add(L"Item 4");
	m_listbox->Add(L"Item 5");
	m_listbox->Add(L"Item 6");
	m_listbox->Add(L"Item 7");
	m_listbox->Add(L"Item 8");
	m_listbox->Add(L"Item 9");*/

///	render::spritesheet::animation& anim = m_spritesheet->CreateAnimation(TEXT("start")); 
	math::rectangle rc = math::rectangle( 0, 0, 100, 150 );

	/*anim.AddFrame(rc, 0.1f);
	
	rc.Left() = 100;
	rc.Width() = 100;
	anim.AddFrame(rc, 0.1f);

	rc.Left() = 200;
	rc.Width() = 120;
	anim.AddFrame(rc, 0.1f);
	
	rc.Left() = 320;
	rc.Width() = 100;
	anim.AddFrame(rc, 0.1f);
	
	rc.Left() = 422;
	rc.Width() = 80;
	anim.AddFrame(rc, 0.1f);
	
	rc.Left() = 510;
	rc.Width() = 114;
	anim.AddFrame(rc, 0.1f);

	render::spritesheet::animation& anim2 = m_spritesheet->CreateAnimation(TEXT("second")); 
	rc.Top() = 256;
	rc.Bottom(rc.Top() + 64);
	rc.Left() = 0;
	rc.Right(rc.Left() + 64);

	auto& f1 = anim2.AddFrame(rc, 0.1f);
	rc.Left() = 64;
	rc.Right(rc.Left() + 64);

	auto& f2 = anim2.AddFrame(rc, 0.1f);	
	rc.Left() = 128;
	rc.Right(rc.Left() + 64);

	auto& f3 = anim2.AddFrame(rc, 0.1f);*/


//	m_spritesheet->Play(TEXT("start"));

	m_testPrim = DirectX::GeometricPrimitive::CreateCube(m_element.GetCore()->GetDevice()->GetImmediateContext());
	//m_testPrim = DirectX::GeometricPrimitive::CreateTeapot(m_element.GetCore()->GetDevice()->GetImmediateContext());



}
static float y = 0.f;

void View_Health::Update(float deltaTime)
{
	y += deltaTime * 2.f;


	m_listbox->HandleInput(deltaTime, m_element.GetCore()->GetInputState());
	m_listbox->Update(deltaTime);
	
	m_progressbar->HandleInput(deltaTime, m_element.GetCore()->GetInputState());

	m_button->Update(deltaTime);
	m_button->HandleInput(deltaTime, m_element.GetCore()->GetInputState());
}


void View_Health::Draw()
{
	element_health& element = static_cast<element_health&>(m_element);
		
	auto device = m_element.GetCore()->GetDevice();

	DirectX::CommonStates states(device->GetDevice());

	static float scale = 1.f;
	//y = 3.14f / 4.f;
	math::matrix world = math::matrix::Identity;
	//math::matrix world = math::matrix::Identity;
	//world =  math::matrix::CreateRotationY(y);
	
	static float zdist = 10.0f;
	
	world.Scale(scale);

	auto& camera = element.Player().Camera();
	auto forward = camera.Forward();

	auto v = element.Player().Camera().Position() + math::vector3::UnitZ * zdist;
	//auto v = element.Player().Camera().Position() + element.Player().Camera().Forward() * zdist;;
	world.Translate(v);

	m_testPrim->Draw(world, device->GetViewMatrix(), device->GetProjectionMatrix(), render::color::GREEN, nullptr, false, [&] {
		device->GetImmediateContext()->RSSetState(states.CullNone());
	});

	v = element.Player().Camera().Position() + math::vector3::UnitZ * -zdist;
	//auto v = element.Player().Camera().Position() + element.Player().Camera().Forward() * zdist;;
	world = math::matrix::Identity;
	world.Translate(v);


	m_testPrim->Draw(world, device->GetViewMatrix(), device->GetProjectionMatrix(), render::color::BLUE, nullptr, false, [&] {
		device->GetImmediateContext()->RSSetState(states.CullNone());
	});
	
	v = element.Player().Camera().Position() + math::vector3::UnitX * zdist;
	//auto v = element.Player().Camera().Position() + element.Player().Camera().Forward() * zdist;;
	world = math::matrix::Identity;
	world.Translate(v);


	m_testPrim->Draw(world, device->GetViewMatrix(), device->GetProjectionMatrix(), render::color::RED, nullptr, false, [&] {
		device->GetImmediateContext()->RSSetState(states.CullNone());
	});

	v = element.Player().Camera().Position() + math::vector3::UnitX * -zdist;
	//auto v = element.Player().Camera().Position() + element.Player().Camera().Forward() * zdist;;
	world = math::matrix::Identity;
	world.Translate(v);

	
	m_testPrim->Draw(world, device->GetViewMatrix(), device->GetProjectionMatrix(), render::color::YELLOW, nullptr, false, [&] {
		device->GetImmediateContext()->RSSetState(states.CullNone());
	});
	
	//m_testPrim->Draw(world, math::matrix::Identity,  
	//	DirectX::XMMatrixPerspectiveOffCenterLH(0,1280, 720, 0, 0.1, 10000.f));

	//m_testPrim->Draw(world, math::matrix::Identity,  
	//	device->GetProjectionMatrix()); // works

							//DirectX::XMMatrixPerspectiveLH(1280, 720, 0.01f, 2000.f));

	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.AlphaBlend());
	
	m_label->Draw(camera);

	float health = element.GetHealth();
	float maxHealth = element.GetMaxHealth();
	float integrity = (maxHealth > 0.f) ? health / maxHealth : 0.f;

	m_progressbar->SetRatio(integrity);
	m_progressbar->Draw(camera);


	m_button->Draw(camera);
	
	m_listbox->Draw(camera);

	m_spriteBatch->End();

//	m_spritesheet->Draw(160, 40, render::color::WHITE, 0.f, math::vector2::Zero);

	
}

} // ui