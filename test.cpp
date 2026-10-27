#include "pch.h"
#include <gtest/gtest.h>
#include "../ConsoleApplication4/Functions.h"

TEST(Tests, TestMinimum)
{
    vector<int> a = { 5, 2, 8, 1, 4 };
    EXPECT_EQ(1, findMin(a));
}

TEST(Tests, TestMaximum)
{
    vector<int> a = { 5, 2, 8, 1, 4 };
    EXPECT_EQ(8, findMax(a));
}

TEST(Tests, TestAverage)
{
    vector<int> a = { 2, 4, 6 };
    EXPECT_DOUBLE_EQ(4.0, findAverage(a));
}