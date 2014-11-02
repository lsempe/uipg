
#define GTEST_HAS_TR1_TUPLE 0

#include "gtest/gtest.h"
#include "engine.math/vector.h"
#include "engine.math/matrix.h"


class VectorTest : public testing::Test
{
public:
	
};


TEST(VectorTest, Basics)
{
	math::matrix m = math::matrix::Identity;


	
}

TEST(VectorTest, Transforms)
{
	math::vector4 one = math::vector4::One;
	math::matrix scale = math::matrix::CreateUniformScale(10.f);
	math::vector4 ten = math::vector4::Transform(one, scale);

	EXPECT_EQ(ten.x(), 10.f);
	EXPECT_EQ(ten.y(), 10.f);
	EXPECT_EQ(ten.z(), 10.f);
}

TEST(VectorTest, Vector3)
{
	math::vector3 one = math::vector3::One;

	EXPECT_FLOAT_EQ(one.x(), 1.f);
	EXPECT_FLOAT_EQ(one.y(), 1.f);
	EXPECT_FLOAT_EQ(one.z(), 1.f);

	math::matrix scale = math::matrix::CreateUniformScale(10.f);
	math::vector3 ten = math::vector3::Transform(one, scale);

	EXPECT_FLOAT_EQ(ten.x(), 10.f);
	EXPECT_FLOAT_EQ(ten.y(), 10.f);
	EXPECT_FLOAT_EQ(ten.z(), 10.f);

	math::matrix translation = math::matrix::CreateTranslation(5.f, 6.f, 7.f);
	math::matrix c = scale * translation;
	math::vector3 scaledOffset = math::vector3::Transform(one, c);

	EXPECT_FLOAT_EQ(scaledOffset.x(), 15.f);
	EXPECT_FLOAT_EQ(scaledOffset.y(), 16.f);
	EXPECT_FLOAT_EQ(scaledOffset.z(), 17.f);
}
