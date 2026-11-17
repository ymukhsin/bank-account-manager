#include "SavingsAccount.h"

#include <iostream>
#include <utility>

SavingsAccount::SavingsAccount(std::string number, double initialBalance,
                               double rate, double minimum)
    : Account(std::move(number), initialBalance),
      interestRate(rate),
      minimumBalance(minimum) {}

void SavingsAccount::withdraw(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException();
    }
    if (balance - amount < minimumBalance) {
        throw InsufficientFundsException();
    }
    balance -= amount;
}

void SavingsAccount::applyInterest() {
    balance += balance * interestRate;
}

std::string SavingsAccount::getType() {
    return "Savings";
}

void SavingsAccount::display() {
    std::cout << "\nSavings Account\n"
              << "Account Number: " << getAccountNumber() << '\n'
              << "Balance: " << balance << '\n'
              << "Interest Rate: " << interestRate << '\n'
              << "Minimum Balance: " << minimumBalance << '\n';
}

double SavingsAccount::getInterestRate() const {
    return interestRate;
}

double SavingsAccount::getMinimumBalance() const {
    return minimumBalance;
}
