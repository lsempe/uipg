#include "gtest/gtest.h"

#include "engine.ui/grid.h"

class GridTest : public testing::Test
{
public:

	
};

// Custom grid item data used for testing.
class griditem
{
public:
	griditem(char a = ' ') : test(a) {}
	griditem(const griditem& rhs) { test = rhs.test; }

	char test;

	virtual bool operator == (const griditem& rhs) const { return test == rhs.test; }
	virtual bool operator != (const griditem& rhs) const { return test != rhs.test; }
};


TEST(DefaultGridTest, Test)
{
	int remainder = 0;

	ui::grid<3, 3, griditem> testGrid;
	testGrid.Add(0,0, griditem('a'), 10, 8, 1, remainder);
	EXPECT_EQ(remainder, 2);

	testGrid.Remove(0,0,8);
	EXPECT_TRUE(testGrid.IsEmpty(0,0));

	testGrid.Add(1,0, griditem('b'), 2, 99, 0, remainder);
	testGrid.Add(2,0, griditem('c'), 3, 99, 0, remainder);
	testGrid.Add(2,0, griditem('c'), 7, 99, 0, remainder);

	testGrid.Add(0,1, griditem('d'), 4, 99, 0, remainder);
	testGrid.Add(1,1, griditem('e'), 5, 99, 0, remainder);
	testGrid.Add(2,1, griditem('f'), 6, 99, 0, remainder);
	testGrid.Remove(2, 1, 5);

	EXPECT_TRUE(testGrid(1, 0).Item().test == 'b');
	EXPECT_TRUE(testGrid(1, 0).Count() == 2);
	testGrid.Remove(1, 0, 2);
	EXPECT_TRUE(testGrid(1, 0).Count() == 0);

	testGrid.Swap(2, 0, 2, 1);
	EXPECT_EQ(testGrid(2, 0).Item().test, 'f');
	EXPECT_EQ(testGrid(2, 1).Item().test, 'c');

}
