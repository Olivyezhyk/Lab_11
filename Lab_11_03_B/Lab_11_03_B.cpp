#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>

using namespace std;

enum Specialty {
    COMPUTER_SCIENCE,
    INFORMATICS,
    MATH_ECONOMICS,
    PHYSICS_INFORMATICS,
    TECHNICAL_EDUCATION
};

string specialtyStr[] = {
    "Computer Science", "Informatics", "Math and Economics", "Physics and Informatics", "Technical Education"
};

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

void PrintFile(const string& filename);
void SortFile(const string& filename);
void CreateIndexFile(const string& filename, const string& indexFile);
bool BinarySearchInFile(const string& filename, const string& surname, int course, int physics_grade);
int GetFileRecordCount(const string& filename);

int main() {
    string filename;
    cout << "Enter the binary filename: ";
    cin >> filename;

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Display students\n";
        cout << "2. Sort students (physical sort)\n";
        cout << "3. Create index file\n";
        cout << "4. Search student (binary search)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again: ";
        }

        switch (choice) {
            case 1:
                PrintFile(filename);
                break;
            case 2:
                SortFile(filename);
                break;
            case 3:
                CreateIndexFile(filename, "index.dat");
                break;
            case 4: {
                string surname;
                int course, physics;
                cout << "Enter surname: "; cin >> surname;
                cout << "Enter course: "; cin >> course;
                cout << "Enter physics grade: "; cin >> physics;

                if (BinarySearchInFile(filename, surname, course, physics))
                    cout << "Student found!\n";
                else
                    cout << "Student not found.\n";
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
        }

    } while (choice != 5);

    return 0;
}

void PrintFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }

    Student_B student;
    int index = 1;

    cout << "===========================================================================================================================" << endl;
    cout << "| #   | Surname         | Course | Specialty                | Physics | Math | Programming | Numerical Methods | Pedagogy |" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student_B))) {
        cout << "| " << left << setw(3) << index++ << " " << left;
        cout << "| " << setw(15) << left << student.surname << " | "
             << setw(6) << student.course << " | "
             << setw(24) << specialtyStr[student.specialty] << " | "
             << setw(7) << student.physics_grade << " | "
             << setw(4) << student.math_grade << " | ";

        switch (student.specialty) {
            case COMPUTER_SCIENCE:
                cout << setw(11) << student.programming_grade << " | ";
                cout << setw(17) << " " << " | ";
                cout << setw(8) << " " << " | ";
                break;
            case INFORMATICS:
                cout << setw(11) << " " << " | ";
                cout << setw(17) << student.numerical_methods_grade << " | ";
                cout << setw(8) << " " << " | ";
                break;
            default:
                cout << setw(11) << " " << " | ";
                cout << setw(17) << " " << " | ";
                cout << setw(8) << student.pedagogy_grade << " | ";
                break;
        }
        cout << endl;
    }

    cout << "===========================================================================================================================" << endl;
    file.close();
}

void SortFile(const string& filename) {
    string tempFile = "temp_sort.dat";

    int totalRecords = GetFileRecordCount(filename);
    if (totalRecords <= 1) {
        cout << "Nothing to sort.\n";
        return;
    }

    for (int i = 0; i < totalRecords - 1; ++i) {
        ifstream inFile(filename, ios::binary);
        ofstream outFile(tempFile, ios::binary);
        if (!inFile || !outFile) {
            cout << "Error opening file during sorting.\n";
            return;
        }

        Student_B prev, curr;
        bool swapped = false;

        inFile.read(reinterpret_cast<char*>(&prev), sizeof(Student_B));

        while (inFile.read(reinterpret_cast<char*>(&curr), sizeof(Student_B))) {
            bool shouldSwap = false;

            if (prev.physics_grade < curr.physics_grade ||
                (prev.physics_grade == curr.physics_grade && prev.course > curr.course) ||
                (prev.physics_grade == curr.physics_grade && prev.course == curr.course &&
                    strcmp(prev.surname, curr.surname) > 0)) {
                shouldSwap = true;
            }

            if (shouldSwap) {
                outFile.write(reinterpret_cast<char*>(&curr), sizeof(Student_B));
                swapped = true;
                prev = prev;
            }
            else {
                outFile.write(reinterpret_cast<char*>(&prev), sizeof(Student_B));
                prev = curr;
            }
        }

        outFile.write(reinterpret_cast<char*>(&prev), sizeof(Student_B));

        inFile.close();
        outFile.close();

        remove(filename.c_str());
        rename(tempFile.c_str(), filename.c_str());

        if (!swapped) break;
    }

    cout << "File sorted successfully (without array logic).\n";
}


void CreateIndexFile(const string& filename, const string& indexFile) {
    ifstream file(filename, ios::binary);
    ofstream out(indexFile);
    if (!file || !out) {
        cout << "Error with index file creation.\n";
        return;
    }

    Student_B student;

    out << left << setw(20) << "Surname"
        << setw(8) << "Course"
        << setw(25) << "Specialty"
        << setw(10) << "Physics"
        << setw(10) << "Math"
        << setw(15) << "Programming"
        << setw(17) << "Numerical Methods"
        << setw(10) << "Pedagogy" << endl;

    out << string(113, '-') << endl;

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student_B))) {
        out << left << setw(20) << student.surname
            << setw(8) << student.course
            << setw(25) << specialtyStr[student.specialty]
            << setw(10) << student.physics_grade
            << setw(10) << student.math_grade;

        switch (student.specialty) {
            case COMPUTER_SCIENCE:
                out << setw(15) << student.programming_grade;
                out << setw(17) << " ";
                out << setw(10) << " " << endl;
                break;
            case INFORMATICS:
                out << setw(15) << " ";
                out << setw(17) << student.numerical_methods_grade;
                out << setw(10) << " " << endl;
                break;
            default:
                out << setw(15) << " ";
                out << setw(17) << " ";
                out << setw(10) << student.pedagogy_grade << endl;
                break;
        }
    }

    file.close();
    out.close();
    cout << "Index file created.\n";
}

bool BinarySearchInFile(const string& filename, const string& surname, int course, int physics_grade) {
    int left = 0, right = GetFileRecordCount(filename) - 1;
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Cannot open file.\n";
        return false;
    }

    while (left <= right) {
        int mid = (left + right) / 2;
        file.seekg(mid * sizeof(Student_B));
        Student_B student;
        file.read(reinterpret_cast<char*>(&student), sizeof(Student_B));

        int cmp = strcmp(student.surname, surname.c_str());

        if (student.physics_grade == physics_grade &&
            student.course == course &&
            cmp == 0) {
            return true;
        }

        if (student.physics_grade > physics_grade ||
            (student.physics_grade == physics_grade && student.course < course) ||
            (student.physics_grade == physics_grade && student.course == course && cmp < 0)) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return false;
}

int GetFileRecordCount(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file) return 0;
    streamsize size = file.tellg();
    return size / sizeof(Student_B);
}
