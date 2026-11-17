#include "Account.h"

#include <utility>

Account::Account(std::string number, double initialBalance)
    : accountNumber(std::move(number)), balance(initialBalance) {}

void Account::deposit(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException();
    }
    balance += amount;
}

double Account::getBalance() const {
    return balance;
}

std::string Account::getAccountNumber() const {
    return accountNumber;
}
