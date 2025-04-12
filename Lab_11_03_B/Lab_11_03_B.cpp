#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>

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

const int MAX_STUDENTS = 100;

void PrintStudents(Student_B students[], int count);
void SortStudents(Student_B students[], int count);
void CreateIndexFile(Student_B students[], int count);
bool BinarySearch(Student_B students[], int count, const string& surname, int course, int physics_grade);

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    Student_B students[MAX_STUDENTS];
    int studentCount = 0;

    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading." << endl;
        return 1;
    }

    while (inFile.read(reinterpret_cast<char*>(&students[studentCount]), sizeof(Student_B))) {
        if (studentCount >= MAX_STUDENTS) {
            cout << "Maximum student limit reached. No more students can be added." << endl;
            break;
        }
        studentCount++;
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

void SortStudents(Student_B students[], int count) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            const Student_B& a = students[j];
            const Student_B& b = students[j + 1];

            if (a.physics_grade < b.physics_grade ||
                (a.physics_grade == b.physics_grade && a.course > b.course) ||
                (a.physics_grade == b.physics_grade && a.course == b.course && strcmp(a.surname, b.surname) > 0)) {
                Student_B temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    PrintStudents(students, count);
    cout << "Students sorted.\n";
}

void PrintStudents(Student_B students[], int count) {
    cout << endl;
    cout << "===========================================================================================================================" << endl;
    cout << "| #   | Surname         | Course | Specialty                | Physics | Math | Programming | Numerical Methods | Pedagogy |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

    for (int i = 0; i < count; i++) {
        cout << "| " << setw(3) << left << i + 1 << " ";
        cout << "| " << setw(15) << left << students[i].surname << " | "
            << setw(6) << left << students[i].course << " | "
            << setw(24) << left << specialtyStr[students[i].specialty] << " | "
            << setw(7) << left << students[i].physics_grade << " | "
            << setw(4) << left << students[i].math_grade << " | ";

        switch (students[i].specialty) {
        case COMPUTER_SCIENCE:
            cout << setw(11) << left << students[i].programming_grade << " | ";
            cout << setw(17) << left << " " << " | ";
            cout << setw(8) << left << " " << " | ";
            break;
        case INFORMATICS:
            cout << setw(11) << left << " " << " | ";
            cout << setw(17) << left << students[i].numerical_methods_grade << " | ";
            cout << setw(8) << left << " " << " | ";
            break;
        default:
            cout << setw(11) << left << " " << " | ";
            cout << setw(17) << left << " " << " | ";
            cout << setw(8) << left << students[i].pedagogy_grade << " | ";
            break;
        }

        cout << endl;
    }
    cout << "===========================================================================================================================" << endl;
    cout << endl;
}

void CreateIndexFile(Student_B students[], int count) {
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
        << setw(15) << "Programming"
        << setw(17) << "Numerical Methods"
        << setw(10) << "Pedagogy" << endl;

    indexFile << string(113, '-') << endl;

    for (int i = 0; i < count; i++) {
        indexFile << left << setw(20) << students[i].surname
            << setw(8) << students[i].course
            << setw(25) << specialtyStr[students[i].specialty]
            << setw(10) << students[i].physics_grade
            << setw(10) << students[i].math_grade;

        switch (students[i].specialty) {
        case COMPUTER_SCIENCE:
            indexFile << setw(15) << students[i].programming_grade;
            indexFile << setw(17) << " " << " ";
            indexFile << setw(10) << " " << endl;
            break;
        case INFORMATICS:
            indexFile << setw(15) << " " << " ";
            indexFile << setw(17) << students[i].numerical_methods_grade;
            indexFile << setw(10) << " " << endl;
            break;
        default:
            indexFile << setw(15) << " " << " ";
            indexFile << setw(17) << " " << " ";
            indexFile << setw(10) << students[i].pedagogy_grade << endl;
            break;
        }
    }

    indexFile.close();
    cout << "Index file created!\n";
}


bool BinarySearch(Student_B students[], int count, const string& surname, int course, int physics_grade) {
    int left = 0, right = count - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        const Student_B& s = students[mid];

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
