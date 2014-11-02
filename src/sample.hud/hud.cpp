#include "hud.h"
#include "player.h"
#include "world.h"
#include <engine.ui/label.h>
#include <engine.core/string_helper.h>
#include <engine.ui/font_library.h>

hud::hud(std::shared_ptr<ui::core_ui> core, game::player& player, game::world& world)
	: m_core(core)
	, m_player(player)
	, m_world(world)
{
	m_core->FontLibrary()->Load(L"title", game::GetContentPath(L"\\content\\fonts\\exo.font"));

	m_positional = std::unique_ptr<ui::positional_element>( new ui::positional_element(m_player, m_world) );
	m_positional->Create(m_core);

	m_minimap = std::unique_ptr<ui::minimap_element>( new ui::minimap_element(m_player, m_world) );
	m_minimap->Create(m_core);

	m_spriteBatch = std::shared_ptr<DirectX::SpriteBatch>(new DirectX::SpriteBatch(m_core->GetDevice()->GetImmediateContext()));
	m_orientation = std::unique_ptr<ui::label>(new ui::label(m_core, m_core->FontLibrary()->Get(L"title"), m_spriteBatch));
	m_orientation->SetPosition(math::vector2(10.f, 300.f), ui::control::Pixels);
	m_orientation->ShowBackground() = false;

}

void hud::Update(float deltaTime)
{
	m_orientation->Update(deltaTime);

	m_minimap->Update(deltaTime);
	m_positional->Update(deltaTime);

	auto forward = m_player.Camera().World().Forward();
	float angle = atan2f(forward.x(), forward.z());
	m_orientation->SetText(helper::stringutils::FormatStringW(L"Angle: %.2f", math::RadiansToDegrees(angle+math::HalfPi)));
}

bool hud::HandleInput(float deltaTime, input::input_state& inputState)
{
	return m_minimap->HandleInput(deltaTime, inputState);
}

void hud::Draw(render::camera& camera)
{

	m_minimap->Draw();
	m_positional->Draw();

	m_spriteBatch->Begin();
	m_orientation->Draw(camera);
	m_spriteBatch->End();

}



