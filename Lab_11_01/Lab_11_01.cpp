#include <iostream>
#include <fstream>
using namespace std;

void CreateBIN(const char* fname) {
    ofstream fout(fname, ios::binary);
    if (!fout) {
        cerr << "Error creating file!" << endl;
        return;
    }
    int num;
    char ch;
    do {
        cout << "Enter an integer: ";
        while (!(cin >> num)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter an integer: ";
        }
        fout.write(reinterpret_cast<char*>(&num), sizeof(num));
        cout << "Continue? (y/n): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fout.close();
}

void PrintBIN(const char* fname, bool isOutputFile = false) {
    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "Error opening file!" << endl;
        return;
    }
    int num;
    if (isOutputFile) {
        int values[2];
        if (fin.read(reinterpret_cast<char*>(values), sizeof(values))) {
            cout << "Max odd: " << values[0] << ", Min even: " << values[1] << endl;
        }
        else {
            cerr << "Error reading output file!" << endl;
        }
    }
    else {
        while (fin.read(reinterpret_cast<char*>(&num), sizeof(num))) {
            cout << num << " ";
        }
        cout << endl;
    }
    fin.close();
}

bool FindMaxOddMinEven(const char* fname, int& maxOdd, int& minEven) {
    ifstream fin(fname, ios::binary);
    if (!fin) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    maxOdd = numeric_limits<int>::min();
    minEven = numeric_limits<int>::max();
    bool foundOdd = false, foundEven = false;
    int num;

    while (fin.read(reinterpret_cast<char*>(&num), sizeof(num))) {
        if (num % 2 == 0) {
            if (!foundEven || num < minEven) {
                minEven = num;
                foundEven = true;
            }
        }
        else {
            if (!foundOdd || num > maxOdd) {
                maxOdd = num;
                foundOdd = true;
            }
        }
    }
    fin.close();
    return foundOdd || foundEven;
}

void WriteResultToFile(const char* gname, int maxOdd, int minEven) {
    ofstream fout(gname, ios::binary);
    if (!fout) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    fout.write(reinterpret_cast<char*>(&maxOdd), sizeof(maxOdd));
    fout.write(reinterpret_cast<char*>(&minEven), sizeof(minEven));
    fout.close();
}

int main() {
    char fname[100], gname[100];

    cout << "Enter input file name: ";
    cin >> fname;
    CreateBIN(fname);

    cout << "Original file contents: ";
    PrintBIN(fname);

    int maxOdd, minEven;
    if (FindMaxOddMinEven(fname, maxOdd, minEven)) {
        cout << "Enter output file name: ";
        cin >> gname;
        WriteResultToFile(gname, maxOdd, minEven);
        cout << "Processed file contents: ";
        PrintBIN(gname, true);
    }
    else {
        cout << "No valid numbers found in the file." << endl;
    }

    return 0;
}