#pragma once

#include <string>
#include <windows.h>

namespace path
{
	bool FileExists(const std::wstring& file);
	void FixPathDelimitersInPlace(wchar_t* path);
	void GetFileName(const std::wstring& path, std::wstring& out);
	void GetPath(const std::wstring& path, std::wstring& out);
	bool CreateFolder(const wchar_t* absolutePath);

} // path