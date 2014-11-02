#pragma once

#include <memory>
#include "engine.ui/element.h"
#include "engine.render/spritesheet.h"
#include "PrimitiveBatch.h"
#include "VertexTypes.h"

namespace game
{
	class player;
	class world;
}

namespace ui
{
	enum class CompassItemType
	{
		North,
		South,
		East,
		West,
		Entity
	};

	class compass_view : public view
	{
	public:
		compass_view(element& element) 
			: view(element) 
			, m_background(nullptr)
			, m_iconSprites(nullptr) 
			, m_spriteBatch(nullptr)
		{}
	
		virtual void Create();	
		virtual void Draw();
		virtual void Update(float deltaTime);

	private:

		math::rectangle m_virtualRect;

		std::unique_ptr<render::texture> m_background;
		std::unique_ptr<render::spritesheet> m_iconSprites;

		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;

		//std::vector<render::spritesheet::animation> m_icons;

		std::unique_ptr<render::platform::sprite_batch> m_spriteBatch;

		math::rectangle m_rectangle;
		math::rectangle m_clipping;

		std::unique_ptr<render::sprite> m_icons;

		std::vector<std::shared_ptr<render::sprite>> m_iconList;
	};

	class compass_controller : public controller
	{
	public:
	
		compass_controller(game::player&, element& element)
			: controller(element)
		{
		}

		virtual void Update(float)
		{
		}
	};

	class compass_element : public element
	{
	public:
		compass_element(game::player& player, const game::world& world, math::vector3 north = math::vector3(0,0,-1))
			: element()
			, m_player(player)
			, m_world(world)
			, m_north(north)
			, m_offsetX(0.f)
		{
			m_view = std::unique_ptr<compass_view>( new compass_view(*this) );
			m_controller = std::unique_ptr<compass_controller>( new compass_controller(player, *this) );
		}

		virtual void Create(std::shared_ptr<core_ui> core);
	
		virtual void Update(float deltaTime);
		virtual void Draw();

		compass_element& Get() { return *this; }
		const game::world& World() const { return m_world; }

	

		struct item
		{
			math::vector3 location;
			std::wstring name;
			CompassItemType itemType;
			render::color color;
		};

		std::vector<item> m_items;

		void AddItem(const wchar_t* name, CompassItemType itemType, const math::vector3& location)
		{
			item i;
			i.name = name;
			i.location = location;
			i.itemType = itemType;
			m_items.push_back(i);
		}

		const std::vector<item>& Items() const { return m_items; }

		game::player& Player() { return m_player; }

		const math::vector3& North() const { return m_north; }

	protected:

		

		game::player& m_player;
		const game::world& m_world;

		math::vector3 m_north;
		float m_offsetX;
	};
}