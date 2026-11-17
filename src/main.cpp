#include "Bank.h"
#include "Exceptions.h"

#include <iostream>

int main() {
    Bank bank("OOP II Bank", "02-123-4567",
              "bank@example.com", "123 Main Street, Seoul");

    try {
        bank.loadData("bank_data.txt");
    } catch (const BankingException& e) {
        std::cout << "Could not load saved data: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Could not load saved data: " << e.what() << '\n';
    }

    bank.run();

    try {
        bank.saveData("bank_data.txt");
        std::cout << "Data saved to bank_data.txt.\n";
    } catch (const BankingException& e) {
        std::cout << "Could not save data: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Could not save data: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
