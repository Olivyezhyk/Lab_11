#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Car {
    char brand[20];
    char type;
    char licensePlate[10];
    int serviceLife;
    union {
        char color[10];
        double loadCapacity;
    } details;
};

void addCar(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Failed to open the file!" << endl;
        return;
    }

    Car car;
    cout << "Brand: ";
    cin >> car.brand;
    cout << "Type (L - sedan, V - truck): ";
    cin >> car.type;
    cout << "License Plate: ";
    cin >> car.licensePlate;
    cout << "Service Life (years): ";
    cin >> car.serviceLife;

    if (car.type == 'L') {
        cout << "Color: ";
        cin >> car.details.color;
    }
    else if (car.type == 'V') {
        cout << "Load Capacity (tons): ";
        cin >> car.details.loadCapacity;
    }
    else {
        cerr << "Unknown car type!" << endl;
        return;
    }

    file.write(reinterpret_cast<char*>(&car), sizeof(Car));
    file.close();
}

void displayCars(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open the file!" << endl;
        return;
    }

    Car car;
    while (file.read(reinterpret_cast<char*>(&car), sizeof(Car))) {
        cout << "Brand: " << car.brand
            << ", License Plate: " << car.licensePlate
            << ", Service Life: " << car.serviceLife
            << " years, Type: " << (car.type == 'L' ? "Sedan" : "Truck");
        if (car.type == 'L') {
            cout << ", Color: " << car.details.color;
        }
        else {
            cout << ", Load Capacity: " << fixed << setprecision(2) << car.details.loadCapacity << " tons";
        }
        cout << endl;
    }

    file.close();
}

void findHeavyTrucks(const string& filename, double minCapacity) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open the file!" << endl;
        return;
    }

    Car car;
    while (file.read(reinterpret_cast<char*>(&car), sizeof(Car))) {
        if (car.type == 'V' && car.details.loadCapacity >= minCapacity) {
            cout << "License Plate: " << car.licensePlate
                << ", Load Capacity: " << car.details.loadCapacity << " tons" << endl;
        }
    }

    file.close();
}

void findCarsByColor(const string& filename, const string& color) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open the file!" << endl;
        return;
    }

    Car car;
    while (file.read(reinterpret_cast<char*>(&car), sizeof(Car))) {
        if (car.type == 'L' && color == car.details.color) {
            cout << "Brand: " << car.brand
                << ", Color: " << car.details.color << endl;
        }
    }

    file.close();
}

void removeOldCars(const string& filename) {
    ifstream file(filename, ios::binary);
    ofstream temp("temp.dat", ios::binary);

    if (!file || !temp) {
        cerr << "Error opening files!" << endl;
        return;
    }

    Car car;
    while (file.read(reinterpret_cast<char*>(&car), sizeof(Car))) {
        if (car.serviceLife <= 5) {
            temp.write(reinterpret_cast<char*>(&car), sizeof(Car));
        }
    }

    file.close();
    temp.close();

    remove(filename.c_str());
    rename("temp.dat", filename.c_str());

    cout << "Cars with service life over 5 years removed.\n";
}

void showMenu() {
    cout << "\n===== MENU =====\n"
        << "1. Add a car\n"
        << "2. Display all cars\n"
        << "3. Remove cars with service life > 5 years\n"
        << "4. Find trucks with minimum load capacity\n"
        << "5. Find sedans by color\n"
        << "6. Exit\n"
        << "Your choice: ";
}

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            addCar(filename);
            break;
        case 2:
            displayCars(filename);
            break;
        case 3:
            removeOldCars(filename);
            break;
        case 4: {
            double minCap;
            cout << "Enter the minimum load capacity: ";
            cin >> minCap;
            findHeavyTrucks(filename, minCap);
            break;
        }
        case 5: {
            string color;
            cout << "Enter color: ";
            cin >> color;
            findCarsByColor(filename, color);
            break;
        }
        case 6:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 7);

    return 0;
}
