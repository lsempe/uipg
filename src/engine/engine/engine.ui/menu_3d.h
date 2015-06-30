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
#include "engine.math/vector.h"

#include "SpriteBatch.h"

namespace ui
{

	class menu_3d
	{
	public:

		class item
		{
		public:

			item(render::device_direct3d* device, std::shared_ptr<render::texture> texture, std::shared_ptr<render::platform::sprite_batch> spriteBatch, std::shared_ptr<render::platform::font> font, std::wstring title)
				: m_device(device)
				, m_spriteBatch(spriteBatch)
				, m_quad(device)
				, m_localTransform(math::matrix::Identity)
				, m_worldTransform(math::matrix::Identity)
				, m_selected(false)
				, m_texture(texture)
				, m_visible(true)
				, m_applyTransform(true)
				, m_font(font)
				, m_title(title)
				, m_state(Released)
			{
				m_quad.ViewAlign() = true;			
				m_quad.BlendState() = render::eBlendState::Opaque;
			}

			void SetWorld(const math::matrix& world) { m_worldTransform = world; m_applyTransform = true; }
			void SetLocal(const math::matrix& local) { m_localTransform = local; m_applyTransform = true; }

			const math::matrix& Local() const { return m_localTransform; }
			const math::matrix& World() const { return m_worldTransform; }

			void SetWorldTransform(const math::matrix& world) { m_applyTransform = true; ApplyWorldTransform(world); }

			virtual void Update(const render::camera& camera, float /*deltaTime*/)
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

			bool OnButtonPressed(const render::camera& camera, input::mouse& mouse)
			{
				if (m_state != Pressed && mouse.ButtonPressed(input::mouse::eButton::Left))
				{
					// Early out, the item is not in front of camera
					auto transform = m_localTransform * m_worldTransform;
					math::vector3 directionToItem = transform.Translation() - camera.Position();
					directionToItem.Normalize();

					float dot = camera.Forward().Dot(directionToItem);
					if (dot <= 0.f)
						return false;

					// Now cast a ray from mouse's screen position at near plane to far plane
					math::vector2& mousePosition = mouse.GetPosition();
					
					const math::vector3 worldNear = m_device->GetViewport().Unproject(math::vector3(mousePosition, 0.f), camera.Projection(), camera.View(), camera.World());
					const math::vector3 worldFar = m_device->GetViewport().Unproject(math::vector3(mousePosition, 1.f), camera.Projection(), camera.View(), camera.World());

					// Do a line vs. plane intersection test
					math::vector3 intersectionPoint;
					if (m_quad.RayIntersect(worldNear, worldFar, intersectionPoint))
					{
						m_onPressed.Invoke(this, *this);
						m_state = Pressed;
						return true;
					}
				}
				else
				if (m_state == Pressed && mouse.ButtonReleased(input::mouse::eButton::Left))
				{
					m_onReleased.Invoke(this, *this);
					m_state = Released;
				}

				return false;
			}

			virtual bool HandleInput(const render::camera& camera, float /*deltaTime*/, const input::input_state& inputState)
			{
				if (!m_visible)
					return false;

				auto mouse = inputState.GetMouse();
				if (mouse)
				{
					for (auto& child : m_children)
					{
						if (child->OnButtonPressed(camera, *mouse))
							return true;
					}
				}
				return false;
			}

