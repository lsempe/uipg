#pragma once

#include <ostream>

#include "grid.h"
#include "engine.render/texture.h"
#include "engine.math/rectangle.h"

#include "engine.core/string_helper.h"

#include "SpriteFont.h"
#include "SpriteBatch.h"

#include "dragdrop.h"


namespace ui
{

template <int m, int n, typename T>
class grid_game : public grid<m,n,T>
{
public:

	
	grid_game(render::device_direct3d* device, std::shared_ptr<DirectX::SpriteFont> font)
		: grid<m,n,T>()
		, m_font(font)
	{
		m_spriteBatch = std::shared_ptr<DirectX::SpriteBatch>( new DirectX::SpriteBatch(device->GetImmediateContext()));
	}

	grid_game(grid_game&& g) 
		: m_spriteBatch(g.m_spriteBatch)
		, m_font(g.m_font)
		, m_position(g.m_position)
	{}

	grid_game& operator = (grid_game&& rhs)
	{
		if ( this != &rhs )
		{
			m_spriteBatch = rhs.m_spriteBatch;
			m_position = rhs.m_position;
			m_font = rhs.m_font;
		}
	}

	void Update(float deltaTime)
	{
	}

	void Draw()
	{
		m_spriteBatch->Begin();
		{
			for (int i = 0; i < Rows(); ++i )
			{
				for ( int j = 0; j < Columns(); ++j )
				{
					auto& slot = m_items[i][j];
					auto& item = slot.Item();
					item.Draw(i, j, slot, m_spriteBatch, m_font, m_position);

				}
			}
		}
		m_spriteBatch->End();
	}

	math::vector2& Position() { return m_position; }

private:

	std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::shared_ptr<DirectX::SpriteFont> m_font;
	math::vector2 m_position;

};

class grid_game_item : public draggable
{
public:

	grid_game_item()
		: draggable()
		, m_id(UINT32_MAX)
		, m_texture(nullptr)
	{}

	
	grid_game_item(const math::rectangle& bounds)
		: draggable(bounds)
		, m_id(UINT32_MAX)
		, m_texture(nullptr)
	{}

	grid_game_item(unsigned int id, const math::rectangle& bounds, std::shared_ptr<render::texture> texture = nullptr)
		: draggable(bounds)
		, m_id(id)
		, m_texture(texture)
	{}

	
	void Draw(int m, int n, const slot& slot, std::shared_ptr<DirectX::SpriteBatch> spriteBatch, std::shared_ptr<DirectX::SpriteFont> font, const math::vector2& position)
	{
		if ( m_texture == nullptr )
			return;

		auto width = m_texture->Width();
		auto height = m_texture->Height();


		math::rectangle rc;
		rc.Left() = position.x() + (m * width);
		rc.Top() = position.y() + ( n * height );
		rc.Width() = static_cast<float>(width);
		rc.Height() = static_cast<float>(height);
		spriteBatch->Draw(*m_texture->GetView(), rc);


		std::wostringstream ss;
		ss << slot.Count();
		std::wstring iTemp = ss.str();

		font->DrawString(spriteBatch.get(), iTemp.c_str(), rc.Position()); 

	}

	bool operator == (const grid_game_item& rhs) const 
	{ 
		return (rhs.ID() == ID()); 
	}

	bool operator != (const grid_game_item& rhs) const 
	{ 
		return (rhs.ID() != ID()); 
	}

	unsigned int ID() const { return m_id; }

private:

	std::shared_ptr<render::texture> m_texture;
	unsigned int m_id;

};

} // ui