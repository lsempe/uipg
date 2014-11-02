
#define GTEST_HAS_TR1_TUPLE 0

#include "gtest/gtest.h"
#include "engine.render/viewport.h"
#include "engine.math/vector.h"
#include "engine.math/matrix.h"


TEST(ViewportTest, Projections)
{
	render::viewport viewport(0.f, 0.f, 1280.f, 720.f, 0.f, 1.f);

	math::vector3 worldPosition(0.f, 0.f, 0.f);
	math::matrix world = math::matrix::Identity;
	math::matrix view = math::lefthanded::LookAt(math::vector3(0,0,-10), math::vector3::Zero, math::vector3::UnitY);
	math::matrix projection;
	math::lefthanded::PerspectiveFOV(math::Pi/4.f, viewport.Aspect(), 0.1f, 3000.f, projection);

	math::vector3 projected = viewport.Project(math::vector3::Zero, projection, view, world);

	EXPECT_FLOAT_EQ(projected.x(), 640.f);
	EXPECT_FLOAT_EQ(projected.y(), 360.f);

	math::vector3 screenPosition1(640.f, 360.f, 0.f);
	math::vector3 screenPosition2(640.f, 360.f, 1.f);

	math::vector3 unprojected = viewport.Unproject(screenPosition1, projection, view, world);
	math::vector3 unprojected1 = viewport.Unproject(screenPosition2, projection, view, world);
	
	EXPECT_FLOAT_EQ(unprojected.x(), 0.f);
	EXPECT_FLOAT_EQ(unprojected.y(), 0.f);
	EXPECT_TRUE(unprojected.z() < unprojected1.z());

//	const math::rectangle& titleSafeArea = viewport.TitleSafeAera();
	
}