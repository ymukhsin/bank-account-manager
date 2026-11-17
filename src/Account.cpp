#include "Account.h"

using namespace std;

Account::Account(string number, double initialBalance)
    : accountNumber(number), balance(initialBalance) {}

void Account::deposit(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException();
    }
    balance += amount;
}

double Account::getBalance() const {
    return balance;
}

string Account::getAccountNumber() const {
    return accountNumber;
}
