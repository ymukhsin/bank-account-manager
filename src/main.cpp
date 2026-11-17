#include "Bank.h"
#include "Exceptions.h"

#include <iostream>

using namespace std;

int main() {
    Bank bank("deu", "02-123-4567",
              "bank@example.com", "123 Main Street, Seoul");

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
        cout << "Data saved to bank_data.txt.\n";
    } catch (const BankingException& e) {
        cout << "Could not save data: " << e.what() << '\n';
        return 1;
    } catch (const exception& e) {
        cout << "Could not save data: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
