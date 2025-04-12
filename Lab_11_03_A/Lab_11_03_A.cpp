#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
using namespace std;

enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, TECHNICAL_EDUCATION };
string specialtyStr[] = { "Computer Science", "Informatics", "Math and Economics", "Physics and Informatics", "Technical Education" };

struct Student_A {
    char surname[50];
    int course;
    Specialty specialty;
    int physics_grade;
    int math_grade;
    int informatics_grade;
};

const int MAX_STUDENTS = 100;

void PrintStudents(Student_A students[], int count);
void SortStudents(Student_A students[], int count);
void CreateIndexFile(Student_A students[], int count);
bool BinarySearch(Student_A students[], int count, const string& surname, int course, int physics_grade);

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    Student_A students[MAX_STUDENTS];
    int studentCount = 0;

    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading." << endl;
        return 1;
    }

    while (inFile.read(reinterpret_cast<char*>(&students[studentCount]), sizeof(Student_A))) {
        studentCount++;
        if (studentCount >= MAX_STUDENTS) break;
    }
    inFile.close();

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Display students\n";
        cout << "2. Sort students (physical sort)\n";
        cout << "3. Create index file\n";
        cout << "4. Search student (binary search)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            PrintStudents(students, studentCount);
            break;
        case 2:
            SortStudents(students, studentCount);
            break;
        case 3:
            CreateIndexFile(students, studentCount);
            break;
        case 4: {
            string surname;
            int course, physics_grade;
            cout << "Enter surname: "; cin >> surname;
            cout << "Enter course: "; cin >> course;
            cout << "Enter physics grade: "; cin >> physics_grade;
            if (BinarySearch(students, studentCount, surname, course, physics_grade)) {
                cout << "Student found!\n";
            }
            else {
                cout << "Student not found.\n";
            }
            break;
        }
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}

void SortStudents(Student_A students[], int count) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            const Student_A& a = students[j];
            const Student_A& b = students[j + 1];

            if (a.physics_grade < b.physics_grade ||
                (a.physics_grade == b.physics_grade && a.course > b.course) ||
                (a.physics_grade == b.physics_grade && a.course == b.course && strcmp(a.surname, b.surname) > 0)) {
                Student_A temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    PrintStudents(students, count);
    cout << "Students sorted.\n";
}

void PrintStudents(Student_A students[], int count) {
    cout << "\n===========================================================================================\n";
    cout << "| #   | Surname         | Course | Specialty               | Physics | Math | Informatics |\n";
    cout << "-------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < count; i++) {
        cout << "| " << setw(3) << left << i + 1 << " ";
        cout << "| " << setw(15) << left << students[i].surname << " | "
            << setw(6) << left << students[i].course << " | "
            << setw(23) << left << specialtyStr[students[i].specialty] << " | "
            << setw(7) << left << students[i].physics_grade << " | "
            << setw(4) << left << students[i].math_grade << " | "
            << setw(11) << left << students[i].informatics_grade << " |\n";
    }
    cout << "===========================================================================================\n\n";
}

void CreateIndexFile(Student_A students[], int count) {
    ofstream indexFile("index.dat");
    if (!indexFile.is_open()) {
        cout << "Error opening index file!\n";
        return;
    }

    indexFile << left << setw(20) << "Surname"
        << setw(8) << "Course"
        << setw(25) << "Specialty"
        << setw(10) << "Physics"
        << setw(10) << "Math"
        << setw(15) << "Informatics" << endl;

    indexFile << string(88, '-') << endl;

    for (int i = 0; i < count; i++) {
        indexFile << left << setw(20) << students[i].surname
            << setw(8) << students[i].course
            << setw(25) << specialtyStr[students[i].specialty]
            << setw(10) << students[i].physics_grade
            << setw(10) << students[i].math_grade
            << setw(15) << students[i].informatics_grade << endl;
    }

    indexFile.close();
    cout << "Index file created!\n";
}

bool BinarySearch(Student_A students[], int count, const string& surname, int course, int physics_grade) {
    int left = 0, right = count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        const Student_A& s = students[mid];

        if (s.physics_grade == physics_grade && s.course == course && surname == s.surname)
            return true;

        if (s.physics_grade > physics_grade ||
            (s.physics_grade == physics_grade && s.course < course) ||
            (s.physics_grade == physics_grade && s.course == course && strcmp(s.surname, surname.c_str()) < 0)) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return false;
}

