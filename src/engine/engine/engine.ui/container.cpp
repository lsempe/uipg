#include "container.h"
#include "control.h"
#include "CommonStates.h"

namespace ui
{

	void container::Update(float deltaTime)
	{
		for ( auto& control : m_controls )
		{
			control->Update(deltaTime);
		}
	}

	bool container::HandleInput(float deltaTime, const input::input_state& inputState)
	{
		for ( auto& control : m_controls )
		{
			if ( control->HandleInput(deltaTime, inputState) )
				return true;
		}

		return false;
	}

	void container::Draw(const render::camera& camera)
	{
		DirectX::CommonStates states(m_core->GetDevice()->GetDevice());
		m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied());
		{
			for ( auto& control : m_controls )
			{
				control->Draw(camera);
			}
		}
		m_spriteBatch->End();
	}

} // ui