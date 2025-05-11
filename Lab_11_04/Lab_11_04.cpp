#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct Note {
    char surname[30];
    char name[30];
    char phone[20];
    int birthDay;
    int birthMonth;
    int birthYear;
};

void addNoteToFile(const string& filename);
void displayFile(const string& filename);
void sortFileByBirthday(const string& filename);
void searchByPhone(const string& filename, const string& phone);
bool isValidPhone(const string& phone);
bool isValidDate(int day, int month, int year);
void clearInputBuffer();

int main() {
    string filename;
    cout << "Enter filename: ";
    getline(cin, filename);

    int menuItem;
    do {
        cout << "\nMenu:\n";
        cout << "1 - Add record\n";
        cout << "2 - Display records\n";
        cout << "3 - Sort by birthday\n";
        cout << "4 - Search by phone number\n";
        cout << "0 - Exit\n";
        cout << "Your choice: ";
        cin >> menuItem;
        clearInputBuffer();

        switch (menuItem) {
        case 1:
            addNoteToFile(filename);
            break;
        case 2:
            displayFile(filename);
            break;
        case 3:
            sortFileByBirthday(filename);
            break;
        case 4: {
            string phone;
            cout << "Enter phone number: ";
            cin >> phone;
            clearInputBuffer();
            searchByPhone(filename, phone);
            break;
        }
        case 0:
            break;
        default:
            cout << "Invalid option! Try again.\n";
        }
    } while (menuItem != 0);
    return 0;
}

void addNoteToFile(const string& filename) {
    ofstream fout(filename, ios::binary | ios::app);
    if (!fout) {
        cout << "Error opening file!\n";
        return;
    }

    Note note = {};
    string input;

    cout << "Enter surname: ";
    cin >> input;
    strncpy(note.surname, input.c_str(), sizeof(note.surname) - 1);

    cout << "Enter name: ";
    cin >> input;
    strncpy(note.name, input.c_str(), sizeof(note.name) - 1);

    cout << "Enter phone: ";
    cin >> input;
    while (!isValidPhone(input)) {
        cout << "Invalid phone number. Please enter again: ";
        cin >> input;
    }
    strncpy(note.phone, input.c_str(), sizeof(note.phone) - 1);

    cout << "Enter birthdate (day month year): ";
    int day, month, year;
    cin >> day >> month >> year;
    while (!isValidDate(day, month, year)) {
        cout << "Invalid date. Please enter again (day month year): ";
        cin >> day >> month >> year;
    }
    note.birthDay = day;
    note.birthMonth = month;
    note.birthYear = year;

    fout.write(reinterpret_cast<const char*>(&note), sizeof(Note));
    fout.close();
    cout << "Record added.\n";
}

void displayFile(const string& filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Error opening file!\n";
        return;
    }

    Note note;
    cout << left << setw(15) << "Surname" << setw(15) << "Name"
        << setw(15) << "Phone" << "Birthdate\n";

    while (fin.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        cout << left << setw(15) << note.surname
            << setw(15) << note.name
            << setw(15) << note.phone
            << note.birthDay << '.' << note.birthMonth << '.' << note.birthYear << '\n';
    }

    fin.close();
}

void sortFileByBirthday(const string& filename) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Error opening file!\n";
        return;
    }

    file.seekg(0, ios::end);
    size_t count = file.tellg() / sizeof(Note);

    for (size_t i = 0; i < count - 1; ++i) {
        for (size_t j = 0; j < count - i - 1; ++j) {
            Note a, b;

            file.seekg(j * sizeof(Note));
            file.read(reinterpret_cast<char*>(&a), sizeof(Note));
            file.read(reinterpret_cast<char*>(&b), sizeof(Note));

            bool shouldSwap =
                a.birthYear > b.birthYear ||
                (a.birthYear == b.birthYear && a.birthMonth > b.birthMonth) ||
                (a.birthYear == b.birthYear && a.birthMonth == b.birthMonth && a.birthDay > b.birthDay);

            if (shouldSwap) {
                file.seekp(j * sizeof(Note));
                file.write(reinterpret_cast<const char*>(&b), sizeof(Note));
                file.write(reinterpret_cast<const char*>(&a), sizeof(Note));
            }
        }
    }

    file.close();
    cout << "Records sorted by birthday.\n";
}

void searchByPhone(const string& filename, const string& phone) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Error opening file!\n";
        return;
    }

    Note note;
    bool found = false;

    while (fin.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        string storedPhone(note.phone);
        storedPhone.erase(storedPhone.find_last_not_of(' ') + 1);

        if (storedPhone == phone) {
            cout << "Found: " << note.surname << " " << note.name << " "
                << note.birthDay << '.' << note.birthMonth << '.' << note.birthYear << '\n';
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Record not found.\n";
    }

    fin.close();
}

bool isValidPhone(const string& phone) {
    if (phone.length() != 10) return false;
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidDate(int day, int month, int year) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool leapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if ((leapYear && day > 29) || (!leapYear && day > 28)) return false;
    }
    return true;
}

void clearInputBuffer() {
    cin.ignore(10000, '\n');
}
