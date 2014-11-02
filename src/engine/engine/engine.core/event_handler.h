#pragma once

#include <functional>
#include <list>

template <typename T>
class event_handler
{
public:

	typedef std::function<void(void*, T)> signature_t;
	typedef std::list<signature_t> functionList_t;

	void Invoke(void* sender, T data)
	{
		for ( auto& callback : m_callbacks )
		{
			callback(sender, data);
		}
	}

	typename functionList_t::iterator operator += (signature_t callback)
	{
		m_callbacks.emplace_front(callback);
		return m_callbacks.begin();
	}

	void operator -= (typename functionList_t::iterator& iterator)
	{
		m_callbacks.erase(iterator);
	}
	
private:

	functionList_t m_callbacks;
};

