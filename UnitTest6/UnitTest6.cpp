#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "../Lab_11_04/Lab_11_04.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest6
{
    TEST_CLASS(UnitTest6)
    {
    public:

        TEST_METHOD(TestAddNoteToFile)
        {
            string filename = "testfile.bin";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Note note = { "Smith", "John", "1234567890", {25, 12, 1995} };
            fout.write(reinterpret_cast<char*>(&note), sizeof(Note));
            fout.close();

            ifstream fin(filename, ios::binary);
            Assert::IsTrue(fin.is_open(), L"File should be opened");

            Note readNote;
            fin.read(reinterpret_cast<char*>(&readNote), sizeof(Note));
            Assert::AreEqual(string("Smith"), string(readNote.surname));
            Assert::AreEqual(string("John"), string(readNote.name));
            Assert::AreEqual(string("1234567890"), string(readNote.phone));
            Assert::AreEqual(25, readNote.birthDate[0]);
            Assert::AreEqual(12, readNote.birthDate[1]);
            Assert::AreEqual(1995, readNote.birthDate[2]);
            fin.close();
        }

        TEST_METHOD(TestDisplayFile)
        {
            string filename = "testfile.bin";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Note note1 = { "Smith", "John", "1234567890", {25, 12, 1995} };
            Note note2 = { "Jones", "Jane", "0987654321", {5, 6, 1998} };
            fout.write(reinterpret_cast<char*>(&note1), sizeof(Note));
            fout.write(reinterpret_cast<char*>(&note2), sizeof(Note));
            fout.close();

            stringstream buffer;
            streambuf* old = cout.rdbuf(buffer.rdbuf());

            displayFile(filename);

            string output = buffer.str();
            Assert::IsTrue(output.find("Smith") != string::npos);
            Assert::IsTrue(output.find("Jones") != string::npos);

            cout.rdbuf(old);
        }

        TEST_METHOD(TestSortFileByBirthday)
        {
            string filename = "testfile.bin";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Note note1 = { "Smith", "John", "1234567890", {25, 12, 1995} };
            Note note2 = { "Jones", "Jane", "0987654321", {5, 6, 1998} };
            Note note3 = { "Taylor", "Chris", "1122334455", {12, 9, 1990} };
            fout.write(reinterpret_cast<char*>(&note1), sizeof(Note));
            fout.write(reinterpret_cast<char*>(&note2), sizeof(Note));
            fout.write(reinterpret_cast<char*>(&note3), sizeof(Note));
            fout.close();

            sortFileByBirthday(filename);

            ifstream fin(filename, ios::binary);
            Note readNote;
            fin.read(reinterpret_cast<char*>(&readNote), sizeof(Note));
            Assert::AreEqual(string("Taylor"), string(readNote.surname));

            fin.read(reinterpret_cast<char*>(&readNote), sizeof(Note));
            Assert::AreEqual(string("Smith"), string(readNote.surname));

            fin.read(reinterpret_cast<char*>(&readNote), sizeof(Note));
            Assert::AreEqual(string("Jones"), string(readNote.surname));

            fin.close();
        }

        TEST_METHOD(TestSearchByPhone)
        {
            string filename = "testfile.bin";
            remove(filename.c_str());

            ofstream fout(filename, ios::binary | ios::app);
            Note note1 = { "Smith", "John", "1234567890", {25, 12, 1995} };
            Note note2 = { "Jones", "Jane", "0987654321", {5, 6, 1998} };
            fout.write(reinterpret_cast<char*>(&note1), sizeof(Note));
            fout.write(reinterpret_cast<char*>(&note2), sizeof(Note));
            fout.close();

            string phone = "1234567890";
            searchByPhone(filename, phone);

            ifstream fin(filename, ios::binary);
            Note foundNote;
            bool found = false;
            while (fin.read(reinterpret_cast<char*>(&foundNote), sizeof(Note))) {
                if (foundNote.phone == phone) {
                    found = true;
                    break;
                }
            }
            Assert::IsTrue(found, L"Phone number not found");
            fin.close();
        }
    };
}
