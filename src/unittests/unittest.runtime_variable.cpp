#include "gtest/gtest.h"

#include "engine.core/runtime_variable.h"
#include "engine.math/vector.h"

class RuntimeVariableTest : public testing::Test
{
public:

	runtime_variable_function m_functionTest;


	RuntimeVariableTest()
		: m_functionTest(L"functionTest", L"a test function", std::bind(&RuntimeVariableTest::functionTest, this, std::placeholders::_1))
	{
	}

	bool functionTest(std::vector<std::wstring> parameters)
	{
		return true;
	}

};



TEST_F(RuntimeVariableTest, Default)
{

	runtime_variable_int varInt(L"test", L"test case", 0, 0, 100);
	EXPECT_EQ(varInt, 0);
	varInt.Set(4);
	EXPECT_EQ(varInt, 4);
	varInt = 2;
	EXPECT_EQ(varInt, 2);

	runtime_variable_float varFloat(L"test2", L"test case 2", 0.f, 0.f, 100.f);
	EXPECT_FLOAT_EQ(varFloat, 0.f);

	varFloat.Set(6.f);
	EXPECT_FLOAT_EQ(varFloat, 6.f);

	varFloat = 1.f;
	EXPECT_FLOAT_EQ(varFloat, 1.f);

	runtime_variable_bool var_boolFalse(L"boolFalse", L"testing false bool", false);
	EXPECT_FALSE(var_boolFalse);

	runtime_variable_bool var_boolTrue(L"boolTrue", L"testing true bool", true);
	EXPECT_TRUE(var_boolTrue);

	var_boolTrue = !var_boolTrue;
	EXPECT_FALSE(var_boolTrue);

	runtime_variable_string var_string1(L"stringTest1", L"testing a string", L"default string");
	var_string1.Set(L"new string");
	std::wstring s(L"test");
	var_string1 = L"Hello World!";

	runtime_variable_vector var_vector(L"vectorTest", L"testing a vector", math::vector4::One);
	EXPECT_FLOAT_EQ(var_vector.Get().x(), 1.f);

	EXPECT_TRUE(m_functionTest(L""));

	runtime_variable::GetDatabase().Print();

}