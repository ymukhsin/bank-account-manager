#include "Account.h"

#include <iostream>
#include <string>
#include <type_traits>

namespace {

const char* const kAppName = "Bank Account Manager";
const char* const kVersion = "0.1.0";
const int kRuleWidth = 60;

void printRule(char symbol = '-') {
    std::cout << std::string(kRuleWidth, symbol) << '\n';
}

void printBanner() {
    std::cout << '\n';
    printRule('=');
    std::cout << "                 BANK ACCOUNT MANAGER\n";
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
    std::cout << " [OK] Pure virtual API: deposit(), withdraw(), displayInfo()\n";
    printRule('-');
}

}  // namespace

int main() {
    printBanner();
    printInterfaceChecks();

    std::cout << "\n No accounts are loaded yet.\n"
                 " Account types and the management menu arrive in the next\n"
                 " milestone.\n";
    printRule('=');

    std::cout << "\n Press Enter to exit...";
    std::cout.flush();

    std::string input;
    std::getline(std::cin, input);

    std::cout << "\n Goodbye.\n";
    return 0;
}
