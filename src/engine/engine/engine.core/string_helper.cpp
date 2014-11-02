#include "string_helper.h"

#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <malloc.h>
#include <sstream>

#include "crc.h"
#include "globals.h"

#include "engine.math/vector.h"

using namespace helper;

// Utility to convert strings to wide strings
// this is very costly, don't use in performance code
void stringutils::string2wstring(std::wstring &dest, const std::string &src)
{
	dest.resize(src.size());
	for (unsigned int i=0; i<src.size(); ++i)
		dest[i] = static_cast<unsigned char>(src[i]);
}

// Utility to convert wide strings to strings
// this is very costly, don't use in performance code

void stringutils::wstring2string(std::string &dest,const std::wstring &src)
{
	dest.resize(src.size());
	for (wchar_t i=0; i < src.size(); i++)
		dest[i] = src[i] < 256 ? static_cast<char>(src[i]) : ' ';
}

// Usage: ReplaceExtension(path, "ext"); // will replace any extension with ext
void stringutils::ReplaceExtension(char* path, char* newext)
{
	char* ext = strrchr(path, '.');
	if ( ext )
	{
		int result = (int)(ext - path + 1);
		path[result] = '\0';
		strcat_s(path, strlen(path), newext);
	}
	else
	{
		strcat_s(path, strlen(path), ".");
		strcat_s(path, strlen(path), newext);
	}
}

// Usage: ReplaceFileName(path, "file.ext"); // will replace the file with file.ext
void stringutils::ReplaceFileName(char* path, char* filename)
{
	int len = static_cast<int>( strlen(path) );
	for ( int i = len; i > 0; i-- )
	{
		if ( path[i] == '\\' )
		{
			path[i+1] = '\0';
			break;
		}
	}

	strcat_s(path, strlen(path), filename);
}

void stringutils::StripExtension(const char* path, std::string& out_stripped)
{
	char temp[globals::kMaxStringLength];
	strcpy_s(temp, globals::kMaxStringLength, path);

	int len = static_cast<int>( strlen(temp) );
	for ( int i = len; i > 0; i-- )
	{
		if ( temp[i] == '.' )
		{
			temp[i] = '\0';
			break;
		}
	}
	out_stripped = temp;
}

void stringutils::Split(std::string str, char delim, std::vector<std::string>& out_StrList)
{
	std::stringstream ss(str);
	char tmp[2048];
	while (ss.getline(tmp, 2048, delim))
	{
		out_StrList.push_back(tmp);
	}	
}

void stringutils::SplitW(std::wstring str, wchar_t delim, std::vector<std::wstring>& out_StrList)
{
	std::wstringstream ss(str);
	wchar_t tmp[2048];
	while (ss.getline(tmp, 2048, delim))
	{
		out_StrList.push_back(tmp);
	}	
}

std::string stringutils::GetIntValueAsString(int value, bool bUnsigned)
{
	std::string ret = "error";
	char buffer[globals::kMaxStringLength];
	
	if ( bUnsigned )
		_ultoa_s(value, buffer, globals::kMaxStringLength, 10);
	else
		_ltoa_s(value, buffer, globals::kMaxStringLength, 10);

	ret = buffer;

	return ret;
}

std::string stringutils::FloatToString(float fVal)
{
	char buffer[_CVTBUFSIZE];
   _gcvt_s( buffer, _CVTBUFSIZE, fVal, 12 ); // C4996

   std::string ret = buffer;
   return ret;
}

std::string stringutils::Vector3ToString(const math::vector3& v)
{
	std::string ret;

	char buffer[_CVTBUFSIZE];
   _gcvt_s( buffer, _CVTBUFSIZE, v.x(), 12 ); 

   ret = buffer;
   ret += ",";

   _gcvt_s( buffer, _CVTBUFSIZE, v.y(), 12 ); 
   ret += buffer;
   ret += ",";

   _gcvt_s( buffer, _CVTBUFSIZE, v.z(), 12 ); 
   ret += buffer;

   return ret;
}

math::vector3 stringutils::ParseVector3(const std::string& str)
{
	std::vector<std::string> elems;
	stringutils::Split(str, ',', elems);
	
	math::vector3 v;

	v.x() = static_cast<float>( atof( elems[0].c_str() ) );
	v.y() = static_cast<float>( atof( elems[1].c_str() ) );
	v.z() = static_cast<float>( atof( elems[2].c_str() ) );

	return v;
}

bool stringutils::StartsWith(const std::string& str, char ch)
{
	return ( str.length() != 0 && *str.begin() == ch );
}

bool stringutils::EndsWith(const std::string& str, char ch)
{
	return ( str.length() != 0 && *(str.end()-1) == ch );
}

std::string stringutils::FormatString(const char* format, ...)
{
	char szBuffer[globals::kMaxStringLength];

	va_list args;
	va_start(args, format);
	_vsnprintf_s(szBuffer, sizeof(szBuffer), globals::kMaxStringLength, format, args);
	va_end(args);

	std::string s = szBuffer;
	return s;
}

std::wstring stringutils::FormatStringW(const wchar_t* format, ...)
{
   std::wstring str(L"");

   if (format != nullptr)
   {
      va_list args = nullptr; 
      va_start(args, format); 
      
      size_t length = _vscwprintf(format, args) + 1;
               
      std::vector<wchar_t> buffer(length, L'\0');
      int nWritten = _vsnwprintf_s(&buffer[0], buffer.size(), length, format, args);    

      if (nWritten > 0)
      {
         str = &buffer[0];
      }
            
      va_end(args); 
   }

   return str; 
}

void stringutils::Reverse(wchar_t str[])
{
	size_t end = wcslen(str) - 1;
	size_t start = 0;

	while (end > start)
	{
		wchar_t ch = str[start];
		str[start] = str[end];
		str[end] = ch;
		++start;
		--end;
	}
}


Tag::Tag()
: str("")
, id(0)
{
}

Tag::Tag(const std::string& tag)
: str(tag)
{
	Init(tag.c_str());
}

Tag::Tag(const char* tag)
: str(tag)
{
	Init(tag);
}

void Tag::Init(const char* tag)
{
	if ( !tag )
	{
		id = UINT32_MAX;
		return;

	}

	char* pStr = const_cast<char*>( tag ); // generally bad practice, but it's ok here because its use is guaranteed to be readonly
	id = sys::CRC::Get().GetCRC32( reinterpret_cast<unsigned char*>( pStr ), sizeof(pStr) );
}

Tag& Tag::operator = (const char* str)
{
	Init(str);
	return *this;
}

bool Tag::operator == (const Tag& t) const
{
	return ( id == t.GetId() );
}

