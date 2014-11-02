#pragma once

#include <math.h>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <wchar.h>

#include "sys.h"
#include "trie.h"
#include "engine.math/math.h"
#include "engine.math/vector.h"
#include "engine.core/string_helper.h"
#include "engine.core/event_handler.h"

namespace runtime_variables
{
	enum eType
	{
		Undefined,
		Int,
		Bool,
		Float,
		String,
		Vector,
		Function,
		COUNT
	};

	static const wchar_t* g_typeStr[] =
	{
		L"Undefined",
		L"Int",
		L"Bool",
		L"Float",
		L"String",
		L"Vector",
		L"Function",
		L""
	};
}

class runtime_variable_database;

/// runtime_variable
/// The base class for all runtime variables, holds the descriptive properties for a variable
/// and the static database of all runtime variables
class runtime_variable
{
public:
	runtime_variable(const std::wstring name, const std::wstring description);
	virtual ~runtime_variable() {}

	const std::wstring& Name() const { return m_name; }
	const std::wstring& Description() const { return m_description; }
	
	virtual const std::wstring Value() const = 0;
	virtual const wchar_t* Type() const = 0;
	virtual bool Parse(const wchar_t* str) = 0;
	
	static runtime_variable_database& GetDatabase();

	static bool ParseBool(const wchar_t* str, bool& value);	
	static bool ParseFloat(const wchar_t* str, float& value);
	static bool ParseInt(const wchar_t* str, int& value);
	static bool ParseVector(const wchar_t* str, math::vector4& value);

private:

	std::wstring m_name;
	std::wstring m_description;

};

/// runtime_variable_type
/// Base templated runtime variable type, allows for the creation of specialized runtime variable types.
/// T is the internal type (ie. int, float, char)
/// type is bound to the runtime_variables::eType enum, it is used for classification and filtering and is not strictly required
template <typename T, runtime_variables::eType type = Undefined>
class runtime_variable_type : public runtime_variable
{
public:

	runtime_variable_type(const std::wstring name, const std::wstring description, T defaultValue, T minValue, T maxValue, runtime_variables::eType _type = type)
		: runtime_variable(name, description)
		, m_value(defaultValue)
		, m_defaultValue(defaultValue)
		, m_minValue(minValue)
		, m_maxValue(maxValue)
		, m_type(_type)
	{}
		
	virtual ~runtime_variable_type() {}

	T& Get() { return m_value; }
	virtual const wchar_t* Type() const { return runtime_variables::g_typeStr[ (int)m_type ]; }	

	operator T() { return m_value; }
	
	template <typename T>
	typename std::enable_if<std::is_arithmetic<T>::value, void>::type Set(T value)
	{
		m_value = value;
		math::Clamp(m_value, m_minValue, m_maxValue);	
		InvokeOnChange();
	}

	template <typename T>
	typename std::enable_if<!std::is_arithmetic<T>::value, void>::type Set(T value)
	{
		m_value = value;
		InvokeOnChange();
	}

	void Revert()
	{
		m_value = m_defaultValue;
		InvokeOnChange();
	}

	bool operator == (const T& value) const { return m_value == value; }
		
	event_handler<int>& OnChange() { return m_onChange; }

protected:
	
	T m_value;
	T m_minValue;
	T m_maxValue;
	T m_defaultValue;
	
	event_handler<int> m_onChange;
	void InvokeOnChange()
	{
		m_onChange.Invoke(this, 0);
	}

	runtime_variables::eType m_type;

};

class runtime_variable_int : public runtime_variable_type<int, runtime_variables::Int>
{
public:
	runtime_variable_int(const std::wstring name, const std::wstring description, int defaultValue, int minValue, int maxValue)
		: runtime_variable_type<int, runtime_variables::Int>(name, description, defaultValue, minValue, maxValue, runtime_variables::Int)
	{}

	runtime_variable_int& operator = (int value) { Set(value); return *this; }

	virtual bool Parse(const wchar_t* str)
	{		
		int value = 0;
		if ( runtime_variable::ParseInt(str, value) )
		{
			Set(value);
			return true;
		}

		return false;
	}
		
	virtual const std::wstring Value() const 
	{
		wchar_t buffer[_CVTBUFSIZE];			
		_itow_s(m_value, buffer, 10);
		return std::wstring(buffer);
	}
};

class runtime_variable_float : public runtime_variable_type<float, runtime_variables::Float>
{
public:
	runtime_variable_float(const std::wstring name, const std::wstring description, float defaultValue, float minValue, float maxValue)
		: runtime_variable_type<float, runtime_variables::Float>(name, description, defaultValue, minValue, maxValue, runtime_variables::Float)
	{}

	runtime_variable_float& operator = (float value) { Set(value); return *this; }

	virtual bool Parse(const wchar_t* str)
	{		
		float value = 0.f;
		if ( runtime_variable::ParseFloat(str, value) )
		{
			Set(value);
			return true;
		}
		return false;
	}

