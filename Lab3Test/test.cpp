#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication4/Functions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Lab3Test
{
    TEST_CLASS(Tests)
    {
    public:

        TEST_METHOD(TestMinimum)
        {
            vector<int> a = { 5, 2, 8, 1, 4 };
            Assert::AreEqual(1, findMin(a));
        }

        TEST_METHOD(TestMaximum)
        {
            vector<int> a = { 5, 2, 8, 1, 4 };
            Assert::AreEqual(8, findMax(a));
        }

        TEST_METHOD(TestAverage)
        {
            vector<int> a = { 2, 4, 6 };
            Assert::AreEqual(4.0, findAverage(a));
        }
    };
}
