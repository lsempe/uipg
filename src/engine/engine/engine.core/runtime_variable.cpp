#include "runtime_variable.h"
#include "engine.core/string_helper.h"

runtime_variable_database& runtime_variable::GetDatabase() 
{
	static runtime_variable_database database;
	return database;
}

runtime_variable::runtime_variable(const std::wstring name, const std::wstring description)
	: m_name(name)
	, m_description(description)
{
	GetDatabase().Register(this);
}

void runtime_variable_database::Print()
{
	for ( auto& it : m_variables )
	{
		std::wcout << it->Name() << "\t|\t" << it->Description() << "\t|\t<" << it->Type() << ">" << std::endl;
	}
}

void runtime_variable_database::OnRuntimeVariableDeletion(runtime_variable* variable)
{
	variable->GetDatabase().Unregister(variable);
}

bool runtime_variable::ParseBool(const wchar_t* str, bool& value)
{
	if ( wcslen(str) == 0 )
		return false;

	if ( _wcsicmp(str, L"true") == 0 || _wcsicmp(str, L"1") == 0 || _wcsicmp(str, L"yes") == 0  )
	{
		value = true;
		return true;
	}

	if ( _wcsicmp(str, L"false") == 0 || _wcsicmp(str, L"0") == 0 || _wcsicmp(str, L"no") == 0 )
	{
		value = false;
		return true;
	}
		
	return false;
}

bool runtime_variable::ParseFloat(const wchar_t* str, float& value)
{
	if ( wcslen(str) == 0 )
		return false;

	value = 0.f;
	if( swscanf_s(str, L"%f", &value) )
	{						
		return true;
	}
	return false;
}

bool runtime_variable::ParseInt(const wchar_t* str, int& value)
{
	if ( wcslen(str) == 0 )
		return false;

	value = 0;
	if( swscanf_s(str, L"%d", &value) )
	{			
		return true;
	}
	return false;
}

bool runtime_variable::ParseVector(const wchar_t* str, math::vector4& )
{
	if ( wcslen(str) == 0 )
		return false;

	std::vector<std::wstring> elems;
	helper::stringutils::SplitW(str, ',', elems);

	if ( elems.size() == 0 )
		return false;

	math::vector4 v = math::vector4::Zero;

	v.x() = static_cast<float>(_wtof( elems[0].c_str() ));

	if ( elems.size() > 1 )
	{
		v.y() = static_cast<float>(_wtof( elems[1].c_str() ));

		if ( elems.size() > 2 )
		{
			v.z() = static_cast<float>(_wtof( elems[2].c_str() ));

			if ( elems.size() > 3 )
			{
				v.w() = static_cast<float>(_wtof( elems[3].c_str() ));
			}
		}
	}		

	return true;
}
