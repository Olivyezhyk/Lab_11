#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "../Lab_11_03_A/Lab_11_03_A.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest4
{

    TEST_CLASS(UnitTest4)
    {
    public:

        TEST_METHOD(TestSortStudents)
        {
            Student_A students[3] = {
                {"Smith", 3, COMPUTER_SCIENCE, 4, 5, 5},
                {"Jones", 2, INFORMATICS, 5, 4, 5},
                {"Taylor", 1, PHYSICS_INFORMATICS, 3, 5, 4}
            };

            int count = 3;
            SortStudents(students, count);

            Assert::AreEqual(string(students[0].surname), string("Jones"));
            Assert::AreEqual(students[0].course, 2);
            Assert::AreEqual(students[0].physics_grade, 5);

            Assert::AreEqual(string(students[1].surname), string("Smith"));
            Assert::AreEqual(students[1].course, 3);
            Assert::AreEqual(students[1].physics_grade, 4);

            Assert::AreEqual(string(students[2].surname), string("Taylor"));
            Assert::AreEqual(students[2].course, 1);
            Assert::AreEqual(students[2].physics_grade, 3);
        }

        TEST_METHOD(TestBinarySearch)
        {
            Student_A students[3] = {
                {"Smith", 3, COMPUTER_SCIENCE, 4, 5, 5},
                {"Jones", 2, INFORMATICS, 5, 4, 5},
                {"Taylor", 1, PHYSICS_INFORMATICS, 3, 5, 4}
            };

            int count = 3;

            bool found = BinarySearch(students, count, "Jones", 2, 5);
            Assert::IsTrue(found);

            found = BinarySearch(students, count, "Williams", 1, 3);
            Assert::IsFalse(found);
        }
    };
}
