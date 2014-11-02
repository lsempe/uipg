#include "minimap.h"
#include "engine.core/game.h"
#include "engine.core/runtime_variable.h"
#include "world.h"
#include "player.h"

namespace ui
{

void minimap_view::Create()
{
	auto device = m_element.GetCore()->GetDevice();
	m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext()));
	
	m_background = std::unique_ptr<render::texture>( new render::texture(device, game::GetContentPath(L"\\content\\sprites\\minimap\\background3.dds")));
	m_mask= std::unique_ptr<render::texture>(new render::texture(device, game::GetContentPath(L"\\content\\sprites\\minimap\\mask.dds")));

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
	m_iconSprite->Play(m_animations[0]);

	minimap_element& element = static_cast<minimap_element&>(m_element);
	element.World().GetEntityPositions(m_entities);

	float radius = (m_rectangle.Width()/2);
	m_ellipse = math::ellipse(m_rectangle.Center(), math::vector2(radius, radius));

}

void minimap_view::Draw()
{
	static runtime_variable_float s_radarZoom(L"radar_zoom", L"controls the zoom amount on the mini map", 2.f, 0.01f, 50.f);

	auto device = m_element.GetCore()->GetDevice();
	minimap_element& element = static_cast<minimap_element&>(m_element);
	auto& playerPosition = element.Player().Position();

	auto& camera = element.Player().Camera();
	const auto playerForward = camera.World().Forward();
	float playerAngle = math::HalfPi + atan2f(-playerForward.z(), playerForward.x());

	math::vector2 playerPos2D = math::vector2(playerPosition.x(), -playerPosition.z());

	math::rectangle textureRect = math::rectangle(0.f, 0.f, static_cast<float>(m_background->Width()), static_cast<float>(m_background->Height()));

	float zoom = element.Zoom();
	math::rectangle sourceRect = math::rectangle(0.f, 0.f, textureRect.Width() / zoom, textureRect.Height() / zoom);

	auto halfTextureWidth = (textureRect.Width() / 2);
	auto halfTextureHeight = (textureRect.Height() / 2);

	sourceRect.Left() = halfTextureWidth + (playerPos2D.x() - (sourceRect.Width() / 2));
	sourceRect.Top() = halfTextureHeight + (playerPos2D.y() - (sourceRect.Height() / 2));

	math::matrix2 rotationMatrix = math::matrix2::CreateRotation(-playerAngle);
	
	auto destinationRect = m_rectangle;
	destinationRect.Offset(m_rectangle.Width()*0.5f, m_rectangle.Height()*0.5f);

	DirectX::CommonStates states(device->GetDevice());
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());
	{
		auto sr = static_cast<RECT>(sourceRect);
		m_spriteBatch->Draw(*m_background->GetView(), destinationRect, &sr, render::color::WHITE, playerAngle, DirectX::XMFLOAT2(sourceRect.Width()*0.5f, sourceRect.Height()*0.5f));
		
		destinationRect.Inflate(4.f, 4.f);
		sourceRect.Left() = 0.f;
		sourceRect.Top() = 0.f;
		sourceRect.Width() = static_cast<float>(m_mask->Width());
		sourceRect.Height() = static_cast<float>(m_mask->Height());
		m_spriteBatch->Draw(*m_mask->GetView(), destinationRect, &sr, render::color::WHITE, playerAngle, DirectX::XMFLOAT2(sourceRect.Width()*0.5f, sourceRect.Height()*0.5f));
	}
	m_spriteBatch->End();

	for (auto& entity : element.World().Entities())
	{				
		if (entity->Type() == game::entity::EntityType::Player)
			continue;

		math::vector2 entityPos2D = math::vector2(entity->Position().x(), -entity->Position().z());	
		math::vector2 directionToEntityXZ = (entityPos2D - playerPos2D) * (zoom/4);

		directionToEntityXZ = rotationMatrix.Rotate(directionToEntityXZ, playerAngle );
		
		math::vector2 screenPosition = directionToEntityXZ;
		static bool useEllipse = true;
		if ( useEllipse )
		{
			auto frameSize = m_iconSprite->Animation()->CurrentFrame().Region().Size();
			auto halfSpriteSize = math::point(frameSize.x() / 2, frameSize.y() / 2);

			screenPosition = m_ellipse.GetPointInEllipse(directionToEntityXZ) - halfSpriteSize;
		}
		else
		{		
			screenPosition += math::vector2(m_rectangle.Width()*0.5f, m_rectangle.Height()*0.5f);
			m_rectangle.Constrain(screenPosition);
		}

		render::color c = render::color::WHITE;
		
		math::Clamp(c.A(), 0.f, 1.f);

		m_iconSprite->Play(m_animations[0]);		
		m_iconSprite->Draw(screenPosition, entity->Color());
	}
		
	m_iconSprite->Play(m_animations[2]);
	auto frameSize = m_iconSprite->FrameSize();
	auto halfSpriteSize = math::point(frameSize.x()/2, frameSize.y()/2);
	m_iconSprite->Draw(m_rectangle.Center() - halfSpriteSize);
	
}

bool minimap_controller::HandleInput(float deltaTime, const input::input_state& inputState)
{
	UNREFERENCED(deltaTime);

	auto keyboard = inputState.GetKeyboard();
	if ( keyboard != nullptr )
	{
		minimap_element& minimap = static_cast<minimap_element&>(m_element);
		if ( keyboard->KeyPressed(DIK_NUMPADPLUS) )
		{
			minimap.IncreaseZoom();
			return true;
		}
		else
		if ( keyboard->KeyPressed(DIK_NUMPADMINUS) )
		{
			minimap.DecreaseZoom();
			return true;
		}

	}
	return false;
}

} // ui