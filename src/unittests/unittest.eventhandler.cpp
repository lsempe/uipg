#include "gtest/gtest.h"

#include "engine.core/event_handler.h"

class EventHandlerTest : public testing::Test
{
public:

	EventHandlerTest()
	{
	}

	void Callback(void* /*sender*/, int& d)
	{
		d = 0x2;
	}
};

void staticTest(void* /*s*/, int& d)
{
	d = 0x4;
}

TEST_F(EventHandlerTest, Events)
{
	// Lambda
	event_handler<int&> handler;
	auto lambda = [](void*, int& d) { d = 0x6; };
	auto functionHandle = handler += lambda;

	int value = 0;
	handler.Invoke(this, value);
	EXPECT_EQ(value, 0x6);

	handler -= functionHandle;

	// Member function
	using namespace std::placeholders;
	
	auto memberFunction = std::bind(&EventHandlerTest::Callback, this, _1, _2);
	functionHandle = handler += memberFunction;

	value = 0;
	handler.Invoke(this, value);
	EXPECT_EQ(value, 0x2);

	handler -= functionHandle;


	// Static function
	functionHandle = handler += &staticTest;

	value = 0;
	handler.Invoke(this, value);

	EXPECT_EQ(value, 0x4);

	handler -= functionHandle;

}