	virtual const std::wstring Value() const 
	{
		char tmpbuffer [_CVTBUFSIZE];
		_gcvt_s(tmpbuffer,sizeof(tmpbuffer),m_value,8);
		wchar_t buffer[_CVTBUFSIZE];
		mbstowcs_s(NULL,buffer,sizeof(buffer)/2,tmpbuffer,sizeof(tmpbuffer));
		return std::wstring(buffer);
	}
};

class runtime_variable_bool : public runtime_variable_type<bool, runtime_variables::Bool>
{
public:
	runtime_variable_bool(const std::wstring name, const std::wstring description, bool defaultValue)
		: runtime_variable_type<bool, runtime_variables::Bool>(name, description, defaultValue, false, true, runtime_variables::Bool)
	{}
		
	runtime_variable_bool& operator = (bool value) { Set(value); return *this; }

	virtual bool Parse(const wchar_t* str)
	{
		bool value = false;
		if ( runtime_variable::ParseBool(str, value) )
		{
			Set(value);
			return true;
		}
		return false;
	}

	virtual const std::wstring Value() const 
	{			
		return std::wstring(m_value ? L"true" : L"false");
	}
};

class runtime_variable_string : public runtime_variable_type<std::wstring, runtime_variables::String>
{
public:
	runtime_variable_string(const std::wstring name, const std::wstring description, std::wstring defaultValue)
		: runtime_variable_type<std::wstring, runtime_variables::String>(name, description, defaultValue, std::wstring(), std::wstring(), runtime_variables::String)
	{}
		
	void Set(const wchar_t* str)
	{
		std::wstring s = str;
		runtime_variable_type<std::wstring, runtime_variables::String>::Set(s);
	}
	
	runtime_variable_string& operator = (const wchar_t* value) { Set(value); return *this; }

	virtual bool Parse(const wchar_t* str)
	{
		Set(str);
		return true;
	}

	virtual const std::wstring Value() const 
	{			
		return m_value;
	}
};

class runtime_variable_vector : public runtime_variable_type<math::vector4, runtime_variables::Vector>
{
public:
	runtime_variable_vector(const std::wstring name, const std::wstring description, const math::vector4& defaultValue)
		: runtime_variable_type<math::vector4, runtime_variables::Vector>(name, description, defaultValue, math::vector4::Zero, math::vector4::Zero, runtime_variables::Vector)
	{}

	virtual bool Parse(const wchar_t* str)
	{
		math::vector4 value = math::vector4::Zero;
		if ( runtime_variable::ParseVector(str, value) )
		{
			Set(value);
			return true;
		}
		return false;
	}
	
	virtual const std::wstring Value() const 
	{			
		return L"";
	}
};

typedef std::function<bool(std::vector<std::wstring>)> runtime_variable_function_signature_t;
class runtime_variable_function : public runtime_variable_type<runtime_variable_function_signature_t, runtime_variables::Function>
{
public:
	runtime_variable_function(const std::wstring name, const std::wstring description, runtime_variable_function_signature_t function)
		: runtime_variable_type<runtime_variable_function_signature_t, runtime_variables::Function>(name, description, function, nullptr, nullptr, runtime_variables::Function)
	{}

	bool operator ()(const wchar_t* parameters)
	{
		return Parse(parameters);
	}

	// TODO: Add helper functions to parse int/float/vector/string - callable from the operator ()

	virtual bool Parse(const wchar_t* str)
	{
		m_functionParameters.clear();
		helper::stringutils::SplitW(str, L' ', m_functionParameters);

		return m_value(m_functionParameters);
	}

	virtual const std::wstring Value() const 
	{			
		return L"";
	}

private:

	std::vector<std::wstring> m_functionParameters;

};

class runtime_variable_database
{
public:
	runtime_variable_database()
	{
		m_trie = std::shared_ptr<trie>(new trie());
	}

	// Registers a pointer to a runtime_variable, does not claim ownership of variable
	void Register(runtime_variable* variable)
	{
		m_trie->add(variable->Name());
		m_variables.push_back(std::shared_ptr<runtime_variable>(variable, OnRuntimeVariableDeletion));
	}

	void Print();

	const std::vector<std::shared_ptr<runtime_variable>>& GetVariables() const { return m_variables; }

	std::shared_ptr<trie> GetTrie() const { return m_trie; }

private:

	// When the shared_ptr goes out of scope it will attempt to delete it, given that this class did not
	// claim ownership of the pointer, we just make sure we no longer keep the pointer in the m_variables
	// vector.
	static void OnRuntimeVariableDeletion(runtime_variable* variable);

	// Finds and removes the runtime_variable from m_variables.
	void Unregister(runtime_variable* variable)
	{
		std::vector<std::shared_ptr<runtime_variable>>::iterator it = m_variables.begin();
		for ( auto& v : m_variables )
		{
			if ( v.get() == variable )
				break;

			++it;
		}

		if ( it != m_variables.end() )
		{
			m_variables.erase(it);
		}
	}

	std::vector<std::shared_ptr<runtime_variable>> m_variables;

	std::shared_ptr<trie> m_trie;
};	
