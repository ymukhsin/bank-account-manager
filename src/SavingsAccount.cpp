#include "SavingsAccount.h"

#include <iostream>
#include <cmath>

using namespace std;

SavingsAccount::SavingsAccount(string number, double initialBalance,
                               double rate, double minimum, string bank)
    : Account(number, initialBalance, bank),
      interestRate(rate),
      minimumBalance(minimum) {}

void SavingsAccount::withdraw(double amount) {
    if (!isfinite(amount) || amount <= 0.0) {
        throw InvalidAmountException();
    }
    if (balance - amount < minimumBalance) {
        throw InsufficientFundsException();
    }
    balance -= amount;
}

void SavingsAccount::applyInterest() {
    if (!isfinite(balance + balance * interestRate)) {
        throw InvalidAmountException("Interest result is invalid.");
    }
    balance += balance * interestRate;
}

string SavingsAccount::getType() {
    return "Savings";
}

void SavingsAccount::display() {
    cout << "\nSavings Account (Bank " << getBankId() << ")\n"
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
    if (!isfinite(rate) || rate < 0.0) {
        throw InvalidAmountException("Interest rate cannot be negative.");
    }
    interestRate = rate;
}
