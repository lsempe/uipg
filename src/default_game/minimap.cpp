#include "minimap.h"

#include "engine.core/runtime_variable.h"

namespace ui
{

void view_minimap::Create()
{
	auto device = m_element.GetCore()->GetDevice();
	m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext()));
	
	m_background = std::unique_ptr<render::texture>( new render::texture(device, game::GetContentPath(L"\\content\\sprites\\minimap\\background2.dds")));

	m_iconSpriteSheet = std::unique_ptr<render::spritesheet>( new render::spritesheet(device, game::GetContentPath(L"\\content\\sprites\\minimap\\icons.dds")));
	
	auto& defaultAnim = m_iconSpriteSheet->CreateAnimation(L"default", m_animations[0]);
	defaultAnim.AddFrame(math::rectangle(8,0,8,8), 0.f);

	auto& playerAnim = m_iconSpriteSheet->CreateAnimation(L"player", m_animations[1]);
	playerAnim.AddFrame(math::rectangle(16,0,8,8), 0.f);

	auto& higherAnim = m_iconSpriteSheet->CreateAnimation(L"higher", m_animations[2]);
	higherAnim.AddFrame(math::rectangle(24,0,8,8), 0.f);

	auto& lowerAnim = m_iconSpriteSheet->CreateAnimation(L"lower", m_animations[3]);
	lowerAnim.AddFrame(math::rectangle(32,0,8,8), 0.f);

	m_iconSprite = m_iconSpriteSheet->Create();

	element_minimap& element = static_cast<element_minimap&>(m_element);
	element.World().GetSampleEntityList(entities);

	float radius = (m_rectangle.Width()/2);
	m_ellipse = math::ellipse(m_rectangle.Center(), math::vector2(radius, radius));

}

void view_minimap::Draw()
{
	static runtime_variable_float s_radarZoom(L"radar_zoom", L"controls the zoom amount on the mini map", 1.f, 0.01f, 50.f);

	auto device = m_element.GetCore()->GetDevice();
	element_minimap& element = static_cast<element_minimap&>(m_element);
	auto& playerPosition = element.Player().Position();

	float zoom = element.Zoom();

	static const float worldExtent = 2048;

	// convert to map coordinates
	auto backgroundHalfWidth = m_background->Width() * 0.5f;
	auto backgroundHalfHeight = m_background->Height() * 0.5f;
	UNREFERENCED(backgroundHalfWidth);
	UNREFERENCED(backgroundHalfHeight);


	math::vector3 testPosition = math::vector3(300, 0, 300);
	math::vector2 positionInMap = math::vector2(testPosition.x() / (worldExtent ), testPosition.z() / (worldExtent));

	math::rectangle background_rectangle = math::rectangle(0, 0, m_background->Width(), m_background->Height());
	float left = (positionInMap.x() * m_background->Width()) ;

	UNREFERENCED(left);
	//math::Clamp(left, 0.f, m_rectangle.Width() - m_background->Width());
	//background_rectangle.Left() = left;


	background_rectangle.Top() = positionInMap.y() *  m_background->Height();
	background_rectangle.Width() = m_rectangle.Width()*(1/zoom);
	background_rectangle.Height() = m_rectangle.Height()*(1/zoom);


	
	DirectX::CommonStates states(device->GetDevice());
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

		auto playerForward = element.Player().Camera().Forward();
	float playerAngle = math::Pi/2 - atan2f(playerForward.z(), playerForward.x());

	auto r = m_rectangle;
	r.Offset(128.f, 128.f);
	m_spriteBatch->Draw(*m_background->GetView(), r, &static_cast<RECT>( background_rectangle ), render::color::WHITE, playerAngle, DirectX::XMFLOAT2(128.f, 128.f) );
	
	m_spriteBatch->End();


	for ( auto& entity :  entities )
	{
		
		float relativeHeight = playerPosition.y() - entity.y();
		
		math::vector3 v = (playerPosition - entity) ;
		math::vector2 v2 = math::vector2(v.x(), v.z());

		math::vector2 screenPosition = v2;
		static bool useEllipse = true;
		if ( useEllipse )
		{
			screenPosition = m_ellipse.GetPointInEllipse(v2);
		}
		else
		{		
			m_rectangle.Constrain(screenPosition);
		}

		const float radiusSquared = (m_rectangle.Width()*m_rectangle.Width());
		render::color c = render::color::WHITE;
		
		math::Clamp(c.A(), 0.f, 1.f);

		v2.Normalize();
		math::vector2 offset = v2 * (1/zoom * 10);
		

		if ( relativeHeight > 100.f )
		{
			c = render::color::YELLOW;
			c.A() = 1.f - (v2.LengthSquared() / (radiusSquared));
			m_iconSprite->Play(m_animations[2]);
		}
		else
		if ( relativeHeight < -100.f )
		{
			c = render::color::GREEN;
			c.A() = 1.f - (v2.LengthSquared() / (radiusSquared));
			m_iconSprite->Play(m_animations[3]);
		}
		else
		{
			c = render::color::RED;
			c.A() = 1.f - (v2.LengthSquared() / (radiusSquared));
			m_iconSprite->Play(m_animations[0]);
		}

		m_iconSprite->Draw(screenPosition - offset, c);

	}
	


	m_iconSprite->Play(m_animations[2]);
	m_iconSprite->Draw(m_ellipse.Center(), render::color::WHITE, playerAngle, math::vector2(0.5f, 0.5f));
	

}

bool controller_minimap::HandleInput(float deltaTime, const input::input_state& inputState)
{
	UNREFERENCED(deltaTime);

	auto keyboard = inputState.GetKeyboard();
	if ( keyboard != nullptr )
	{
		element_minimap& minimap = static_cast<element_minimap&>(m_element);
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