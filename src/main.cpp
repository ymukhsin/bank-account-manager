#include "Bank.h"
#include "CheckingAccount.h"
#include "Exceptions.h"
#include "SavingsAccount.h"

#include <iostream>

int main() {
    Bank bank("OOP II Bank", "02-123-4567", "bank@example.com", "Seoul");

    bank.addCustomer(new Customer("C101", "Mukhsinjon",
                                  "010-1111-2222", "mukh@example.com"));
    bank.addCustomer(new Customer("C102", "Mina",
                                  "010-3333-4444", "mina@example.com"));

    bank.openAccount("C101",
                     new SavingsAccount("SAV-1001", 1000.0, 0.05, 100.0));
    bank.openAccount("C101",
                     new CheckingAccount("CHK-2001", 200.0, 300.0, 5.0));

    Customer* customer = bank.findCustomer("C101");
    if (customer != nullptr) {
        std::cout << "Customer: " << customer->getFullName() << '\n';
        customer->listAccounts();
        std::cout << "Total balance: " << customer->getTotalBalance() << '\n';
    }

    bank.deposit("SAV-1001", 500.0);
    bank.withdraw("CHK-2001", 100.0);

    SavingsAccount* savings =
        dynamic_cast<SavingsAccount*>(bank.findAccount("SAV-1001"));
    if (savings != nullptr) {
        savings->applyInterest();
    }

    std::cout << "\nAfter transactions:\n";
    Account* savingsAccount = bank.findAccount("SAV-1001");
    Account* checkingAccount = bank.findAccount("CHK-2001");

    std::cout << "Type: " << savingsAccount->getType() << '\n';
    savingsAccount->display();
    std::cout << "Type: " << checkingAccount->getType() << '\n';
    checkingAccount->display();

    try {
        bank.deposit("SAV-1001", 0.0);
    } catch (const InvalidAmountException& e) {
        std::cout << "\nInvalid amount: " << e.what() << '\n';
    }

    try {
        bank.withdraw("CHK-2001", 1000.0);
    } catch (const InsufficientFundsException& e) {
        std::cout << "Insufficient funds: " << e.what() << '\n';
    }

    try {
        bank.findAccount("MISSING");
    } catch (const AccountNotFoundException& e) {
        std::cout << "Missing account: " << e.what() << '\n';
    }

    return 0;
}
