#pragma once

#include <list>

#include "engine.render/platform.h"
#include "engine.render/camera.h"
#include "engine.math/matrix.h"
#include "engine.core/event_handler.h"
#include "engine.core/input.h"
#include "engine.render/texture.h"
#include "engine.render/device_direct3d.h"
#include "engine.render/quad.h"
#include "engine.render/device_direct3d.h"
#include "engine.ui/interpolator.h"

#include "SpriteBatch.h"

namespace ui
{

	class menu_3d
	{
	public:

		class item
		{
		public:

			item(render::device_direct3d* device, std::shared_ptr<render::texture> texture, std::shared_ptr<render::platform::sprite_batch> spriteBatch)
				: m_device(device)
				, m_spriteBatch(spriteBatch)
				, m_quad(device)
				, m_localTransform(math::matrix::Identity)
				, m_worldTransform(math::matrix::Identity)
				, m_selected(false)
				, m_texture(texture)
				, m_visible(true)
				, m_applyTransform(true)
			{
				m_quad.ViewAlign() = true;			
				m_quad.BlendState() = render::eBlendState::Opaque;
			}

			void SetWorld(const math::matrix& world) { m_worldTransform = world; m_applyTransform = true; }
			void SetLocal(const math::matrix& local) { m_localTransform = local; m_applyTransform = true; }

			const math::matrix& Local() const { return m_localTransform; }
			const math::matrix& World() const { return m_worldTransform; }

			void SetWorldTransform(const math::matrix& world) { m_applyTransform = true; ApplyWorldTransform(world); }

			virtual void Update(float /*deltaTime*/)
			{
				if ( m_applyTransform )
				{
					for ( auto& child : m_children )
					{
						child->ApplyWorldTransform(m_worldTransform);
					}
					m_applyTransform = false;
				}
			}

			virtual bool HandleInput(float /*deltaTime*/, const input::input_state& /*inputState*/)
			{
				return false;
			}

			virtual void Draw(const render::camera& camera)
			{
				if ( !m_visible )
					return;

				if ( m_texture != nullptr )
				{
					m_quad.Draw(m_localTransform * m_worldTransform, camera.View(), camera.Projection(), m_texture);
				}

				for ( auto& child : m_children )
				{
					child->Draw(camera);
				}
			}

			std::shared_ptr<item> AddChild(const math::matrix& localTransform, std::shared_ptr<render::texture> texture)
			{
				std::shared_ptr<item> it = std::shared_ptr<item>(new item(m_device, texture, m_spriteBatch));
				it->SetLocal(localTransform);
				m_children.push_back( it );
				return it;
			}

			event_handler<item&> OnPressed() { return m_onPressed; }

			const std::vector< std::shared_ptr<item>>& Children() { return m_children; }

			bool& Selected() { return m_selected; }
			bool& Visible() { return m_visible; }

			render::quad& Quad() { return m_quad; }

		protected:

			math::matrix m_localTransform;
			math::matrix m_worldTransform;
		
			bool m_selected;
			bool m_visible;
			bool m_applyTransform;

			std::vector< std::shared_ptr<item>> m_children;

			event_handler<item&> m_onPressed;

			std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

			render::device_direct3d* m_device;
			std::shared_ptr<render::texture> m_texture;
			render::quad m_quad;

			void ApplyWorldTransform(const math::matrix& world)
			{
				m_worldTransform = world;
				for ( auto& child : m_children )
				{
					child->SetWorld( m_worldTransform * child->Local() );
				}
			}

		};

		menu_3d(render::device_direct3d* device, std::shared_ptr<render::texture> texture = nullptr)
			: m_currentAngle(0.f)
			, m_targetAngle(0.f)
			, m_animationTime(0.5f)
			, m_animating(false)
			, m_currentItem(0)
		{
			m_root = std::unique_ptr<item>(new item(device, nullptr, m_spriteBatch));
			m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(device->GetImmediateContext()));
		}

		void CycleLeft()
		{
			if ( m_animating ) 
				return;

			for (auto& item : m_root->Children() )
			{
				item->Selected() = false;
			}

			if ( m_currentItem - 1 < 0 )
			{
				m_currentItem = static_cast<int>(m_root->Children().size() - 1);
				m_currentAngle = math::TwoPi;
			}
			else
			{
				--m_currentItem;
			}

			m_root->Children()[m_currentItem]->Selected() = true;

			const float step = math::TwoPi / m_root->Children().size();
			m_targetAngle = m_currentItem * step;

			ui::animation anim(m_currentAngle, m_targetAngle, m_animationTime, ui::curves::ease_in_out<ui::curves::quadratic>, [&]{ m_animating=false; } );
			m_interpolator.Add(anim);

			m_animating = true;
		}

		void CycleRight()
		{
			if ( m_animating ) 
				return;
		
			for (auto& item : m_root->Children() )
			{
				item->Selected() = false;
			}

			const int childrenCount = static_cast<int>(m_root->Children().size());
			const float step = math::TwoPi / childrenCount;
			if ( m_currentItem + 1 >= childrenCount )
			{
				m_currentItem = 0;
				m_currentAngle = -step;
			}
			else
			{
				m_currentItem = (m_currentItem + 1) %  childrenCount;
			}

			m_root->Children()[m_currentItem]->Selected() = true;

			m_targetAngle = m_currentItem * step;

			ui::animation anim(m_currentAngle, m_targetAngle, m_animationTime, ui::curves::ease_in_out<ui::curves::quadratic>, [&]{ m_animating=false; } );
			m_interpolator.Add(anim);
		

			m_animating = true;
		}

		virtual void Update(float deltaTime)
		{
			m_interpolator.Update(deltaTime);

			if (m_animating)
			{
				m_root->SetWorld( math::matrix::CreateRotationY(m_currentAngle) );
			}

			m_root->Update(deltaTime);

		}

		virtual bool HandleInput(float deltaTime, const input::input_state& inputState)
		{
			auto keyboard = inputState.GetKeyboard();
			if ( keyboard != nullptr )
			{
				if ( keyboard->KeyPressed(DIK_RIGHT) )
				{
					CycleRight();
					return true;
				}
				if ( keyboard->KeyPressed(DIK_LEFT) )
				{
					CycleLeft();
					return true;
				}
			}

			return m_root->HandleInput(deltaTime, inputState);
		}

		virtual void Draw(const render::camera& camera)
		{
			m_root->Draw(camera);
		}

		std::unique_ptr<item>& Root() { return m_root; } 
		std::shared_ptr<render::platform::sprite_batch>& SpriteBatch() { return m_spriteBatch; }

	protected:

		float m_currentAngle;
		float m_targetAngle;
		float m_animationTime;
		bool m_animating;
		int m_currentItem;
	
		std::unique_ptr<item> m_root;
	
		ui::interpolator m_interpolator;

		std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

	};

} // ui