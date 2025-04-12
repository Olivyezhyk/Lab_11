#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>

using namespace std;

struct Note {
    char surname[30] = "";
    char name[30] = "";
    char phone[20] = "";
    int birthDate[3] = { 0 };
};

const int MAX_RECORDS = 100;

void addNoteToFile(const string& filename);
void displayFile(const string& filename);
void sortFileByBirthday(const string& filename);
void searchByPhone(const string& filename, const string& phone);

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
        cin.ignore();

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

    Note note;
    string temp;

    cout << "Enter surname: ";
    cin >> temp;
    strncpy_s(note.surname, sizeof(note.surname), temp.c_str(), _TRUNCATE);

    cout << "Enter name: ";
    cin >> temp;
    strncpy_s(note.name, sizeof(note.name), temp.c_str(), _TRUNCATE);

    cout << "Enter phone: ";
    cin >> temp;
    strncpy_s(note.phone, sizeof(note.phone), temp.c_str(), _TRUNCATE);

    cout << "Enter birthdate (day month year): ";
    cin >> note.birthDate[0] >> note.birthDate[1] >> note.birthDate[2];

    fout.write(reinterpret_cast<char*>(&note), sizeof(Note));
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
    cout << left << setw(15) << "Surname" << setw(15) << "Name" << setw(15) << "Phone" << "Birthdate\n";

    while (fin.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        cout << left << setw(15) << note.surname
            << setw(15) << note.name
            << setw(15) << note.phone
            << note.birthDate[0] << '.' << note.birthDate[1] << '.' << note.birthDate[2] << '\n';
    }
    fin.close();
}

void sortFileByBirthday(const string& filename) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Error opening file!\n";
        return;
    }

    Note notes[MAX_RECORDS];
    int size = 0;

    while (size < MAX_RECORDS && fin.read(reinterpret_cast<char*>(&notes[size]), sizeof(Note))) {
        size++;
    }
    fin.close();

    if (size == 0) {
        cout << "No records to sort!\n";
        return;
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            Note& a = notes[j];
            Note& b = notes[j + 1];
            if ((b.birthDate[2] < a.birthDate[2]) ||
                (b.birthDate[2] == a.birthDate[2] && b.birthDate[1] < a.birthDate[1]) ||
                (b.birthDate[2] == a.birthDate[2] && b.birthDate[1] == a.birthDate[1] && b.birthDate[0] < a.birthDate[0])) {
                swap(a, b);
            }
        }
    }

    ofstream fout(filename, ios::binary | ios::trunc);
    if (!fout) {
        cout << "Error writing to file!\n";
        return;
    }

    for (int i = 0; i < size; i++) {
        fout.write(reinterpret_cast<char*>(&notes[i]), sizeof(Note));
    }
    fout.close();

    cout << "Records sorted by birthday.\n";
}

void searchByPhone(const string& filename, const string& phone) {
    ifstream fin(filename, ios::binary);
    if (!fin) {
        cout << "Error opening file!\n";
        return;
    }

    Note note;
    while (fin.read(reinterpret_cast<char*>(&note), sizeof(Note))) {
        if (phone == note.phone) {
            cout << "Found: " << note.surname << " " << note.name << " "
                << note.birthDate[0] << '.' << note.birthDate[1] << '.' << note.birthDate[2] << '\n';
            fin.close();
            return;
        }
    }
    cout << "Record not found.\n";
    fin.close();
}
