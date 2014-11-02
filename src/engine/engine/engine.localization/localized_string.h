#pragma once

namespace localization
{

enum eLanguage
{
	English,
	Spanish,
	French,
	SupportedLanguageCount	
};

class localized_string
{
public:

	wchar_t* m_english;

	wchar_t* m_text[SupportedLanguageCount];

};

} // localization