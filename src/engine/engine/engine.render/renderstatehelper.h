#pragma once

#include <memory>

#include "device_direct3d.h"
#include "CommonStates.h"

namespace render
{

// renderstatehelper
enum class eBlendState
{
	Opaque,
	AlphaBlend,
	Additive,
	NonPremultiplied
};

enum class eDepthStencilState
{
	None,
	Default,
	Read
};

enum class eRasterizerState
{
	CullNone,
	CullCW,
	CullCCW
};

enum class eSamplerState
{
	PointWrap,
	PointClamp,
	LinearWrap,
	LinearClamp,
	AnisotroppicWrap,
	AnisotropicClamp
};

class renderstatehelper
{
public:

	renderstatehelper(device_direct3d* device)
	{
		m_states = std::unique_ptr<DirectX::CommonStates>( new DirectX::CommonStates(device->GetDevice()) );
	}

	ID3D11BlendState* BlendState(eBlendState blendState)
	{
		switch ( blendState )
		{		
		case eBlendState::AlphaBlend:
			return m_states->AlphaBlend();
		case eBlendState::Additive:
			return m_states->Additive();
		case eBlendState::NonPremultiplied:
			return m_states->NonPremultiplied();
		case eBlendState::Opaque:
		default:
			return m_states->Opaque();
		}
	}

	ID3D11DepthStencilState* DepthStencilState(eDepthStencilState depthStencilState)
	{
		switch ( depthStencilState )
		{
		case eDepthStencilState::None:
			return m_states->DepthNone();
		case eDepthStencilState::Read:
			return m_states->DepthRead();
		case eDepthStencilState::Default:
		default:
			return m_states->DepthDefault();

		}
	}

	ID3D11RasterizerState* RasterizerState(eRasterizerState rasterizerState)
	{
		switch ( rasterizerState )
		{
		
		case eRasterizerState::CullCW:
			return m_states->CullClockwise();
		case eRasterizerState::CullCCW:
			return m_states->CullCounterClockwise();		
		case eRasterizerState::CullNone:
		default:
			return m_states->CullNone();
		}		
	}

	ID3D11SamplerState* SamplerState(eSamplerState samplerState)
	{
		switch ( samplerState )
		{
		case eSamplerState::PointWrap:
			return m_states->PointClamp();
		case eSamplerState::PointClamp:
			return m_states->PointWrap();		
		case eSamplerState::LinearClamp:
			return m_states->LinearClamp();
		case eSamplerState::AnisotropicClamp:
			return m_states->AnisotropicClamp();
		case eSamplerState::AnisotroppicWrap:
			return m_states->AnisotropicWrap();
		case eSamplerState::LinearWrap:
		default:
			return m_states->LinearWrap();
		}
	}

	~renderstatehelper()
	{
		m_states = nullptr;
	}

private:

	std::unique_ptr<DirectX::CommonStates> m_states;
};


} // render