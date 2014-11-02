#include "positional.h"
#include "engine.core/game.h"
#include "engine.core/runtime_variable.h"
#include "world.h"
#include "player.h"

#include "Effects.h"

namespace ui
{

void positional_view::Create()
{
	auto device = m_element.GetCore()->GetDevice();
	m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext()));
	
	m_iconSpriteSheet = std::unique_ptr<render::spritesheet>( new render::spritesheet(device, game::GetContentPath(L"\\content\\sprites\\minimap\\icons2.dds")));
	
	auto& defaultAnim = m_iconSpriteSheet->CreateAnimation(L"default", m_animations[0]);
	defaultAnim.AddFrame(math::rectangle(16,0,16,16), 0.f);

	auto& playerAnim = m_iconSpriteSheet->CreateAnimation(L"player", m_animations[1]);
	playerAnim.AddFrame(math::rectangle(32,0,16,16), 0.f);

	auto& higherAnim = m_iconSpriteSheet->CreateAnimation(L"higher", m_animations[2]);
	higherAnim.AddFrame(math::rectangle(48,0,16,16), 0.f);

	auto& lowerAnim = m_iconSpriteSheet->CreateAnimation(L"lower", m_animations[3]);
	lowerAnim.AddFrame(math::rectangle(64,0,16,16), 0.f);

	m_iconSprite = m_iconSpriteSheet->Create();

	positional_element& element = static_cast<positional_element&>(m_element);
	element.World().GetEntityPositions(m_entities);

	m_font = element.GetCore()->FontLibrary()->Get(L"title");

	m_primitiveBatch = std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(device->GetImmediateContext()));
	
	m_ellipse = math::ellipse(device->GetViewport().Center(), math::vector2((device->GetViewport().Width() * 0.5f)-10.f, (device->GetViewport().Height()*0.5f)-10.f));
}

void positional_view::Draw()
{
	positional_element& element = static_cast<positional_element&>(m_element);
	auto& playerPosition = element.Player().Position();

	auto device = m_element.GetCore()->GetDevice();
	auto& camera = element.Player().Camera();

	const auto playerForward = camera.World().Forward();
	float playerAngle = math::HalfPi + atan2f(-playerForward.z(), playerForward.x());
	
	
	math::matrix2 rotationMatrix = math::matrix2::CreateRotation(math::HalfPi);
	math::matrix m = math::matrix::CreateRotationX(math::HalfPi);
	auto viewport = device->GetViewport();
	math::vector2 screenPosition;

	for (auto& entity : element.World().Entities())
	{
		if (entity->Type() == game::entity::EntityType::Player)
			continue;

		auto entityPos2D = entity->Position() * math::vector3(1.f, 0.f, 1.f);
		auto playerPos2D = playerPosition * math::vector3(1.f, 0.f, 1.f);
		math::vector3 dirToEntity3D = (entity->Position() - playerPosition);
		float distance = dirToEntity3D.Length();
		dirToEntity3D.Normalize();

		math::matrix2 rotationMatrix = math::matrix2::CreateRotation(-playerAngle);

		// If the angle between the player's view and the direction to the entity is greater than or equal to 90 degrees then we 
		// can ignore this entity, we can't see it.
		// Alternatively, we could choose to display the icon anyway but constrain it within the viewport.
		float dot = playerForward.Dot(dirToEntity3D);
		float horizontalFOV = camera.HorizontalFieldOfView() * 0.5f;
		if (dot <= horizontalFOV)
		{
			continue;
		}
		else
		{
			screenPosition = math::vector2(device->GetViewport().Project(entity->Position(), camera.Projection(), camera.View(), math::matrix::Identity));
		}

		std::wstring distanceStr = helper::stringutils::FormatStringW(L"%.2f m", distance);
		math::vector2 textExtents = math::vector2(element.GetCore()->FontLibrary()->Get(L"title")->MeasureString(distanceStr.c_str()));
		m_spriteBatch->Begin();
		{
			m_font->DrawString(m_spriteBatch.get(), distanceStr.c_str(), screenPosition - (textExtents * 0.5f), render::color::YELLOW);
		}
		m_spriteBatch->End();
		
		m_iconSprite->Play(m_animations[2]);

		auto frameSize = m_iconSprite->FrameSize() / 2;
		m_iconSprite->Draw(screenPosition - frameSize, entity->Color());// entity->Color());
	}

}

bool positional_controller::HandleInput(float deltaTime, const input::input_state& inputState)
{
	UNREFERENCED(deltaTime);
	UNREFERENCED(inputState);

	
	return false;
}

} // ui