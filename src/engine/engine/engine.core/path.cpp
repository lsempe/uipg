#include "path.h"
#include <iostream>
#include <fstream>
#include <list>
#include <tchar.h>

namespace path
{

bool FileExists(const std::wstring& file)
{
  std::ifstream stream(file);
  return stream.good();
}

void FixPathDelimitersInPlace(wchar_t* path)
{
	wchar_t ch = *path;
	while (ch != L'\0')
	{		
		if (ch == L'/')
			*path = L'\\';

		ch = *++path;
	};
}

void GetFileName(const std::wstring& path, std::wstring& out)
{
	wchar_t fixed[MAX_PATH];
	wcscpy_s(fixed, MAX_PATH, path.c_str());
	FixPathDelimitersInPlace(fixed);
	std::wstring tmp = fixed;
	size_t i = tmp.find_last_of('\\');
	out = tmp.substr(i+1, tmp.size());
}

void GetPath(const std::wstring& path, std::wstring& out)
{
	wchar_t fixed[MAX_PATH];
	wcscpy_s(fixed, MAX_PATH, path.c_str());
	FixPathDelimitersInPlace(fixed);
	std::wstring tmp = fixed;
	size_t i = tmp.find_last_of('\\');
	out = tmp.substr(0, i);
}

bool CreateFolder(const wchar_t* absolutePath)
{
	wchar_t fixed[MAX_PATH];

	wcscpy_s(fixed, MAX_PATH, absolutePath);

	FixPathDelimitersInPlace(fixed);

	std::list<std::wstring> pathList;
	std::wstring src = fixed ;
	size_t i = 0;
	do {
		
		i = src.find_last_of(L'\\');
		if (i == std::wstring::npos )
			pathList.push_front( src );
		else
		{
			pathList.push_front( src.substr( i, src.length() - i ) );
			src = src.substr(0, i);
		}

	} while (i > 0 && i != std::string::npos );

	std::wstring out = L"";
	std::list<std::wstring>::iterator it = pathList.begin();
	for (; it != pathList.end(); ++it )
	{
		out += *it;
		_wmkdir(out.c_str());
	}

	return true;
}

}