#include "gtest/gtest.h"

#include "engine.core/string_helper.h"

class StringHelperTest : public testing::Test
{
public:
};

TEST(StringHelperTest, Default)
{	
	int v0 = -1234;
	std::string a = helper::IntegerToString(v0);

	unsigned int v1 = 4321;
	std::string b = helper::IntegerToString(v1);

	EXPECT_TRUE(a == "-1234");
	EXPECT_TRUE(b == "4321");

	std::wstring wstr1(L"1234 5.67");
	std::wstring wstr2 = helper::stringutils::FormatStringW(L"%d %.2f", 1234, 5.67);
	EXPECT_TRUE(wstr1.compare(wstr2) == 0);

	std::string str1("1234 5.67");
	std::string str2 = helper::stringutils::FormatString("%d %.2f", 1234, 5.67);
	EXPECT_TRUE(str1.compare(str2) == 0);

	EXPECT_TRUE(helper::stringutils::StartsWith(str1, '1'));
	EXPECT_TRUE(helper::stringutils::EndsWith(str1, '7'));

	std::wstring wstr3;
	helper::stringutils::string2wstring(wstr3, str1);
	EXPECT_TRUE(wstr3.compare(wstr1) == 0);

	std::string str3;
	helper::stringutils::wstring2string(str3, wstr1);
	EXPECT_TRUE(str3.compare(str1) == 0);

	std::vector<std::string> strArray;
	std::string str4 = "The brown fox jumped over the white dog.";
	helper::stringutils::Split(str4, ' ', strArray);
	EXPECT_TRUE(strArray.size() == 8);
	EXPECT_TRUE(strArray[0].compare("The") == 0);
	EXPECT_TRUE(strArray[7].compare("dog.") == 0);

	std::vector<std::wstring> wstrArray;
	std::wstring wstr4 = L"The brown fox jumped over the white dog.";
	helper::stringutils::SplitW(wstr4, L' ', wstrArray);
	EXPECT_TRUE(wstrArray.size() == 8);
	EXPECT_TRUE(wstrArray[0].compare(L"The") == 0);
	EXPECT_TRUE(wstrArray[7].compare(L"dog.") == 0);

	wchar_t chars[] = { 'A', 'B', 'C', 0 };
	helper::stringutils::Reverse(chars);
	EXPECT_TRUE(chars[0] == 'C');
	EXPECT_TRUE(chars[1] == 'B');
	EXPECT_TRUE(chars[2] == 'A');


}