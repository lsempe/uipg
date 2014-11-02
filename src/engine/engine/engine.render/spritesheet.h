#pragma once

#include <memory>
#include <string>
#include <map>
#include <list>
#include "engine.core/sys.h"
#include "device_direct3d.h"
#include "texture.h"
#include "color.h"
#include "engine.math/vector.h"

#include "engine.ui/core.h"
#include "engine.core/event_handler.h"
#include "CommonStates.h"

namespace render
{

class sprite;

/// spritesheet
/// A 2D sprite sheet with animation support.
/// 
class spritesheet
{
public:

	/// animation
	/// A collection of frames in a sprite sheet.
	class animation
	{
	public:

		/// frame
		/// A frame represents a region of the sprite sheet and the amount of time the frame should be active.
		/// Frames support custom triggers which will be executed on the frame's entry.
		class frame
		{
		public:

			frame(const math::rectangle& region, float duration)
				: m_region(region)
				, m_duration(duration)				
			{}

			float Duration() const { return m_duration; }
			const math::rectangle& Region() const { return m_region; }

		protected:

			math::rectangle m_region;
			float m_duration;
		};

		animation()
			: m_time(0.f)
			, m_loop(true)
		{}
	
		explicit animation(const std::wstring& name)
			: m_name(name)
			, m_time(0.f)
			, m_loop(true)
		{
		}

		frame& AddFrame(const math::rectangle& region, float duration)
		{
			frame f(region, duration);
			m_frames.push_back(f);
			
			return m_frames.back();
		}	

		bool& Loop() { return m_loop; }
		const bool Loop() const { return m_loop; }
		const frame& Frame(size_t index) const { return m_frames[index]; }
		const std::wstring& Name() const { return m_name; }
		size_t FrameCount() const { return m_frames.size(); }

	private:

		std::wstring m_name;
		bool m_loop;
		float m_time;

		std::vector<frame> m_frames;
	};

	spritesheet(render::device_direct3d* device, std::wstring path)
		: m_device(device)
		, m_currentFrame(0)
	{
		m_texture = std::shared_ptr<render::texture>(new render::texture(device, path));
		m_width = m_texture->Width();
		m_height = m_texture->Height();		

		m_spriteBatch = std::unique_ptr<render::platform::sprite_batch>(new render::platform::sprite_batch(device->GetImmediateContext()));
	}

	~spritesheet()
	{
		m_texture = nullptr;
		m_spriteBatch = nullptr;
	}
	
	bool GetAnimationID(const std::wstring& name, size_t& id)
	{
		id = SIZE_MAX;

		if ( m_animationIDMap.find(name) == m_animationIDMap.end() )
			return false;

		id = m_animationIDMap[name];

		return true;
	}

	const animation& GetAnimation(size_t animationID)
	{
		if ( animationID >= m_animations.size() )
		{
			throw std::out_of_range("Animation does not exist.");
		}

		return m_animations[animationID];
	}

	animation& CreateAnimation(std::wstring name, size_t& animationID)
	{
		if ( GetAnimationID(name, animationID) )
		{
			return m_animations[animationID];
		}

		animation anim(name);
		m_animations.push_back(anim);
		animationID = m_animations.size()-1;
		m_animationIDMap[name] = animationID;

		return m_animations[animationID];
	}

	const unsigned int Width() const { return m_width; }
	const unsigned int Height() const { return m_height; }

	class animation_track
	{
	public:

		enum eStatus { 
			Playing,
			Finished
		};

		explicit animation_track(const animation& anim)
			: m_animation(anim)
			, m_currentFrame(0)
			, m_time(0.f)
			, m_status(Playing)
		{}

		void Start(size_t frame=0)
		{
			if ( frame >= m_animation.FrameCount() )
				return;

			m_status = Playing;
			m_currentFrame = frame;
			auto& currentFrame = m_animation.Frame(m_currentFrame);
			m_time = currentFrame.Duration();
		}

