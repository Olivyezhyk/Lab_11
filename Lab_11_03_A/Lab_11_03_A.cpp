#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
using namespace std;

enum Specialty {
    COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS,
    PHYSICS_INFORMATICS, TECHNICAL_EDUCATION
};

const string specialtyStr[] = {
    "Computer Science", "Informatics", "Math and Economics",
    "Physics and Informatics", "Technical Education"
};

struct Student {
    char surname[50];
    int course;
    Specialty specialty;
    int physics_grade;
    int math_grade;
    int informatics_grade;
};

string GetFilenameFromUser() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    return filename;
}

int GetMenuChoice() {
    int choice;
    cout << "\nMenu:\n"
        << "1. Display students\n"
        << "2. Sort students in file\n"
        << "3. Create index file\n"
        << "4. Search student in file\n"
        << "5. Exit\n"
        << "Enter your choice: ";
    while (!(cin >> choice) || choice < 1 || choice > 5) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Try again: ";
    }
    return choice;
}

void DisplayStudents(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Cannot open file.\n";
        return;
    }

    Student s;
    int index = 1;

    cout << "\n===========================================================================================\n";
    cout << "| #   | Surname         | Course | Specialty               | Physics | Math | Informatics |\n";
    cout << "-------------------------------------------------------------------------------------------\n";

    while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        cout << "| " << setw(3) << left << index++ << " "
            << "| " << setw(15) << left << s.surname << " | "
            << setw(6) << left << s.course << " | "
            << setw(23) << left << specialtyStr[s.specialty] << " | "
            << setw(7) << left << s.physics_grade << " | "
            << setw(4) << left << s.math_grade << " | "
            << setw(11) << left << s.informatics_grade << " |\n";
    }

    cout << "===========================================================================================\n\n";
    file.close();
}

void SortStudentsInFile(const string& filename) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file) {
        cout << "Cannot open file for sorting.\n";
        return;
    }

    bool swapped;
    do {
        swapped = false;
        file.clear();
        file.seekg(0, ios::beg);

        Student s1, s2;
        streampos pos1 = file.tellg();

        while (file.read(reinterpret_cast<char*>(&s1), sizeof(Student))) {
            streampos pos2 = file.tellg();
            if (!file.read(reinterpret_cast<char*>(&s2), sizeof(Student))) break;

            bool shouldSwap = (s1.physics_grade < s2.physics_grade) ||
                (s1.physics_grade == s2.physics_grade && s1.course > s2.course) ||
                (s1.physics_grade == s2.physics_grade && s1.course == s2.course && strcmp(s1.surname, s2.surname) > 0);

            if (shouldSwap) {
                file.seekp(pos1);
                file.write(reinterpret_cast<char*>(&s2), sizeof(Student));
                file.write(reinterpret_cast<char*>(&s1), sizeof(Student));
                swapped = true;
            }
            else {
                pos1 = pos2;
                file.seekg(pos2);
            }
        }
    } while (swapped);

    file.close();
    cout << "Students sorted in file.\n";
}


void CreateIndexFileFromData(const string& dataFilename, const string& indexFilename) {
    ifstream dataFile(dataFilename, ios::binary);
    ofstream indexFile(indexFilename);
    if (!dataFile || !indexFile) {
        cout << "Error opening file(s).\n";
        return;
    }

    indexFile << left << setw(20) << "Surname"
        << setw(8) << "Course"
        << setw(25) << "Specialty"
        << setw(10) << "Physics"
        << setw(10) << "Math"
        << setw(15) << "Informatics" << endl;

    indexFile << string(88, '-') << endl;

    Student s;
    while (dataFile.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
        indexFile << left << setw(20) << s.surname
            << setw(8) << s.course
            << setw(25) << specialtyStr[s.specialty]
            << setw(10) << s.physics_grade
            << setw(10) << s.math_grade
            << setw(15) << s.informatics_grade << endl;
    }

    cout << "Index file created: " << indexFilename << endl;
    dataFile.close();
    indexFile.close();
}

bool FindStudentInFile(const string& filename, const string& surname, int course, int physics_grade) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cout << "Cannot open file.\n";
        return false;
    }

    file.seekg(0, ios::end);
    int count = file.tellg() / sizeof(Student);
    int left = 0, right = count - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        Student s;

        file.seekg(mid * sizeof(Student));
        file.read(reinterpret_cast<char*>(&s), sizeof(Student));

        int cmp = strcmp(s.surname, surname.c_str());
        if (s.physics_grade == physics_grade && s.course == course && cmp == 0) {
            return true;
        }

        if (s.physics_grade > physics_grade ||
            (s.physics_grade == physics_grade && s.course < course) ||
            (s.physics_grade == physics_grade && s.course == course && cmp < 0)) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }

    return false;
}

void PromptAndSearchStudent(const string& filename) {
    string surname;
    int course, physics_grade;

    cout << "Enter surname: ";
    cin >> surname;

    cout << "Enter course: ";
    while (!(cin >> course)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Enter course again: ";
    }

    cout << "Enter physics grade: ";
    while (!(cin >> physics_grade)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Enter physics grade again: ";
    }

    bool found = FindStudentInFile(filename, surname, course, physics_grade);
    cout << (found ? "Student found!\n" : "Student not found.\n");
}

int main() {
    string filename = GetFilenameFromUser();

    int choice;
    do {
        choice = GetMenuChoice();
        switch (choice) {
        case 1:
            DisplayStudents(filename);
            break;
        case 2:
            SortStudentsInFile(filename);
            break;
        case 3:
            CreateIndexFileFromData(filename, "index.dat");
            break;
        case 4:
            PromptAndSearchStudent(filename);
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        }
    } while (choice != 5);

    return 0;
}
