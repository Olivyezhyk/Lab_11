#include "pch.h"
#include "CppUnitTest.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include "../Lab_11_03_B/Lab_11_03_B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest5
{
    TEST_CLASS(UnitTest5)
    {
    public:

        TEST_METHOD(TestSortStudents)
        {
            Student_B students[3] = {
                {"Smith", 3, COMPUTER_SCIENCE, 4, 5, 5},
                {"Jones", 2, INFORMATICS, 5, 4, 5},
                {"Taylor", 1, PHYSICS_INFORMATICS, 3, 5, 4}
            };

            int studentCount = 3;
            SortStudents(students, studentCount);

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
            Student_B students[3] = {
                {"Smith", 3, COMPUTER_SCIENCE, 4, 5, 5},
                {"Jones", 2, INFORMATICS, 5, 4, 5},
                {"Taylor", 1, PHYSICS_INFORMATICS, 3, 5, 4}
            };

            int studentCount = 3;
            bool found = BinarySearch(students, studentCount, "Jones", 2, 5);
            Assert::IsTrue(found);

            found = BinarySearch(students, studentCount, "Williams", 1, 3);
            Assert::IsFalse(found);
        }

        TEST_METHOD(TestPrintStudents)
        {
            Student_B students[3] = {
                {"Smith", 3, COMPUTER_SCIENCE, 4, 5, 5},
                {"Jones", 2, INFORMATICS, 5, 4, 5},
                {"Taylor", 1, PHYSICS_INFORMATICS, 3, 5, 4}
            };

            stringstream buffer;
            streambuf* old = cout.rdbuf(buffer.rdbuf());

            PrintStudents(students, 3);

            string output = buffer.str();
            Assert::IsTrue(output.find("Smith") != string::npos);
            Assert::IsTrue(output.find("Jones") != string::npos);
            Assert::IsTrue(output.find("Taylor") != string::npos);

            cout.rdbuf(old);
        }
    };
}
