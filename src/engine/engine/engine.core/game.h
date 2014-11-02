#pragma once

#include <string>

namespace game
{

class core
{
public:
	
	core(HWND& windowHandle, int clientWidth, int clientHeight);
	virtual ~core();

	virtual void Initialize() {}
	virtual void Shutdown() {}

	virtual void Update() {}
	virtual void PreRender() {}
	virtual void Render() {} 
	virtual void Present() {} 
		
	const int ClientWidth() const { return m_clientWidth; }
	const int ClientHeight() const { return m_clientHeight; }

	core operator = (const core& rhs) 
	{ 
		m_windowHandle = rhs.m_windowHandle; 
		m_clientWidth = rhs.m_clientWidth;
		m_clientHeight = rhs.m_clientHeight;
		return *this; 
	}

protected:

	HWND& m_windowHandle;
	int m_clientWidth;
	int m_clientHeight;
	

};

std::wstring GetCurrentDirectory();
std::wstring GetContentPath(const wchar_t* asset);


} // game