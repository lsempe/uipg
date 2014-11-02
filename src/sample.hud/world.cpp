#include "world.h"

#include <engine.render/axes.h>
#include <engine.render/device_direct3d.h>
#include <engine.render/texture_white.h>


namespace game
{


world::world(render::device_direct3d* device)
	: m_groundQuad(device, 10.f, 10.f)
	, m_test(device)
	, m_device(device)
{
	m_groundQuad.ViewAlign() = false;
	m_groundQuad.Transform() = math::matrix::CreateUniformScale(20.f) * math::matrix::CreateRotationX(math::Pi/2);
	m_groundQuad.BlendState() = render::eBlendState::NonPremultiplied;

	m_whiteTexture = std::shared_ptr<render::texture>( new render::texture(device, game::GetContentPath(L"\\content\\textures\\misc\\grass.dds") ) );
}	


world::~world(void)
{
}

void world::Update(float deltaTime)
{
	for ( auto entity : m_entities )
	{
		entity->Update(deltaTime);
	}
	
}

void world::Draw()
{
	static float y =0.f;
	y += 0.1f;

	//m_groundQuad.Pivot() = math::vector3(0.f, 0.0f, 0.f);
	//m_groundQuad.Transform() = math::matrix::CreateUniformScale(20.f);// *math::matrix::CreateRotationX(math::Pi);
	
	auto& camera = m_player->Camera();
	//math::matrix transform = math::matrix::CreateTranslation(camera.Position() - ( camera.Forward() * 10.f));
	m_groundQuad.Draw(math::matrix::Identity, camera.View(), camera.Projection(), m_whiteTexture);

	
	render::axes a(m_device);
	a.Draw(math::matrix::Identity, camera.View(), camera.Projection());

	for ( auto entity : m_entities )
	{		
		entity->Draw(camera);
	}
	

	//m_test.Draw(camera);
}

}