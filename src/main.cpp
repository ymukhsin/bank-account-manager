#include "Bank.h"
#include "Exceptions.h"

#include <iostream>

using namespace std;

int main() {
    Bank bank("deu", "01000000000",
              "deu@example.com", "Busan, South Korea");

    try {
        bank.loadData("bank_data.txt");
    } catch (const BankingException& e) {
        cout << "Could not load saved data: " << e.what() << '\n';
    } catch (const exception& e) {
        cout << "Could not load saved data: " << e.what() << '\n';
    }

    bank.run();

    try {
        bank.saveData("bank_data.txt");
        cout << "Data saved automatically.\n";
    } catch (const BankingException& e) {
        cout << "Could not save data: " << e.what() << '\n';
        return 1;
    } catch (const exception& e) {
        cout << "Could not save data: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
