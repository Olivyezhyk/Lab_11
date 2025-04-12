#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "../Lab_11_05/Lab_11_05.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest7
{
    TEST_CLASS(UnitTest7)
    {
    public:

        TEST_METHOD(TestAddCar)
        {
            string filename = "testfile.dat";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Car car = { "Toyota", 'L', "ABC1234", 4, {"Red"} };
            fout.write(reinterpret_cast<char*>(&car), sizeof(Car));
            fout.close();

            ifstream fin(filename, ios::binary);
            Assert::IsTrue(fin.is_open(), L"File should be opened");

            Car readCar;
            fin.read(reinterpret_cast<char*>(&readCar), sizeof(Car));
            Assert::AreEqual(string("Toyota"), string(readCar.brand));
            Assert::AreEqual('L', readCar.type);
            Assert::AreEqual(string("ABC1234"), string(readCar.licensePlate));
            Assert::AreEqual(4, readCar.serviceLife);
            Assert::AreEqual(string("Red"), string(readCar.details.color));
            fin.close();
        }

        TEST_METHOD(TestDisplayCars)
        {
            string filename = "testfile.dat";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Car car1 = { "Toyota", 'L', "ABC1234", 4, {"Red"} };
            Car car2 = { "Volvo", 'V', "XYZ5678", 7, {0, 10.5} };
            fout.write(reinterpret_cast<char*>(&car1), sizeof(Car));
            fout.write(reinterpret_cast<char*>(&car2), sizeof(Car));
            fout.close();

            stringstream buffer;
            streambuf* old = cout.rdbuf(buffer.rdbuf());

            displayCars(filename);

            string output = buffer.str();
            Assert::IsTrue(output.find("Toyota") != string::npos);
            Assert::IsTrue(output.find("Volvo") != string::npos);

            cout.rdbuf(old);
        }

        TEST_METHOD(TestFindCarsByColor)
        {
            string filename = "testfile.dat";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Car car1 = { "Toyota", 'L', "ABC1234", 4, {"Red"} };
            Car car2 = { "Volvo", 'V', "XYZ5678", 7, {0, 10.5} };
            fout.write(reinterpret_cast<char*>(&car1), sizeof(Car));
            fout.write(reinterpret_cast<char*>(&car2), sizeof(Car));
            fout.close();

            stringstream buffer;
            streambuf* old = cout.rdbuf(buffer.rdbuf());

            findCarsByColor(filename, "Red");

            string output = buffer.str();
            Assert::IsTrue(output.find("Toyota") != string::npos);

            cout.rdbuf(old);
        }
    };
}
