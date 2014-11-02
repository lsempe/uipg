
#define GTEST_HAS_TR1_TUPLE 0

#include "gtest/gtest.h"
#include "engine.math/rectangle.h"
#include "engine.math/point.h"
#include "engine.math/math.h"

class RectangleTest : public testing::Test
{
public:

	
};

TEST(RectangleTest, Setup)
{
	math::rectangle rc(-1.f, -1.f, 2.f, 2.f);

	EXPECT_EQ( rc.Center(), math::point::Zero);
	EXPECT_FALSE(rc.IsEmpty());
	EXPECT_FLOAT_EQ(rc.Left(), -1.f);
	EXPECT_FLOAT_EQ(rc.Top(), -1.f);
	EXPECT_FLOAT_EQ(rc.Right(),  1.f);
	EXPECT_FLOAT_EQ(rc.Bottom(), 1.f);
	EXPECT_FLOAT_EQ(rc.Width(), 2.f);
	EXPECT_FLOAT_EQ(rc.Height(), 2.f);

	EXPECT_TRUE(rc == rc);
	EXPECT_TRUE(rc != math::rectangle());
}

TEST(RectangleTest, Intersection)
{
	math::rectangle r1(-1.f, -1.f, 2.f, 2.f);
	math::rectangle r2(0.f, 0.f, 1.f, 1.f);
	math::rectangle r3(0.f, 0.75f, 2.f, 2.f);
	math::rectangle r4(2.f, 2.f, 3.f, 1.f);

	EXPECT_TRUE(r1.Contains(r2));
	EXPECT_FALSE(r1.Contains(r3));
	EXPECT_TRUE(r1.Intersects(r3));
	EXPECT_FALSE(r2.Intersects(r4));

	math::rectangle r5 = math::rectangle::MakeRectangle(-5.f, -5.f, 10.f, 10.f);
	EXPECT_EQ(15.f, r5.Width());
	EXPECT_EQ(15.f, r5.Height());

	math::point p1 = math::point::Zero;
	math::point p2(1.f, 0.f);
	math::point p3(0.f, 1.f);
	math::point p4(2.f, 2.f);

	EXPECT_TRUE(r1.Contains(p1));
	EXPECT_TRUE(r1.Contains(p2));
	EXPECT_FALSE(r1.Contains(p4));
	
}

TEST(RectangleTest, Geometry)
{
	math::rectangle r1(-1.f, -1.f, 2.f, 2.f);
	math::rectangle r2(0.f, 0.f, 1.f, 1.f);
	
	EXPECT_EQ(r1.Right(), 1.f);
	EXPECT_EQ(r1.Bottom(), 1.f);

	math::point center1 = r1.Center();
	EXPECT_TRUE(math::IsEqual(center1.x(), 0.f) && math::IsEqual(center1.y(), 0.f));
	
	math::point center2 = r2.Center();
	EXPECT_TRUE(math::IsEqual(center2.x(), 0.5f) && math::IsEqual(center2.y(), 0.5f));
}

