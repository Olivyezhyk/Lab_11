#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <limits>
#include "../Lab_11_01/Lab_11_01.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestFindMaxOddMinEven)
        {
            const char* fname = "test.dat";
            std::ofstream fout(fname, std::ios::binary);
            int numbers[] = { 1, 2, 3, 4, 5, 6 };
            for (int num : numbers) {
                fout.write(reinterpret_cast<char*>(&num), sizeof(num));
            }
            fout.close();

            int maxOdd, minEven;
            bool result = FindMaxOddMinEven(fname, maxOdd, minEven);

            Assert::IsTrue(result, L"Функція не знайшла жодного непарного чи парного числа.");
            Assert::AreEqual(5, maxOdd, L"Максимальне непарне число неправильне.");
            Assert::AreEqual(2, minEven, L"Мінімальне парне число неправильне.");
        }
    };
}
