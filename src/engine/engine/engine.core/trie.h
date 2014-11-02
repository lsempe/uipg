#pragma once

#include <vector>
#include <memory>
#include <iostream>

// A trie is a tree data structure specialized for retrieving text that matches a given prefix.
class trie
{
public:

	trie();
	void add(const std::wstring s);
	bool find(const std::wstring s);
	std::wstring nearest_match(const std::wstring& s) const;

	class node
	{

	public:

		node(wchar_t ch = 0);

		void MarkWord();
		std::shared_ptr<node> find(wchar_t character);

		wchar_t GetCharacter() const;
		bool IsWord() const;

		void push_back(std::shared_ptr<node> n);
		std::shared_ptr<node> next();

	private:
		wchar_t m_character;
		bool m_isWord;
		size_t m_index;
		
		std::vector<std::shared_ptr<node>> m_children;
	};

	std::shared_ptr<node> find_prefix(const std::wstring& prefix) const;
	bool contains_word(const std::wstring& word);
	bool contains_prefix(const std::wstring& prefix);

	bool get_from_prefix(std::shared_ptr<node> from, std::wstring& s);

	void Print(std::shared_ptr<node> n, std::wstring& s);

	void PrintAllNearest(const std::wstring& s);

	std::shared_ptr<node> get_node(const std::wstring& word);

private:

	bool contains(const std::wstring& word, bool matchWord = true);
	
	std::shared_ptr<node> m_root;

};