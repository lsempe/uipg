#include "gtest/gtest.h"

#include "engine.ui/digits.h"

class UITest : public testing::Test
{
public:
	
};

TEST(UITest, Default)
{
	ui::digits d(56789);
	EXPECT_EQ(d(0), L'5');
	EXPECT_EQ(d(1), L'6');
	EXPECT_EQ(d(2), L'7');
	EXPECT_EQ(d(3), L'8');
	EXPECT_EQ(d(4), L'9'); 
	EXPECT_EQ(d(5), 0); // out of bounds 

}
