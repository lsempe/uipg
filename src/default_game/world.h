#pragma once

#include <list>

#include "engine.math/vector.h"

namespace game
{

class WorldProperties
{
public:

	WorldProperties(const math::vector3& north)
		: m_north(north)
	{}

	math::vector3 m_north;

};

class World
{
public:

	World(const WorldProperties& properties)
		: m_properties(properties)
	{
	}

	~World() {}

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

private:

	WorldProperties m_properties;
	
};


} // game