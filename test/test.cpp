//#include <iostream>
//#include <fstream>
//using namespace std;
//
//enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, TECHNICAL_EDUCATION };
//
//struct Student_A {
//    char surname[50];
//    int course;
//    Specialty specialty;
//    int physics_grade;
//    int math_grade;
//    int informatics_grade;
//};
//
//void CreateBinaryFile(const string& filename) {
//    ofstream outFile(filename, ios::binary);
//    if (!outFile) {
//        cout << "Error opening file for writing." << endl;
//        return;
//    }
//
//    Student_A students[] = {
//        {"Ivanenko", 2, COMPUTER_SCIENCE, 5, 4, 4},
//        {"Petrenko", 3, INFORMATICS, 4, 3, 4},
//        {"Shevchenko", 1, MATH_ECONOMICS, 3, 5, 3}
//    };
//
//    for (const auto& student : students) {
//        outFile.write(reinterpret_cast<const char*>(&student), sizeof(Student_A));
//    }
//
//    outFile.close();
//    cout << "File '" << filename << "' has been created successfully!" << endl;
//}
//
//int main() {
//    CreateBinaryFile("text.dat");
//    return 0;
//}

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <fstream>
//#include <cstring> // for strcpy
//using namespace std;
//
//enum Specialty { COMPUTER_SCIENCE, INFORMATICS, MATH_ECONOMICS, PHYSICS_INFORMATICS, TECHNICAL_EDUCATION };
//
//struct Student_B {
//    char surname[50];  // Fixed-length surname for binary compatibility
//    int course;
//    Specialty specialty;
//    int physics_grade;
//    int math_grade;
//    union {
//        int programming_grade;
//        int numerical_methods_grade;
//        int pedagogy_grade;
//    };
//};
//
//void CreateBinaryFile(const string& filename) {
//    ofstream outFile(filename, ios::binary);
//    if (!outFile) {
//        cout << "Error opening file for writing." << endl;
//        return;
//    }
//
//    Student_B students[3];
//
//    // First student: Computer Science (programming grade)
//    strcpy_s(students[0].surname, sizeof(students[0].surname), "Ivanenko");
//    students[0].course = 2;
//    students[0].specialty = COMPUTER_SCIENCE;
//    students[0].physics_grade = 5;
//    students[0].math_grade = 4;
//    students[0].programming_grade = 4;
//
//    // Second student: Informatics (numerical methods grade)
//    strcpy(students[1].surname, "Petrenko");
//    students[1].course = 3;
//    students[1].specialty = INFORMATICS;
//    students[1].physics_grade = 4;
//    students[1].math_grade = 3;
//    students[1].numerical_methods_grade = 4;
//
//    // Third student: Math and Economics (pedagogy grade)
//    strcpy(students[2].surname, "Shevchenko");
//    students[2].course = 1;
//    students[2].specialty = MATH_ECONOMICS;
//    students[2].physics_grade = 3;
//    students[2].math_grade = 5;
//    students[2].pedagogy_grade = 3;
//
//    // Writing to binary file
//    for (const auto& student : students) {
//        outFile.write(reinterpret_cast<const char*>(&student), sizeof(Student_B));
//    }
//
//    outFile.close();
//    cout << "File '" << filename << "' has been created successfully!" << endl;
//}
//
//int main() {
//    CreateBinaryFile("text2.dat");
//    return 0;
//}

//#include <iostream>
//#include <fstream>
//#include <string>
//
//using namespace std;
//
//struct Note {
//    string surname;
//    string name;
//    string phone;
//    int birthDate[3];
//};
//
//// Функція для створення бінарного файлу
//void createBinaryFile(const string& filename) {
//    ofstream fout(filename, ios::binary);
//    if (!fout) {
//        cout << "Error opening file!" << endl;
//        return;
//    }
//
//    // Додаємо кілька записів у файл
//    Note notes[] = {
//        {"Ivanov", "Ivan", "123456789", {1, 1, 1990}},
//        {"Petrov", "Petr", "987654321", {15, 5, 1985}},
//        {"Sidorov", "Sidor", "555123456", {10, 12, 1992}}
//    };
//
//    // Записуємо всі 3 записи в бінарний файл
//    for (int i = 0; i < 3; i++) {
//        fout.write(reinterpret_cast<char*>(&notes[i]), sizeof(Note));
//    }
//
//    fout.close();
//    cout << "Binary file created successfully!" << endl;
//}
//
//int main() {
//    string filename;
//    cout << "Enter filename to create: ";
//    getline(cin, filename);
//
//    createBinaryFile(filename);
//    return 0;
//}



