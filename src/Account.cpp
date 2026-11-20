#include "Account.h"
#include <cmath>

using namespace std;

Account::Account(string number, double initialBalance, string bank)
    : accountNumber(number), bankId(bank), balance(initialBalance) {}

void Account::deposit(double amount) {
    // isfinite() - rejects zero, negative, NaN, or infinite amounts.
    if (!isfinite(amount) || amount <= 0.0) {
        throw InvalidAmountException();
    }
    if (!isfinite(balance + amount)) {
        throw InvalidAmountException("Balance is too large.");
    }
    balance += amount;
}

double Account::getBalance() const {
    return balance;
}

string Account::getAccountNumber() const {
    return accountNumber;
}

string Account::getBankId() const {
    return bankId;
}
