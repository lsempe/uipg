#pragma once

namespace core
{

template <typename T>
class singleton
{
public:

	static T& Get()
	{
		static T instance;
		return instance;
	}	

protected:

	singleton() {}
	singleton(const singleton&) {}
	singleton(const singleton&&) {}
	singleton& operator = (const singleton&) { return *this; }

	friend typename T;
};

} // core