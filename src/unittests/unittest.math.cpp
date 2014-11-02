#include "gtest/gtest.h"

#include "engine.math/math.h"

class MathTest : public testing::Test
{
public:

};

TEST(MathTest, Functions)
{
	using namespace math;

	signed int sint = -1;
	sint = Abs(sint);
	EXPECT_TRUE(sint > 0);

	unsigned int uint = 1;
	uint = Abs(uint);
	EXPECT_TRUE(uint > 0);

	float x = 2.f;
	float xx = Square(x);
	EXPECT_FLOAT_EQ(xx, 4.f);

	EXPECT_FLOAT_EQ(Min(x, xx), x);
	EXPECT_FLOAT_EQ(Max(x, xx), xx);

	float y = 5.f;
	Clamp(y, x, xx);
	EXPECT_FLOAT_EQ(y, xx);

	y = 0.f;
	Clamp(y, x, xx);
	EXPECT_FLOAT_EQ(y, x);

	int q = 1;
	int r = 2;
	EXPECT_TRUE(IsOdd(q));
	EXPECT_FALSE(IsOdd(r));

	EXPECT_TRUE(IsEven(r));
	EXPECT_FALSE(IsEven(q));

	EXPECT_FLOAT_EQ(RadiansToDegrees(Pi / 4.f), 45.f);

	EXPECT_FLOAT_EQ(DegreesToRadians(180.f), Pi);

}
