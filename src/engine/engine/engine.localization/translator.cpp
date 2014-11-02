#include "translator.h"
#include <iostream>

namespace localization
{
	const std::wstring translator::s_missingLocalizedText = L"$> MISSING";

	const std::wstring& translator::Get(unsigned int id)
	{
		auto it = m_database.find(id);
		if ( it != m_database.end() )
		{
			return it->second;
		}

		return s_missingLocalizedText;
	}

	void translator::SaveSampleData()
	{
		core::text_file file;
		file.open(L"0_HelloWorld.txt", core::file::Mode::Write);
		file.write_line(L"0");
		file.write_line(L"en: Hello World!");
		file.write_line(L"fr: Bonjour le Monde!");
		file.write_line(L"sp: Hola Mundo!");
		file.close();

		file.open(L"1_Open.txt", core::file::Mode::Write);
		file.write_line(L"1");
		file.write_line(L"en: Open");
		file.write_line(L"fr: Ouvrir");
		file.write_line(L"sp: Abrir");
		file.close();

		file.open(L"2_Close.txt", core::file::Mode::Write);
		file.write_line(L"2");
		file.write_line(L"en: Close");
		file.write_line(L"fr: Fermer");
		file.write_line(L"sp: Cerrar");
		file.close();


		file.close();
	}

	void translator::OpenConfiguration(const std::wstring& file, Language language)
	{
		m_currentLanguage = language;

		core::text_file f;
		f.open(file, core::file::Mode::Read);

		std::wstring line;			
		f.read_line(line);
		int id = _wtoi(line.c_str());

		auto it = m_database.find(id);
		if ( it != m_database.end() )
		{
			// There is a conflicting ID in the database, report the error
			return;
		}

		while (f.read_line(line) != 0)
		{				
			std::wcout << line << std::endl;
			if (line.empty())
				break;

			size_t index = line.find_first_of(L':');
			std::wstring languageStr = line.substr(0, index);
			std::wstring text = line.substr(index+1, text.size() - index);
			if (text.c_str()[0] == ' ')
				text.erase(0,1);

			Language lang = Language::English;
			if ( wcscmp(languageStr.c_str(), L"en") == 0)
				lang = Language::English;
			else
			if ( wcscmp(languageStr.c_str(), L"fr") == 0)
				lang = Language::French;
			else
			if ( wcscmp(languageStr.c_str(), L"sp") == 0)
				lang = Language::Spanish;
			
			if ( lang == m_currentLanguage )
			{
				m_database.emplace(id, text);
				break;
			}
		}

		f.close();
	}
}