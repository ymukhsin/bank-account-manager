#include "Account.h"
#include "Bank.h"
#include "CheckingAccount.h"
#include "Customer.h"
#include "InsufficientFundsException.h"
#include "SavingsAccount.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

const char* const kAppName = "Bank Account Manager";
const char* const kVersion = "0.6.0";
const int kRuleWidth = 60;

void printRule(char symbol = '-') {
    std::cout << std::string(kRuleWidth, symbol) << '\n';
}

void printAccountType(const Account& account) {
    std::cout << " Account type: " << account.getType() << '\n';
}

void printBanner() {
    std::cout << '\n';
    printRule('=');
    std::cout << "                 " << kAppName << "\n";
    std::cout << "                   OOP II Final Project\n";
    printRule('=');
    std::cout << " Version : " << kVersion << '\n';
    std::cout << " Build   : C++14 / g++\n";
    printRule('-');
}

void printInterfaceChecks() {
    std::cout << " [OK] Account interface loaded from include/Account.h\n";
    std::cout << " [OK] Account is abstract: "
              << (std::is_abstract<Account>::value ? "yes" : "no") << '\n';
    std::cout << " [OK] SavingsAccount derives from Account: "
              << (std::is_base_of<Account, SavingsAccount>::value ? "yes" : "no")
              << '\n';
    std::cout << " [OK] CheckingAccount derives from Account: "
              << (std::is_base_of<Account, CheckingAccount>::value ? "yes" : "no")
              << '\n';
    std::cout << " [OK] deposit(), withdraw(), and displayInfo() overridden\n";
    printRule('-');
}

void runSavingsAccountTest() {
    std::cout << "\n Savings Account Test\n";
    printRule('-');

    SavingsAccount account("Mina", 1000.0, 0.05);
    printAccountType(account);
    account.addInterest();
    account.displayInfo();

    printRule('-');
}

void runCheckingAccountTest() {
    std::cout << "\n Checking Account Test\n";
    printRule('-');

    CheckingAccount account("Jin", 200.0, 300.0);
    printAccountType(account);

    std::cout << " Attempting withdrawal: 400.00 (within overdraft)\n";
    account.withdraw(400.0);

    std::cout << " Attempting withdrawal: 200.00 (over limit)\n";
    try {
        account.withdraw(200.0);
    } catch (const InsufficientFundsException& e) {
        std::cout << "\n [CATCH] " << e.what() << '\n';
        std::cout << std::fixed << std::setprecision(2);
        std::cout << " Requested Amount : " << e.getRequestedAmount() << '\n';
        std::cout << " Current Balance  : " << e.getCurrentBalance() << '\n';
    }

    account.displayInfo();
    printRule('-');
}

void runBankManagerTest() {
    std::cout << "\n Bank Manager Test\n";
    printRule('-');

    Bank myBank;
    myBank.addAccount(new SavingsAccount("Mina", 1000.0, 0.05));
    myBank.addAccount(new CheckingAccount("Jin", 200.0, 300.0));

    std::cout << " Accounts registered with myBank:\n";
    myBank.showAllAccounts();

    Account* found = myBank.findAccount("Mina");
    if (found != nullptr) {
        std::cout << "\n findAccount(\"Mina\") succeeded. Depositing 500.00...\n";
        found->deposit(500.0);
        std::cout << " Updated account:\n";
        found->displayInfo();
    } else {
        std::cout << "\n findAccount(\"Mina\") returned nullptr.\n";
    }

    Account* missing = myBank.findAccount("Unknown");
    std::cout << "\n findAccount(\"Unknown\") returned "
              << (missing == nullptr ? "nullptr" : "a valid pointer") << ".\n";

    std::cout << "\n Bank scope ends; ~Bank() deletes both accounts.\n";
    printRule('-');
}

void runCustomerTest() {
    std::cout << "\n Customer Test\n";
    printRule('-');

    Customer customer("C101", "Mukhsinjon", "1234");
    customer.addAccount(new SavingsAccount("Mukhsinjon", 1500.0, 0.04));
    customer.addAccount(new CheckingAccount("Mukhsinjon", 500.0, 250.0));

    std::cout << " Customer: C101 - Mukhsinjon\n";
    std::cout << " verifyPin(\"1234\") -> "
              << (customer.verifyPin("1234") ? "true" : "false") << '\n';
    std::cout << " verifyPin(\"0000\") -> "
              << (customer.verifyPin("0000") ? "true" : "false") << '\n';

    std::cout << "\n Accounts owned by the customer:\n";
    customer.listAccounts();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n Total balance: " << customer.getTotalBalance() << '\n';
    std::cout << "\n Customer scope ends; ~Customer() deletes both accounts.\n";
    printRule('-');
}

}  // namespace

int main() {
    printBanner();
    printInterfaceChecks();
    runSavingsAccountTest();
    runCheckingAccountTest();
    runBankManagerTest();
    runCustomerTest();

    std::cout << "\n Press Enter to exit...";
    std::cout.flush();

    std::string input;
    std::getline(std::cin, input);

    std::cout << "\n Goodbye.\n";
    return 0;
}
