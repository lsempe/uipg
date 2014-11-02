#pragma once

#include <memory>
#include "viewport.h"

namespace render
{
	class texture;

	class device
	{
	public:

		device();
		virtual ~device();

		virtual HRESULT Create(const HWND& windowHandle);

		virtual void SetViewport(const viewport& vp) = 0;
		virtual viewport GetViewport(unsigned int numViewport = 0) const = 0;

		std::shared_ptr<texture> GetBlackTexture() const { return m_blackTexture; }
		std::shared_ptr<texture> GetWhiteTexture() const { return m_whiteTexture; }

	protected:

		std::shared_ptr<texture> m_whiteTexture;
		std::shared_ptr<texture> m_blackTexture;
	};

} // render