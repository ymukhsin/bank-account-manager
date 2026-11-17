#include "SavingsAccount.h"

#include <iostream>
#include <utility>

using namespace std;

SavingsAccount::SavingsAccount(string number, double initialBalance,
                               double rate, double minimum)
    : Account(move(number), initialBalance),
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

string SavingsAccount::getType() {
    return "Savings";
}

void SavingsAccount::display() {
    cout << "\nSavings Account\n"
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

void SavingsAccount::setInterestRate(double rate) {
    if (rate < 0.0) {
        throw InvalidAmountException("Interest rate cannot be negative.");
    }
    interestRate = rate;
}
