#include "BankSystem.h"
#include "FileStorage.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

int main() {
    BankSystem system;

    try {
        FileStorage::load(system);
    } catch (const exception& e) {
        cout << "Could not load saved data: " << e.what() << '\n';
        cout << "Original data has NOT been overwritten.\n";
        return 1;  
    }

    system.run();

    try {
        system.save();
        cout << "Data saved automatically.\n";
    } catch (const exception& e) {
        cout << "Could not save data: " << e.what() << '\n';
        return 1;
    }
    return 0;
}