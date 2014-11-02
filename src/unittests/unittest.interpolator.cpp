#include "gtest/gtest.h"

#include "engine.ui/interpolator.h"

class InterpolatorTest : public testing::Test
{
public:

	
};


TEST(InterpolatorTest, Default)
{
	using namespace ui::curves;

	ui::interpolator interpolator;

	bool animating = true;
	float someValue = 0.f;
	
	ui::animation anim(someValue, 1.0, 10.0, ease_in<quadratic>,  
		[&](float v){ someValue = v;  },
		[&]{ std::cout << "animdone" << std::endl; animating = false; });

	interpolator.Add(anim);

	float time = 0.f;

	// Will run animation until animation is done
	while (animating)
	{		
		interpolator.Update(time);
		time += 1.f / 30.f;
	}

	EXPECT_GE(someValue, 1.f);
}
