#pragma once

#include <memory>
#include <engine.math/vector.h>
#include <engine.math/quaternion.h>
#include <engine.math/matrix.h>
#include <engine.render/color.h>

namespace render
{
	class cube;
	class device_direct3d;
	class camera;
	class texture;
}

namespace game
{

class entity
{
public:

	enum class EntityType
	{
		Player,
		NonPlayer,
		Virtual
	};

	entity(render::device_direct3d* device, EntityType type = EntityType::NonPlayer);
	virtual ~entity();


	EntityType Type() const { return m_type; }
	EntityType m_type;

	const math::vector3& Position() const { return m_position; }
	void SetPosition(const math::vector3& position) 
	{ 
		m_position = position; 
		m_transform = math::matrix::CreateTranslation(m_position); 
	} 

	virtual void Update(float deltaTime);

	virtual void Draw(render::camera& camera);

	void SetColor(const render::color& color);
	const render::color Color() const { return m_color; }
protected:

	math::vector3 m_position;
	math::quaternion m_rotation;

	math::matrix m_transform;
	render::color m_color;

	std::unique_ptr<render::cube> m_mesh; 
	std::shared_ptr<render::texture> m_texture;

	render::device_direct3d* m_device;
};


}