#include "compass.h"
#include "engine.core/game.h"
#include "engine.core/string_helper.h"

#include "CommonStates.h"
#include "Effects.h"


#include "player.h"
#include "world.h"

namespace ui
{

void compass_element::Create(std::shared_ptr<core_ui> core)
{
	element::Create(core);
}

void compass_element::Update(float deltaTime)
{
	element::Update(deltaTime);
}

void compass_element::Draw()
{
	element::Draw();
}

void compass_view::Create()
{

	auto device = m_element.GetCore()->GetDevice();
	m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext()));
	m_background = std::unique_ptr<render::texture>( new render::texture(device, game::GetContentPath(L"\\content\\sprites\\compass\\compass_bg.dds")));

	m_iconSprites = std::unique_ptr<render::spritesheet>( new render::spritesheet(device, game::GetContentPath(L"\\content\\sprites\\compass\\compass_icons.dds")));

	size_t animId = 0;
	auto& north = m_iconSprites->CreateAnimation(L"north", animId);
	north.AddFrame(math::rectangle(32,0,32,64), 0.f);
	m_iconList.push_back( m_iconSprites->Create() );
	m_iconList.back()->Play(animId);

	//
	auto& south = m_iconSprites->CreateAnimation(L"south", animId);
	south.AddFrame(math::rectangle(0,0,32,64), 0.f);
	m_iconList.push_back( m_iconSprites->Create() );
	m_iconList.back()->Play(animId);
	
	auto& east = m_iconSprites->CreateAnimation(L"east", animId);
	east.AddFrame(math::rectangle(64,0,32,64), 0.f);
	m_iconList.push_back( m_iconSprites->Create() );
	m_iconList.back()->Play(animId);

	auto& west = m_iconSprites->CreateAnimation(L"west", animId);
	west.AddFrame(math::rectangle(96,0,32,64), 0.f);
	m_iconList.push_back( m_iconSprites->Create() );
	m_iconList.back()->Play(animId);

	auto& entityIcon = m_iconSprites->CreateAnimation(L"entity", animId);
	entityIcon.AddFrame(math::rectangle(128, 0, 32, 64), 0.f);
	m_iconList.push_back(m_iconSprites->Create());
	m_iconList.back()->Play(animId);
	
	m_icons = std::unique_ptr<render::sprite>(new render::sprite(*m_iconSprites));
	m_icons->Play(animId);


	m_primitiveBatch = std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(m_element.GetCore()->GetDevice()->GetImmediateContext()));

	auto& viewport = m_element.GetCore()->GetDevice()->GetViewport();
	m_virtualRect = math::rectangle(viewport.Width() * 0.1f, 100.f, viewport.Width() *  0.8f, 110.f);

}



	static float x = 0.f;


	static float test = 0.f;


void compass_view::Update(float deltaTime)
{
	x += deltaTime * 100.f;
	if ( x > 570 )
		x = -70;

	test += deltaTime * 0.7f;

	if ( test >= math::TwoPi )
		test = 0;

	m_icons->Update(deltaTime);

}

inline double wrapAngle( double angle )
{    
	return angle - math::Pi * floor( angle / math::Pi );
}

double constrainAngle(double x){
	x = fmod(x + math::Pi,math::TwoPi);
    if (x < math::Pi)
        x += math::TwoPi;
    return x - math::Pi;
}

bool crossSign(math::vector2& v1, math::vector2& v2)
{
	return v1.x() * v2.y() > v2.x() * v1.y();
}

float angleBetween(math::vector2& v1, math::vector2& v2)
{
	float n = v1.x() * v2.x() + v1.y() * v2.y();
	float d = sqrtf(v1.x()*v1.x() + v1.y() * v1.y()) * (v2.x() * v2.x() + v2.y() * v2.y());
	return acosf(n / d);
}

