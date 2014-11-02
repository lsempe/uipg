#include "gtest/gtest.h"

#include "engine.localization/translator.h"

class LocalizationTest : public testing::Test
{
public:

	
	
};

TEST(LocalizationTest, Translation)
{
	localization::translator translator;

	translator.SaveSampleData();

	const localization::Language currentLanguage = localization::Language::French;

	translator.OpenConfiguration(L"0_HelloWorld.txt", currentLanguage); 
	translator.OpenConfiguration(L"1_Open.txt", currentLanguage);
	translator.OpenConfiguration(L"2_Close.txt", currentLanguage);

	EXPECT_EQ(translator.Get(0), L"Bonjour le Monde!");
	EXPECT_EQ(translator.Get(1), L"Ouvrir");
	EXPECT_EQ(translator.Get(2), L"Fermer");
	EXPECT_EQ(translator.Get(3), L"$> MISSING");
}
