#pragma once

#include <stddef.h>
#include "engine.math/vector.h"
#include "color.h"

namespace render
{

struct vertex_default
{
    math::vector3 position;
    unsigned int diffuse;
    math::vector2 texcoord;
    math::vector3 normal;

	vertex_default()
		: position(0.f, 0.f, 0.f)
		, diffuse(color::WHITE.ToU32())
		, texcoord(0.f, 0.f)
		, normal(0.f, 0.f, 0.f)
	{}

	vertex_default (const math::vector3& p, 
					const color& c = color::WHITE, 
					const math::vector2& uvs = math::vector2::Zero, 
					math::vector3 n = math::vector3::UnitX)
      : position(p)
	  , diffuse(c.ToU32())
	  , texcoord(uvs)
	  , normal(n)
    {}

	vertex_default(	float x, float y, float z, 
					const color& c = color::WHITE,
					float u = 0.f, float v = 0.f,
					math::vector3 n = math::vector3::UnitX) 
		: position(x,y,z)
		, diffuse(c.ToU32())
		, texcoord(u, v)
		, normal(n)
    { }
	
    bool operator==(const vertex_default& b) const
    {
		return position == b.position && diffuse == b.diffuse && texcoord == b.texcoord;
    }
};

// Vertex format
static D3D11_INPUT_ELEMENT_DESC DefaultVertexDesc[] =
{
    {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(vertex_default, position),	D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"Color",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, offsetof(vertex_default, diffuse),	D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT,		0, offsetof(vertex_default, texcoord),	D3D11_INPUT_PER_VERTEX_DATA, 0},    
    {"Normal",   0, DXGI_FORMAT_R32G32B32_FLOAT,	0, offsetof(vertex_default, normal),	D3D11_INPUT_PER_VERTEX_DATA, 0},
};


} // render