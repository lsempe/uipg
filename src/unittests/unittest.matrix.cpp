
#define GTEST_HAS_TR1_TUPLE 0

#include "gtest/gtest.h"
#include "engine.math/matrix.h"

class MatrixTest : public testing::Test
{
public:
	
};


TEST(MatrixTest, Identities)
{
	math::matrix m = math::matrix::Identity;

	EXPECT_EQ(m.Right(),	math::vector3::UnitX);
	EXPECT_EQ(m.Up(),		math::vector3::UnitY);
	EXPECT_EQ(m.Forward(),	math::vector3::UnitZ);

	EXPECT_EQ(m.Left(),		-math::vector3::UnitX);
	EXPECT_EQ(m.Down(),		-math::vector3::UnitY);
	EXPECT_EQ(m.Backward(), -math::vector3::UnitZ);

}

TEST(MatrixTest, Multiplication)
{
	
	math::matrix i1 = math::matrix::Identity;
	math::matrix i2 = math::matrix::Identity;
	math::matrix t1 = math::matrix::CreateRotationX(math::Pi/4);

	math::matrix r1 = i1 * i2;
	EXPECT_EQ(r1, math::matrix::Identity);

	math::matrix r2 = t1 * i1;
	EXPECT_EQ(r2, t1);

	math::matrix r3 = i1 * t1;
	EXPECT_EQ(r3, t1);



	math::matrix m1 = math::matrix::Identity;
	math::matrix m2 = math::matrix::Identity;

	m1(0,0) = 1.f; m1(1,0) = 1.f;
	m1(1,1) = 1.f;

	m2(1,0) = 1.f;
	m2(1,1) = 1.f;

	r1 = m1 * m2;
	r2 = m2 * m1;

	/*EXPECT_FLOAT_EQ(r1(1,0), 2.f);
	EXPECT_FLOAT_EQ(r1(1,1), 1.f);

	EXPECT_FLOAT_EQ(r2(1,0), 1.f);
	EXPECT_FLOAT_EQ(r2(1,1), 1.f);*/

}

TEST(MatrixTest, Rotation)
{
	math::matrix r1 = math::matrix::CreateRotationX(math::Pi / 4.f);
	math::matrix r2 = math::matrix::CreateRotationX(-math::Pi / 4.f);

	math::matrix r3 = r1 * r2;

	EXPECT_EQ(r3, math::matrix::Identity);
		
	r1 = math::matrix::CreateRotationY(math::Pi / 4.f);
	r2 = math::matrix::CreateRotationY(-math::Pi / 4.f);

	r3 = r1 * r2;

	EXPECT_EQ(r3, math::matrix::Identity);

	r1 = math::matrix::CreateRotationZ(math::Pi / 4.f);
	r2 = math::matrix::CreateRotationZ(-math::Pi / 4.f);

	r3 = r1 * r2;

	EXPECT_EQ(r3, math::matrix::Identity);

}

TEST(MatrixTest, Inverse)
{
	math::matrix m1 = math::lefthanded::Perspective(1280.f, 720.f, 0.1f, 100.f);
	math::matrix im = math::matrix::Invert(m1);
	math::matrix m3 = im * m1;

	EXPECT_TRUE(m3 == math::matrix::Identity);


}