			virtual void Draw(const render::camera& camera)
			{
				if ( !m_visible )
					return;

				auto transform = m_localTransform * m_worldTransform;
				
				// Given the menu item's world space position, project it onto the 2D screen space.
				auto position = m_device->GetViewport().Project(transform.Translation(), camera.Projection(), camera.View(), math::matrix::Identity);

				// Calculate the direction from the camera to this item.
				math::vector3 directionToItem = transform.Translation() - camera.Position();

				// Compute the dot product to determine whether the angle between the directionToItem vector and the camera's forward vector is less than 90 degrees
				float dot = camera.Forward().Dot(directionToItem);

				// If the dot product is less than 0, the angle between the vectors is greater than 90 degrees and cannot be seen, so we don't render it.
				if (dot < 0.f)
					return;

				// Store the tentative y position at which we will draw the text
				float yOffset = position.y();
				if ( m_texture != nullptr )
				{
					m_quad.Draw(m_localTransform * m_worldTransform, camera.View(), camera.Projection(), m_texture);

					// Given that we have a texture we will offset the y just enough to place our title underneath
					yOffset = static_cast<float>(m_texture->Height() / 6);
				}

				// If we have a valid font and a title, render it. Note: we will render the text in 2D screenspace using the projected position.
				if (m_font && !m_title.empty())
				{
					m_spriteBatch->Begin();
					// Measure the size of the text to center it under the icon.
					math::vector2 textSize = static_cast<math::vector2>(m_font->MeasureString(m_title.c_str()));
					m_font->DrawString(m_spriteBatch.get(), m_title.c_str(), position + math::vector2(-static_cast<float>(textSize.x() / 2), yOffset), render::color::WHITE, 0.f, DirectX::FXMVECTOR());
					m_spriteBatch->End();
				}

				for ( auto& child : m_children )
				{
					child->Draw(camera);
				}
			}

			std::shared_ptr<item> AddChild(const math::matrix& localTransform, std::shared_ptr<render::texture> texture, const std::wstring title)
			{
				std::shared_ptr<item> it = std::make_shared<item>(m_device, texture, m_spriteBatch, m_font, title);
				it->SetLocal(localTransform);
				m_children.push_back( it );
				return it;
			}

			event_handler<item&>& OnPressed() { return m_onPressed; }
			event_handler<item&>& OnReleased() { return m_onReleased; }

			const std::vector< std::shared_ptr<item>>& Children() { return m_children; }

			bool& Selected() { return m_selected; }
			bool& Visible() { return m_visible; }

			render::quad& Quad() { return m_quad; }

			const std::wstring Title() const { return m_title; }
			void SetTitle(const std::wstring& title) { m_title = title; }

		protected:

			enum eState
			{
				Released,
				Pressed
			};

			eState m_state;

			math::matrix m_localTransform;
			math::matrix m_worldTransform;
		
			bool m_selected;
			bool m_visible;
			bool m_applyTransform;

			std::wstring m_title;

			std::vector< std::shared_ptr<item>> m_children;

			event_handler<item&> m_onPressed;
			event_handler<item&> m_onReleased;

			std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

			render::device_direct3d* m_device;
			std::shared_ptr<render::texture> m_texture;
			std::shared_ptr<render::platform::font> m_font;
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

		menu_3d(render::device_direct3d* device, std::shared_ptr<render::platform::font> font, std::shared_ptr<render::texture> texture = nullptr)
			: m_currentAngle(0.f)
			, m_targetAngle(0.f)
			, m_animationTime(0.5f)
			, m_animating(false)
			, m_currentItem(0)
			, m_font(font)
		{
			m_spriteBatch = std::make_unique<render::platform::sprite_batch>(device->GetImmediateContext());
			m_root = std::unique_ptr<item>(new item(device, nullptr, m_spriteBatch, m_font, L""));
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

		virtual void Update(const render::camera& camera, float deltaTime, const input::input_state& inputState)
		{
			
			m_interpolator.Update(deltaTime);

			if (m_animating)
			{
				m_root->SetWorld( math::matrix::CreateRotationY(m_currentAngle) );
			}

			m_root->Update(camera, deltaTime);

			HandleInput(camera, deltaTime, inputState);

		}

		virtual bool HandleInput(const render::camera& camera, float deltaTime, const input::input_state& inputState)
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

			return m_root->HandleInput(camera, deltaTime, inputState);
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
		std::shared_ptr<render::platform::font> m_font;
	};

} // ui