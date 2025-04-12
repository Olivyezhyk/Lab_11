#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "../Lab_11_02_B/Lab_11_02_B.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest3
{
	TEST_CLASS(UnitTest3)
	{
	public:

		TEST_METHOD(TestPrintFromFile)
		{
			std::string filename = "test.dat";
			std::ofstream outFile(filename, std::ios::binary);
			Student_B student1 = { "Smith", 1, COMPUTER_SCIENCE, 5, 4, 3 };
			outFile.write(reinterpret_cast<const char*>(&student1), sizeof(Student_B));
			outFile.close();

			std::ostringstream oss;
			std::streambuf* old = std::cout.rdbuf(oss.rdbuf());

			PrintFromFile(filename);

			std::string output = oss.str();
			Assert::IsTrue(output.find("Smith") != std::string::npos, L"Student surname not found in the output.");
			Assert::IsTrue(output.find("1") != std::string::npos, L"Course not found in the output.");
			Assert::IsTrue(output.find("Computer Science") != std::string::npos, L"Specialty not found in the output.");
			std::cout.rdbuf(old);
		}

		TEST_METHOD(TestCountPhysicsGradesFromFile)
		{
			std::string filename = "test.dat";
			std::ofstream outFile(filename, std::ios::binary);
			Student_B student1 = { "Smith", 1, COMPUTER_SCIENCE, 5, 4, 3 };
			Student_B student2 = { "Jones", 2, INFORMATICS, 4, 3, 4 };
			outFile.write(reinterpret_cast<const char*>(&student1), sizeof(Student_B));
			outFile.write(reinterpret_cast<const char*>(&student2), sizeof(Student_B));
			outFile.close();

			std::ostringstream oss;
			std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
			CountPhysicsGradesFromFile(filename);
			std::string output = oss.str();
			std::cout.rdbuf(old);

			Assert::IsTrue(output.find("Physics grade distribution: 5 - 1, 4 - 1, 3 - 0") != std::string::npos, L"Incorrect grade distribution.");
		}

		TEST_METHOD(TestCountPhysicsAndMathGradesFromFile)
		{
			std::string filename = "test.dat";
			std::ofstream outFile(filename, std::ios::binary);
			Student_B student1 = { "Smith", 1, COMPUTER_SCIENCE, 5, 5, 3 };
			Student_B student2 = { "Jones", 2, INFORMATICS, 4, 4, 4 };
			Student_B student3 = { "Taylor", 3, MATH_ECONOMICS, 3, 3, 5 };
			outFile.write(reinterpret_cast<const char*>(&student1), sizeof(Student_B));
			outFile.write(reinterpret_cast<const char*>(&student2), sizeof(Student_B));
			outFile.write(reinterpret_cast<const char*>(&student3), sizeof(Student_B));
			outFile.close();

			int count = CountPhysicsAndMathGradesFromFile(filename);

			Assert::AreEqual(2, count, L"Incorrect count of students with high grades in both Physics and Math.");
		}

	};
}