		void Update(float deltaTime)
		{
			if ( m_animation.FrameCount() == 0 || m_status == Finished )
			{
				return;
			}

			m_time -= deltaTime;

			if ( m_time <= 0.f )
			{
				++m_currentFrame;
				if ( m_currentFrame >= m_animation.FrameCount() )
				{
					if ( m_animation.Loop() )
					{
						m_currentFrame = 0;
					}
					else
					{
						m_status = Finished;
						m_currentFrame = m_animation.FrameCount() - 1;
						m_onAnimationFinished.Invoke(this, *this);
						return;
					}

					m_onExitFrame.Invoke(this, *this);
				}

				auto& currentFrame = m_animation.Frame(m_currentFrame);
				m_onEnterFrame.Invoke(this, *this);
				m_time = currentFrame.Duration();
			}			
		}
		
		bool IsFirstFrame() const { return m_currentFrame == 0; }
		bool IsLastFrame() const { return m_currentFrame == m_animation.FrameCount() - 1; }

		const size_t FrameCount() const { return m_animation.FrameCount(); }
		const size_t FrameNumber() const { return m_currentFrame; }
		const animation::frame& CurrentFrame() const { return m_animation.Frame(m_currentFrame); }
		
		event_handler<const animation_track&>& OnAnimationFinished() { return m_onAnimationFinished; }
		event_handler<const animation_track&>& OnEnterFrame() { return m_onEnterFrame; }
		event_handler<const animation_track&>& OnExitFrame() { return m_onExitFrame; }

	protected:

		eStatus m_status;

		float m_time;
		size_t m_currentFrame;
		const animation& m_animation;

		event_handler<const animation_track&> m_onAnimationFinished;
		event_handler<const animation_track&> m_onEnterFrame;
		event_handler<const animation_track&> m_onExitFrame;

		animation_track& operator = (const animation_track&) { return *this; }
	};
	
	

	std::shared_ptr<sprite> Create();

	render::device_direct3d* GetDevice() { return m_device; }
	std::shared_ptr<render::texture> Texture() { return m_texture; }

	std::shared_ptr<render::platform::sprite_batch> SpriteBatch() { return m_spriteBatch; }

protected:

	unsigned int m_currentFrame;

	unsigned int m_width;
	unsigned int m_height;

	std::map<std::wstring, size_t> m_animationIDMap;
	std::vector<animation> m_animations;

	std::shared_ptr<render::platform::sprite_batch> m_spriteBatch;

	render::device_direct3d* m_device;
	std::shared_ptr<render::texture> m_texture;

	std::vector<std::shared_ptr<sprite>> m_sprites;

};

	class sprite
	{
	public:
		sprite(spritesheet& spriteSheet);

		void Update(float deltaTime);

		void Draw(const math::vector2& position, render::color color = render::color::WHITE, float rotation = 0.f, const math::vector2& origin = math::vector2::Zero, const math::rectangle clipping = math::rectangle::Zero);

		void Play(size_t animationID, unsigned int startFrame=0);
		
		bool IsPlaying(size_t animationID);

		event_handler<const spritesheet::animation_track&>& OnAnimationFinished() { return m_onAnimationFinished; }
		event_handler<const spritesheet::animation_track&>& OnEnterFrame() { return m_onEnterFrame; }
		event_handler<const spritesheet::animation_track&>& OnExitFrame() { return m_onEnterFrame; }

		size_t CurrentAnimationID() const  { return m_currentAnimationID; }

		math::vector2 FrameSize() const { return m_currentAnimation->CurrentFrame().Region().Size(); }

		std::shared_ptr<spritesheet::animation_track> Animation() { return m_currentAnimation; }

	protected:

		void Clip(const math::vector2& position, math::rectangle& source, math::rectangle& destination, const math::vector2& sourceSize, const math::rectangle& clipping);
		
		size_t m_currentAnimationID;
		std::shared_ptr<spritesheet::animation_track> m_currentAnimation;
		spritesheet& m_spriteSheet;
		
		event_handler<const spritesheet::animation_track&> m_onEnterFrame;
		event_handler<const spritesheet::animation_track&> m_onExitFrame;
		event_handler<const spritesheet::animation_track&> m_onAnimationFinished;

		sprite& operator = (const sprite&) { return *this; }
	};

} // render