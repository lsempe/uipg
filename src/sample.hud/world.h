#pragma once

#include "player.h"
#include <engine.core/game.h>
#include <engine.render/quad.h>
#include <engine.render/texture.h>
#include "entity.h"

namespace render
{
	class device_direct3d;
}

namespace game
{

class world
{
public:
	world(render::device_direct3d* device);
	~world();

	virtual void Update(float deltaTime);	
	virtual void Draw();

	void AddPlayer(std::shared_ptr<game::player> player)
	{
		m_player = player;
	}

	entity m_test;

	std::shared_ptr<entity> AddEntity(std::shared_ptr<entity> entity)
	{
		m_entities.push_back(entity);
		return m_entities.back();
	}

	void GetEntityPositions(std::list<math::vector3>& positions)
	{		
		for (auto& entity : m_entities)
		{
			math::vector3 v;
			positions.push_back(entity->Position());
		}
	}

	void GetSampleEntityList(std::list<math::vector3>& positions)
	{
		int r = 100 + rand()%300;
		for (int i = 0; i < r; ++i )
		{
			math::vector3 v;
			v.x() = -200.f + ( rand()%400 );
			v.y() = -200.f + ( rand()%400 );
			v.z() = -200.f + ( rand()%400 );
			positions.push_back(v);
		}
	}
	
	const std::vector<std::shared_ptr<entity>>& Entities() const { return m_entities; }

protected:

	render::quad m_groundQuad;
	std::shared_ptr<render::texture> m_whiteTexture;

	std::vector<std::shared_ptr<entity>> m_entities;

	std::shared_ptr<game::player> m_player;

	render::device_direct3d* m_device;
};

}