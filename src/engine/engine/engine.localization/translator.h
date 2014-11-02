#pragma once

#include <map>
#include "engine.core/file.h"

namespace localization
{

	// Localization file syntax:
	// unique_id 
	// <language> <text>
	// <language> <text>
	// <language> <text>
	// <language> <text>
	// 
	enum class Language
	{
		English,
		Spanish,
		French
	};

	class translator
	{
	public:

		typedef std::map<unsigned int, std::wstring> database;

		database m_database;

		Language m_currentLanguage;

		static const std::wstring s_missingLocalizedText;

		const std::wstring& Get(unsigned int id);
		void SaveSampleData();
		void OpenConfiguration(const std::wstring& file, Language language);

	};

	

} // localization