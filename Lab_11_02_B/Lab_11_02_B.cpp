#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, TECHNICAL_EDUCATION };
string specialtyStr[] = { "Computer Science", "Informatics", "Math and Economics", "Physics and Informatics", "Technical Education" };

struct Student_B {
    char surname[50];
    int course;
    Specialty specialty;
    int physics_grade;
    int math_grade;
    union {
        int programming_grade;
        int numerical_methods_grade;
        int pedagogy_grade;
    };
};

void AddStudentToFile(const string& filename);
void PrintFromFile(const string& filename);
void CountPhysicsGradesFromFile(const string& filename);
int CountPhysicsAndMathGradesFromFile(const string& filename);


int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    int choice;
    do {
        cout << "\nMENU:" << endl;
        cout << "1. Add student" << endl;
        cout << "2. Print students" << endl;
        cout << "3. Count Physics grades" << endl;
        cout << "4. Count students with high Physics and Math grades" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            AddStudentToFile(filename);
            break;
        case 2:
            PrintFromFile(filename);
            break;
        case 3:
            CountPhysicsGradesFromFile(filename);
            break;
        case 4: {
            int count = CountPhysicsAndMathGradesFromFile(filename);
            cout << "Number of students with grades 4 or 5 in both Physics and Math: " << count << endl;
            break;
        }
        case 5:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}

void AddStudentToFile(const string& filename) {
    ofstream outFile(filename, ios::binary | ios::app);
    if (!outFile) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    Student_B student;
    int specialty;
    cin.ignore();
    cout << "Surname: ";
    string temp_surname;
    getline(cin, temp_surname);
    strncpy(student.surname, temp_surname.c_str(), sizeof(student.surname) - 1);
    student.surname[sizeof(student.surname) - 1] = '\0';

    cout << "Course: ";
    cin >> student.course;
    cout << "Specialty (0 - Computer Science, 1 - Informatics, 2 - Math and Economics, 3 - Physics and Informatics, 4 - Technical Education): ";
    cin >> specialty;
    student.specialty = static_cast<Specialty>(specialty);
    cout << "Physics grade: ";
    cin >> student.physics_grade;
    cout << "Math grade: ";
    cin >> student.math_grade;

    if (student.specialty == COMPUTER_SCIENCE) {
        cout << "Programming grade: ";
        cin >> student.programming_grade;
    }
    else if (student.specialty == INFORMATICS) {
        cout << "Numerical Methods grade: ";
        cin >> student.numerical_methods_grade;
    }
    else {
        cout << "Pedagogy grade: ";
        cin >> student.pedagogy_grade;
    }

    outFile.write(reinterpret_cast<const char*>(&student), sizeof(Student_B));
    outFile.close();
    cout << "Student added successfully." << endl;
}

void PrintFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading." << endl;
        return;
    }

    Student_B student;
    int i = 0;
    cout << "\n===========================================================================================================================" << endl;
    cout << "| #   | Surname         | Course | Specialty                | Physics | Math | Programming | Numerical Methods | Pedagogy |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student_B))) {
        cout << "| " << setw(3) << left << ++i << " ";
        cout << "| " << setw(15) << left << student.surname << " | "
            << setw(6) << left << student.course << " | "
            << setw(24) << left << specialtyStr[student.specialty] << " | "
            << setw(7) << left << student.physics_grade << " | "
            << setw(4) << left << student.math_grade << " | ";

        switch (student.specialty) {
        case COMPUTER_SCIENCE:
            cout << setw(11) << left << student.programming_grade << " | ";
            cout << setw(17) << left << " " << " | ";
            cout << setw(8) << left << " " << " | ";
            break;
        case INFORMATICS:
            cout << setw(11) << left << " " << " | ";
            cout << setw(17) << left << student.numerical_methods_grade << " | ";
            cout << setw(8) << left << " " << " | ";
            break;
        default:
            cout << setw(11) << left << " " << " | ";
            cout << setw(17) << left << " " << " | ";
            cout << setw(8) << left << student.pedagogy_grade << " | ";
            break;
        }
        cout << endl;
    }
    cout << "===========================================================================================================================" << endl;
    inFile.close();
}

void CountPhysicsGradesFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading." << endl;
        return;
    }

    int count_5 = 0, count_4 = 0, count_3 = 0;
    Student_B student;
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student_B))) {
        if (student.physics_grade == 5) count_5++;
        else if (student.physics_grade == 4) count_4++;
        else if (student.physics_grade == 3) count_3++;
    }
    inFile.close();

    cout << "Physics grade distribution: 5 - " << count_5 << ", 4 - " << count_4 << ", 3 - " << count_3 << endl;
}

int CountPhysicsAndMathGradesFromFile(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading." << endl;
        return 0;
    }

    int count = 0;
    Student_B student;
    while (inFile.read(reinterpret_cast<char*>(&student), sizeof(Student_B))) {
        if ((student.physics_grade >= 4) && (student.math_grade >= 4)) {
            count++;
        }
    }
    inFile.close();
    return count;
}