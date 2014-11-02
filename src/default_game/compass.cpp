#include "compass.h"
#include "engine.core/game.h"

namespace ui
{

void element_compass::Create(std::shared_ptr<core_ui> core)
{
	element::Create(core);
}

void element_compass::Update(float deltaTime)
{
	element::Update(deltaTime);
}

void element_compass::Draw()
{
	element::Draw();
}

void View_Compass::Create()
{

	auto device = m_element.GetCore()->GetDevice();
	m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>( new render::platform::sprite_batch(device->GetImmediateContext()));
	m_background = std::unique_ptr<render::texture>( new render::texture(device, game::GetContentPath(L"\\content\\sprites\\compass\\compass_bg.dds")));

	m_iconSprites = std::unique_ptr<render::spritesheet>( new render::spritesheet(device, game::GetContentPath(L"\\content\\sprites\\compass\\compass_icons.dds")));

	//auto& north = m_iconSprites->CreateAnimation(L"north");
	//north.AddFrame(math::rectangle(32,0,32,64), 0.f);
	//
	//auto& south = m_iconSprites->CreateAnimation(L"south");
	//south.AddFrame(math::rectangle(0,0,32,64), 0.f);
	
	//auto& east = m_iconSprites->CreateAnimation(L"east");
	//east.AddFrame(math::rectangle(64,0,32,64), 0.f);

	//auto& west = m_iconSprites->CreateAnimation(L"west");
	//west.AddFrame(math::rectangle(96,0,32,64), 0.f);
}

	static float x = 0.f;


	static float test = 0.f;


void View_Compass::Update(float deltaTime)
{
	x += deltaTime * 100.f;
	if ( x > 570 )
		x = -70;

	test += deltaTime * 0.7f;

	if ( test >= math::TwoPi )
		test = 0;

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

void View_Compass::Draw()
{
	
	auto device = m_element.GetCore()->GetDevice();

	m_rectangle = math::rectangle( device->GetViewport().Width()/2.f - (m_background->Width()/2.f), device->GetViewport().TitleSafeArea().Top(), m_background->Width(), m_background->Height());
	m_clipping = m_rectangle;
	m_clipping.Contract(12.f, 12.f);

	DirectX::CommonStates states(device->GetDevice());
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());

	m_spriteBatch->Draw(*m_background->GetView(), m_rectangle);
	
	m_spriteBatch->End();

	
	float playerGlobalOrientationOffset = 0.f;

	element_compass& element = static_cast<element_compass&>(m_element);
	
	const auto& north = element.North();
	auto forward = element.Player().Camera().View().Forward();
	auto position = element.Player().Camera().Position();

	float dotForwardToZ = forward.Dot(math::vector3::UnitZ);
	float heading = acos( forward.Dot(math::vector3::UnitX) );
	float extra = (math::Pi/4) * 0.05f;
	float alpha = heading - (math::Pi/4) - extra;
	float beta = heading + (math::Pi/4) + extra;
	float range = beta - alpha;




	std::vector<element_compass::item> toDraw;
	for (auto& item : element.Items() )
	{
		auto itemDirection = (item.location - position);
		//itemDirection.Normalize();

		float d = itemDirection.Dot(forward);
		if ( d <= 0 )
			continue;

		auto left = math::vector3(-sin(alpha), 0.f, -cos(alpha));
		auto right = math::vector3(-sin(beta), 0.f, -cos(beta));		

		auto test = left * left.Dot(itemDirection);
		//math::vector3 far = (forward * 100.f);
		auto span = right - left;
		auto spanLength = span.Length();

		auto distance = (test - left).Length();
		
		//iconSprites->Draw(item.name.c_str(), 0, m_rectangle.Position() + offset + offset2 ,render::color::WHITE, 0.f, math::vector2::Zero/*, m_clipping*/);

		wchar_t b[200];
		swprintf_s(b, 200, L"%s: \n", item.name.c_str());
		//OutputDebugString(b);
	}


		

	//std::vector<element_compass::item> toDraw;
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