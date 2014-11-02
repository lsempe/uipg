#include "trie.h"

trie::trie()
	: m_root(std::shared_ptr<node>(new node()))
{}

void trie::add(const std::wstring s)
{
	std::shared_ptr<node> current = m_root;

	if (s.size() == 0)
	{
		return;
	}

	for (size_t i = 0; i < s.size(); ++i)
	{
		std::shared_ptr<node> child = current->find(s.c_str()[i]);
		if (child != nullptr)
		{
			current = child;
		}
		else
		{
			std::shared_ptr<node> tmp = std::shared_ptr<node>(new node(s.c_str()[i]));
			current->push_back(tmp);
			current = tmp;
		}

		if (i == s.size() - 1)
		{
			current->MarkWord();
		}
	}
}

bool trie::find(const std::wstring s)
{
	std::shared_ptr<node> current = m_root;

	while (current != nullptr)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			std::shared_ptr<node> tmp = current->find(s.c_str()[i]);
			if (tmp == nullptr)
			{
				return false;
			}

			current = tmp;
		}

		if (current->IsWord())
			return true;
		else
			return false;
	}
	return false;
}

std::wstring trie::nearest_match(const std::wstring& s) const
{
	std::shared_ptr<node> current = m_root;
	std::shared_ptr<node> last = nullptr;
	std::wstring str;
	while (current != nullptr)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			std::shared_ptr<node> tmp = current->find(s.c_str()[i]);
			if (tmp == nullptr)
				return str;

			current = tmp;
			str += (s.c_str()[i]);

			if (current->IsWord())
			{
				last = current;
				str += (s.c_str()[i]);
			}
		}
	}
	return str;
}

trie::node::node(wchar_t ch /*= 0*/)
	: m_character(ch)
	, m_isWord(false)
	, m_index(0)
{

}

void trie::node::MarkWord()
{
	m_isWord = true;
}

std::shared_ptr<trie::node> trie::node::find(wchar_t character)
{
	for (auto& n : m_children)
	{
		if (n->GetCharacter() == character)
		{
			return n;
		}
	}
	return nullptr;
}

wchar_t trie::node::GetCharacter() const { return m_character; }

bool trie::node::IsWord() const { return m_isWord; }

void trie::node::push_back(std::shared_ptr<node> n)
{
	m_children.push_back(n);
}

std::shared_ptr<trie::node> trie::node::next()
{
	if (m_children.size() == 0)
		return nullptr;

	if (m_index >= m_children.size())
	{
		m_index = 0;
	}

	auto& next = m_children.at(m_index);
	++m_index;
	return next;
}


std::shared_ptr<trie::node> trie::find_prefix(const std::wstring& prefix) const
{
	if (prefix.size() == 0)
	{
		return nullptr;
	}

	std::shared_ptr<trie::node> current = m_root;
	std::shared_ptr<trie::node> last = nullptr;

	while (current != nullptr)
	{
		for (size_t i = 0; i < prefix.size(); ++i)
		{
			auto tmp = current->find(prefix.c_str()[i]);
			if (tmp == nullptr)
			{
				return nullptr;
			}

			current = tmp;

			if (!current->IsWord())
			{
				last = current;
			}
		}

		break;
	}

	if (!current->IsWord())
		return last;

	return nullptr;
}

bool trie::contains_word(const std::wstring& word)
{
	return contains(word);
}

bool trie::contains_prefix(const std::wstring& prefix)
{
	return contains(prefix, false);
}

bool trie::get_from_prefix(std::shared_ptr<trie::node> from, std::wstring& s)
{
	std::shared_ptr<trie::node> current = from;

	wchar_t c = current->GetCharacter();
	if (c != 0)
	{
		s.push_back(c);
	}

	if (current->IsWord())
	{
		return true;
	}

	std::shared_ptr<trie::node> n = current->next();
	while (n != nullptr)
	{
		if (get_from_prefix(n, s))
		{
			return true;
		}

		n = n->next();
	}

	current = current->next();

	return false;

}

void trie::Print(std::shared_ptr<trie::node> n, std::wstring& s)
{
	std::shared_ptr<node> current = n;
	while (current != nullptr)
	{
		wchar_t c = current->GetCharacter();
		if (c != 0)
			s.push_back(c);

		if (current->IsWord())
		{
			std::wcout << s.c_str() << std::endl;
			break;
		}

		std::shared_ptr<node> next = current->next();
		while (next != nullptr)
		{
			Print(next, s);
			next = next->next();
			s.pop_back();
		}

		current = current->next();
		if (current != nullptr)
		{
			if (s.back() != c)
				s.pop_back();
		}
	}
}

void trie::PrintAllNearest(const std::wstring& s)
{
	auto n = find_prefix(s);
	decltype(n) current = n;

	std::wstring s2;
	Print(n, s2);
}

std::shared_ptr<trie::node> trie::get_node(const std::wstring& word)
{
	auto current = m_root;

	for (size_t i = 0; i < word.size() && current != nullptr; ++i)
	{
		wchar_t character = word.c_str()[i];
		auto n = current->find(character);
		if (n == nullptr)
		{
			return nullptr;
		}
		current = n;
	}

	return current;
}

bool trie::contains(const std::wstring& word, bool matchWord /*= true*/)
{
	auto n = get_node(word);
	return (matchWord && n != nullptr && n->IsWord()) || (!matchWord && n != nullptr);
}
