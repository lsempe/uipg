#include "gtest/gtest.h"
#include "engine.core/trie.h"

class TrieTest : public testing::Test
{
public:

	
};


TEST(TrieTest, ExampleTest)
{
	trie t;

	t.add(L"doctor");
	t.add(L"doctrine");
	t.add(L"document");
	t.add(L"doctorate");
	t.add(L"docile");
	t.add(L"ambitious");
	t.add(L"blasphemous");
	t.add(L"amphibian");
	
	EXPECT_FALSE(t.find(L"doc"));
	EXPECT_TRUE(t.find(L"doctor"));

	std::wstring outStr;
	auto n = t.get_node(L"doc");

	t.get_from_prefix(n->next(), outStr);
	outStr = L"";
	t.get_from_prefix(n->next(), outStr);
	outStr = L"";
	t.get_from_prefix(n->next(), outStr);
	outStr = L"";
	t.get_from_prefix(n->next(), outStr);

	EXPECT_TRUE(t.contains_word(L"doctor"));
	EXPECT_TRUE(t.contains_prefix(L"amp"));

	//t.PrintAllNearest(L"doc");
}
