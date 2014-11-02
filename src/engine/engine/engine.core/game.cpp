#include "stdafx.h"
#include "game.h"

namespace game
{

core::core(HWND& windowHandle, int clientWidth, int clientHeight)
	: m_windowHandle(windowHandle)
	, m_clientWidth(clientWidth)
	, m_clientHeight(clientHeight)
{
}


core::~core(void)
{
}

std::wstring GetCurrentDirectory()
{
	wchar_t buffer[MAX_PATH];
	GetModuleFileNameW(nullptr, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring( buffer ).find_last_of( L"\\/" );
    return std::wstring( buffer ).substr( 0, pos);
}

std::wstring GetContentPath(const wchar_t* asset)
{
	std::wstring out = GetCurrentDirectory();	
	out.append(asset);
	return out;
}


} // game