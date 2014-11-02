#include "spritesheet.h"

namespace render
{

	std::shared_ptr<sprite> spritesheet::Create() 
	{ 
		std::shared_ptr<sprite> newSprite = std::shared_ptr<sprite>(new sprite(*this));		
		m_sprites.push_back(newSprite);
		return newSprite;
	}

	sprite::sprite(spritesheet& spriteSheet)
		: m_spriteSheet(spriteSheet)
		, m_currentAnimationID(SIZE_MAX)
	{}

	void sprite::Update(float deltaTime)
	{
		if ( m_currentAnimation != nullptr )
		{
			m_currentAnimation->Update(deltaTime);
		}
	}

	void sprite::Draw(const math::vector2& position, render::color color, float rotation, const math::vector2& origin, const math::rectangle clipping)
	{
		if ( m_currentAnimation == nullptr )
			return;

		auto& frame = m_currentAnimation->CurrentFrame();
									
		auto source = frame.Region();
		auto destination = math::rectangle(position.x(), position.y(), source.Width(), source.Height());

		Clip(position, source, destination, frame.Region().Size(), clipping);
				
		auto device = m_spriteSheet.GetDevice();
		DirectX::CommonStates states(device->GetDevice());

		auto spriteBatch = m_spriteSheet.SpriteBatch();			
		spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, states.NonPremultiplied());
		{
			auto view = m_spriteSheet.Texture()->GetView();
			RECT rc = static_cast<RECT>( source );
			spriteBatch->Draw(*view, destination, &rc, color, rotation, DirectX::XMFLOAT2(origin.x(), origin.y()));
		}
		spriteBatch->End();
	}

	void sprite::Play(size_t animationID, unsigned int startFrame/*=0*/)
	{			
		if ( m_currentAnimationID == animationID )
			return;

		m_currentAnimationID = animationID;
		m_currentAnimation = std::shared_ptr<spritesheet::animation_track>(new spritesheet::animation_track(m_spriteSheet.GetAnimation(animationID)));
			
		m_currentAnimation->OnEnterFrame() += [&](void*, const spritesheet::animation_track& track) { OnEnterFrame().Invoke(this, track); };
		m_currentAnimation->OnExitFrame() += [&](void*, const spritesheet::animation_track& track) { OnExitFrame().Invoke(this, track); };
		m_currentAnimation->OnAnimationFinished() += [&](void*, const spritesheet::animation_track& track) { OnAnimationFinished().Invoke(this, track); };

		m_currentAnimation->Start(startFrame);		
	}

	bool sprite::IsPlaying(size_t animationID)
	{
		return ( m_currentAnimation != nullptr && m_currentAnimationID == animationID );
	}

	void sprite::Clip(const math::vector2& position, math::rectangle& source, math::rectangle& destination, const math::vector2& sourceSize, const math::rectangle& clipping)
	{
		if ( !clipping.IsEmpty() )
		{
			if ( !clipping.Intersects(destination) )
				return;

			destination.ClipTo(clipping);
		}			
		
		// adjust source
		if ( position.x() <= clipping.Left() )
		{
			auto delta = clipping.Left() - position.x();
			source.Left() += delta;
			source.Width() -= delta;
		}

		if ( position.x() + sourceSize.x() >= clipping.Right() )
		{
			source.Width() = destination.Width();
		}

		if ( position.y() <= clipping.Top() )
		{
			auto delta = clipping.Top() - position.y();
			source.Top() += delta;
			source.Height() -= delta;
		}

		if ( position.y() + sourceSize.y() >= clipping.Bottom() )
		{
			source.Height() = destination.Height();
		}
	}

}