void compass_view::Draw()
{
	//const math::vector3 north = -math::vector3::UnitZ;

	auto device = m_element.GetCore()->GetDevice();

	m_rectangle = math::rectangle( device->GetViewport().Width()/2.f - (m_background->Width()/2.f), device->GetViewport().TitleSafeArea().Top(), static_cast<float>(m_background->Width()), static_cast<float>(m_background->Height()));
	m_clipping = m_rectangle;
	m_clipping.Contract(12.f, 12.f);

	DirectX::CommonStates states(device->GetDevice());
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	m_spriteBatch->Draw(*m_background->GetView(), m_rectangle);
	
	m_spriteBatch->End();

	
	float playerGlobalOrientationOffset = 0.f;
	(void)playerGlobalOrientationOffset;

	compass_element& element = static_cast<compass_element&>(m_element);
	
	const auto& north = element.North();
	(void)north;

	auto forward = element.Player().Camera().World().Forward();
	auto position = element.Player().Camera().Position();

	float dotForwardToZ = forward.Dot(math::vector3::UnitZ);
	(void)dotForwardToZ;

	float heading = acos( forward.Dot(math::vector3::UnitX) );
	float extra = (math::Pi/4) * 0.05f;
	float alpha = heading - (math::Pi/4) - extra;
	float beta = heading + (math::Pi/4) + extra;
	float range = beta - alpha;
	(void)range;

	float playerAngle = element.Player().Yaw();

	float fovX = element.Player().Camera().HorizontalFieldOfView() / 2;

	std::vector<compass_element::item> toDraw;
	for (auto& entity : element.World().Entities())
	{
		if (entity->Type() == game::entity::EntityType::Player)
			continue;

		compass_element::item i;
		i.itemType = CompassItemType::Entity;
		i.color = render::color::RED;
		i.location = entity->Position();
		i.name = L"entity";
		toDraw.push_back(i);
	}

	int i = 0;
	
	for (auto& item : element.Items())
	{
		compass_element::item i;
		i.color = render::color::WHITE;
		i.itemType = item.itemType;
		switch (item.itemType)
		{
		case CompassItemType::North:
			i.location = math::vector3(0.f, 0.f, -10000.f);
			i.name = L"north";
			break;
		case CompassItemType::West:
			i.location = math::vector3(-10000.f, 0.f, 0.f);
			i.name = L"west";
			break;
		case CompassItemType::East:
			i.location = math::vector3(10000.f, 0.f, 0.f);
			i.name = L"east";
			break;
		case CompassItemType::South:
			i.location = math::vector3(0.f, 0.f, 10000.f);
			i.name = L"south";
			break;

		}
		toDraw.push_back(i);
	}
	
	for (auto& item : toDraw)
	{

		math::vector2 entityPos2D = math::vector2(item.location.x(), item.location.z());
		math::vector2 playerPos2D = math::vector2(position.x(), position.z());

		math::vector2 playerDirection2D = math::vector2(forward.x(), forward.z());

		math::vector2 directionToEntity2D = entityPos2D - playerPos2D;
		directionToEntity2D.Normalize();
		
		float dot = playerDirection2D.Dot(directionToEntity2D);
		if (dot < 0.f) // 90 degrees fov for now
			continue;

		float a = atan2f(playerDirection2D.y(), playerDirection2D.x()) ;
		float b =  atan2f(directionToEntity2D.y(), directionToEntity2D.x());
	
		float c = math::Abs(a - b);
		if (a > b)
			c *= -1;

		

		float spriteWidth = m_icons->FrameSize().x() / 2;

		float x = c / math::Pi;
		float destinationX = m_clipping.Center().x() + x * (m_clipping.Width() * 0.5f) - spriteWidth;


		{
	
			m_icons->Draw(math::vector2(destinationX, m_rectangle.Top()));
		}

		++i;

		{
			//wchar_t b[200];
			//swprintf_s(b, 200, L"dToNorth: %.2f\n", dToNorth);
			//OutputDebugString(b);
		}

		{
			//wchar_t b[200];
			//swprintf_s(b, 200, L"Angle To %s: %.2f %.1f (%.2f,%.2f)\n", item.name.c_str(), x, d, forward.x(), forward.z());
			//OutputDebugString(b);
		}

		/*if (item.name.compare(L"north") == 0)
		{
			wchar_t b[200];
			swprintf_s(b, 200, L"Angle To NORTH: %.2f %.1f (%.2f,%.2f)\n", x, d, forward.x(), forward.z());
			OutputDebugString(b);
		}

		if (item.name.compare(L"south") == 0)
		{
			wchar_t b[200];
			swprintf_s(b, 200, L"Angle To SOUTH: %.2f %.1f (%.2f,%.2f)\n", x, d, forward.x(), forward.z());
			OutputDebugString(b);
		}
		
		if (item.name.compare(L"east") == 0)
		{
			wchar_t b[200];
			swprintf_s(b, 200, L"Angle To EAST: %.2f %.1f (%.2f,%.2f)\n", x, d, forward.x(), forward.z());
			OutputDebugString(b);
		}


		if (item.name.compare(L"west") == 0)
		{
			wchar_t b[200];
			swprintf_s(b, 200, L"Angle To WEST: %.2f %.1f (%.2f,%.2f)\n", x, d, forward.x(), forward.z());
			OutputDebugString(b);
		}*/
	}

	

	std::unique_ptr<DirectX::BasicEffect> basicEffect(new DirectX::BasicEffect(m_element.GetCore()->GetDevice()->GetDevice()));

		
		//basicEffect->SetProjection(XMMatrixOrthographicOffCenterRH(0, screenHeight, screenWidth, 0, 0, 1));
		basicEffect->SetProjection(DirectX::XMMatrixOrthographicOffCenterLH(0, 1280, 720, 0, 0, 1));
		//basicEffect->SetProjection(camera.Projection());
		basicEffect->SetVertexColorEnabled(true);

    void const* shaderByteCode;
    size_t byteCodeLength;

    basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    ID3D11InputLayout* inputLayout;

	m_element.GetCore()->GetDevice()->GetDevice()->CreateInputLayout(DirectX::VertexPositionColor::InputElements,
                              DirectX::VertexPositionColor::InputElementCount,
                              shaderByteCode, byteCodeLength,
                              &inputLayout);

	basicEffect->Apply(m_element.GetCore()->GetDevice()->GetImmediateContext());
    m_element.GetCore()->GetDevice()->GetImmediateContext()->IASetInputLayout(inputLayout);

	m_primitiveBatch->Begin();
	{
		DirectX::VertexPositionColor v0;
		DirectX::VertexPositionColor v1;
		v0.position = DirectX::XMFLOAT3(m_virtualRect.Left(), m_virtualRect.Top()-10.f, 0.f);
		v1.position = DirectX::XMFLOAT3(m_virtualRect.Right(), m_virtualRect.Top()-10.f, 0.f);
		v0.color = DirectX::XMFLOAT4(1.f,0.f,1.f,1.f);
		v1.color = DirectX::XMFLOAT4(1.f,0.f,1.f,1.f);
		m_primitiveBatch->DrawLine(v0, v1);

		float ratio = math::Pi / math::TwoPi;
		float offset = ratio * ((m_rectangle.Width()));

		v0.color = DirectX::XMFLOAT4(1.f,0.f,0.f,1.f);
		v1.color = DirectX::XMFLOAT4(1.f,0.f,0.f,1.f);

		v0.position = DirectX::XMFLOAT3(m_virtualRect.Left() + offset, m_virtualRect.Top()-20.f, 0.f);
		v1.position = DirectX::XMFLOAT3(m_virtualRect.Left() + offset, m_virtualRect.Top()+10.f, 0.f);
		m_primitiveBatch->DrawLine(v0, v1);


		auto playerPos = element.Player().Camera().World().Translation();
		v0.position = DirectX::XMFLOAT3(playerPos.x(), playerPos.y(), playerPos.z());

		auto far0 = playerPos + (forward * 10.f);
		v1.position = DirectX::XMFLOAT3(far0.x(), far0.y(), far0.z());
		m_primitiveBatch->DrawLine(v0, v1);

	}
	m_primitiveBatch->End();


		

	//std::vector<compass_element::item> toDraw;
	//for (auto& item : element.Items() )
	//{
	//	auto itemDirection = (item.location - position);
	//	itemDirection.Normalize();

	//	float d = itemDirection.Dot(forward);


	//	float d2 = math::vector3::UnitX.Dot(itemDirection);
	//	if ( d2 == 0.f )
	//		d2 = math::vector3::UnitX.Dot(-itemDirection);
	//	
	//	float absAngle = acos(d2);
	//	if ( d < 0 )
	//		absAngle *= -1.f;
	//	
	//		
	//	float angleToItem = acos(d);
	//	if ( d < 0 )
	//		angleToItem *= -1.f;
		
	//	if ( angleToItem > math::Pi )
	//		angleToItem = 0.f;

	//	
	//	/*float d = forward.Dot(itemDirection);
	//	if ( d >= 0 )
		//	continue;

	//	float cs = cosf(math::Pi/2);
	//	float ss = sinf(math::Pi/2);
	//	float csn = cosf(-math::Pi/2);
	//	float ssn = sinf(-math::Pi/2);

	//	math::vector2 right = math::vector2(forward.x() * cs  - forward.z() * ss, forward.x() * ss - forward.z() * cs);
	//	math::vector2 left = math::vector2(forward.x() * csn - forward.z() * ssn, forward.x() * ssn - forward.z() * csn);
	//	math::vector2 span = right - left;

	//	float spanLength = span.Length();
	//	float radius = 10.f;
	//	auto directionToItemFromLeft = item.location - (position + (left*radius));

	//	auto test = left * ( left.Dot(directionToItemFromLeft) );
	//	auto spanLengthToItem = test.Length();
	//	auto ratio = spanLengthToItem / spanLength;*/
	//	
	//	float moveAngle = 0.f;
	//	moveAngle = (forward.Dot(math::vector2::UnitX));
	//	//if ( moveAngle < -math::Pi )
	//		//moveAngle = math::Pi;

	//	//if ( d < 0 )
	//	//	moveAngle *= -1.f;

	//	absAngle += moveAngle;

	//	float moveRatio = moveAngle / math::Pi ;

	//	float width = m_rectangle.Width();

	//	math::vector2 offset2 = math::vector2::Zero;
	//	//math::vector2 offset2 = math::vector2(moveRatio * width, 0.f);

	//	float ratio = ( (absAngle ) / math::Pi );
	//			
	//	float halfWidth = (m_rectangle.Width()/2);
	//	math::vector2 offset = math::vector2(halfWidth + (ratio * width), 0.f);
	//	m_iconSprites->Draw(item.name.c_str(), 0, m_rectangle.Position() + offset + offset2 ,render::color::WHITE, 0.f, math::vector2::Zero/*, m_clipping*/);

	//	wchar_t b[200];
	//	swprintf_s(b, 200, L"%s: d: %.4f angle: %.3f ratio: %.3f fwd:(%.2f,%.2f) dir:(%.2f,%.2f) absAngle: %0.2f moveAngle: %0.2f\n", item.name.c_str(), d, math::RadiansToDegrees(angleToItem), ratio, forward.x(), forward.z(), itemDirection.x(),itemDirection.z(), absAngle, moveAngle);
	//	OutputDebugString(b);
	//}



	// STILL NOT PERFECT.

	//for (auto& item : element.Items() )
	//{
	//	auto itemDirection = (position - item.location);
	//	itemDirection.Normalize();

	//	if ( itemDirection.Dot(forward) <= 0 )
	//		continue;
	//	
	//	float dotDirectionToX = itemDirection.Dot(math::vector3::UnitX);
	//	float dotDirectionToZ = itemDirection.Dot(math::vector3::UnitZ);
	//	float itemHeading = acos( (dotDirectionToX > 0) ? dotDirectionToZ : dotDirectionToX);
	//	//itemHeading = wrapAngle(itemHeading);
	
	//	auto testAngle = acos(dotForwardToZ);
	//	

	//	//itemHeading = constrainAngle(itemHeading);
	//	
	//	//float dotDirectionToForward = itemDirection.Dot( (dotForwardToZ < 0) ? -forward : forward);		
	//	float dotDirectionToForward = itemDirection.Dot( forward );		

	//	auto testAngle2 = acos(dotDirectionToForward);

	//	//if ( dotDirectionToForward < 0 && dotForwardToZ > 0 || dotDirectionToForward > 0 && dotForwardToZ < 0  )
	//		//continue;
	//	
	//	float angle = itemHeading - alpha;
	//	float ratio = angle / range;
	//	
	//	const auto pad = (m_rectangle.Width() * 0.05);
	//	const auto width = (m_rectangle.Width())/* - (2 * pad)*/;
	//	math::vector2 offset = math::vector2(/*pad +*/ (width * ratio), 0.f);

	//	if ( dotForwardToZ > 0 ) 
	//	{
	//		offset = math::vector2(/*pad + */(width * (0.95-ratio)), 0.f);
	//	}

	//	m_iconSprites->Draw(item.name.c_str(), 0, m_rectangle.Position() + offset ,render::color::WHITE, 0.f, math::vector2::Zero, m_clipping);

	//	wchar_t b[200];
	//	swprintf_s(b, 200, L"%s: %.3f dftz: %.3f | %.3f %.3f offset: %.3f angle: %.3f ddtx: %.3f ddtz: %.3f itemHeading: %.3f ddtf: %.3f\n", item.name.c_str(), testAngle, dotForwardToZ, testAngle2, dotDirectionToForward, offset.x(), angle, dotDirectionToX, dotDirectionToZ, itemHeading, dotDirectionToForward);
	//	//swprintf_s(b, 200, L"%s: %.2f | %.4f offset.x: %.2f | itemHeading: %.2f dotForwardToZ: %.2f dotDirectionToForward: %.2f dotDirectionToX: %.2f\n", item.name.c_str(), angle, ratio, offset.x(), itemHeading, dotForwardToZ, dotDirectionToForward, dotDirectionToX);
	//	OutputDebugString(b);
	//}
}

} // ui