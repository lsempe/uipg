#include "entity.h"
#include "engine.render/camera.h"
#include "engine.render/cube.h"
#include "engine.render/texture.h"
#include "engine.render/texture_white.h"


namespace game
{

entity::entity(render::device_direct3d* device, EntityType type )
	: m_type(type)
	, m_position(math::vector3::Zero)
	, m_color(render::color::RED)
	, m_device(device)
{
	m_mesh = std::unique_ptr<render::cube>(new render::cube(device));
	m_transform = math::matrix::CreateTranslation(math::vector3::Zero);

	m_texture = std::shared_ptr<render::texture>(new render::texture(device, render::textures::white::data, render::textures::white::size));
}

void entity::SetColor(const render::color& color) 
{ 
	m_color = color; 
}


entity::~entity()
{
}

void entity::Update(float )
{
	m_transform = math::matrix::CreateTranslation(m_position);
}

void entity::Draw(render::camera& camera)
{
	m_mesh->Draw(m_transform, camera.View(), camera.Projection(), m_texture, m_color);

	//math::matrix t = math::matrix::CreateTranslation(math::vector3(10.f, 10.f, 0.f));
	//m_mesh->Draw(t, camera.View(), camera.Projection(), m_texture, render::color::BLUE);
}

}