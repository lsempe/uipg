#pragma once

#include "globals.h"

#include <vector>
#include <string>

namespace math
{
	class vector3;
}

namespace helper
{

class stringutils
{
public:

	// Utility to convert strings to wide strings
	static void string2wstring(std::wstring &dest,const std::string &src);

	// Utility to convert wide strings to strings
	static void wstring2string(std::string &dest,const std::wstring &src);

	// Usage: ReplaceExtension(path, "ext"); // will replace any extension with ext
	static void ReplaceExtension(char* path, char* ext);

	// Usage: ReplaceFileName(path, "file.ext"); // will replace the file with file.ext
	static void ReplaceFileName(char* path, char* filename);

	static void StripExtension(const char* path, std::string& out_stripped);

	// Usage Split(string, ',', elems); // will return an array of elements, without the delimiter char
	static void Split(std::string str, char delim, std::vector<std::string>& out_StrList);

	static void SplitW(std::wstring str, wchar_t delim, std::vector<std::wstring>& out_StrList);

	static std::string GetIntValueAsString(int value, bool bUnsigned = false);

	static std::string FloatToString(float fVal);

	static std::string Vector3ToString(const math::vector3& v);

	static math::vector3 ParseVector3(const std::string& str);

	static bool StartsWith(const std::string& str, char ch);

	static bool EndsWith(const std::string& str, char ch);

	static std::string FormatString(const char* fmt, ...);

	static std::wstring FormatStringW(const wchar_t* fmt, ...);

	// Reverses an array of wchar_t, expects to have a 0 terminator.
	static void Reverse(wchar_t str[]);

};

template <typename T>
typename std::enable_if<std::is_signed<T>::value, std::string>::type IntegerToString(T value)
{
	std::string ret = "error";
	char buffer[globals::kMaxStringLength];
	_ltoa_s(value, buffer, globals::kMaxStringLength, 10);
	ret = buffer;
	return ret;
}

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, std::string>::type IntegerToString(T value)
{
	std::string ret = "error";
	char buffer[globals::kMaxStringLength];
	_ultoa_s(value, buffer, globals::kMaxStringLength, 10);
	ret = buffer;
	return ret;
}

class Tag
{
protected:
	unsigned int id;
	std::string str;

public:
	Tag();
	Tag(const std::string& tag);
	Tag(const char* tag);

	Tag& operator = (const char* str);
	bool operator == (const Tag& t) const;

	unsigned int GetId() const { return id; }
	const char* c_str() const { return str.c_str(); }
	
	operator unsigned int() { return id; }
	operator const char*() { return str.c_str(); }

protected:
	void Init(const char* tag);
};


} // helper