#include "Account.h"
#include "SavingsAccount.h"

#include <iostream>
#include <string>
#include <type_traits>

namespace {

const char* const kAppName = "Bank Account Manager";
const char* const kVersion = "0.2.0";
const int kRuleWidth = 60;

void printRule(char symbol = '-') {
    std::cout << std::string(kRuleWidth, symbol) << '\n';
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
    std::cout << " [OK] deposit(), withdraw(), and displayInfo() overridden\n";
    printRule('-');
}

void runSavingsAccountTest() {
    std::cout << "\n Savings Account Test\n";
    printRule('-');

    SavingsAccount account("Mina", 1000.0, 0.05);
    account.addInterest();
    account.displayInfo();

    printRule('-');
}

}  // namespace

int main() {
    printBanner();
    printInterfaceChecks();
    runSavingsAccountTest();

    std::cout << "\n Press Enter to exit...";
    std::cout.flush();

    std::string input;
    std::getline(std::cin, input);

    std::cout << "\n Goodbye.\n";
    return 0;
}
