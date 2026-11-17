#include "CheckingAccount.h"

#include <iostream>
#include <utility>

using namespace std;

CheckingAccount::CheckingAccount(string number, double initialBalance,
                                 double limit, double fee)
    : Account(move(number), initialBalance),
      overdraftLimit(limit),
      transactionFee(fee) {}

void CheckingAccount::withdraw(double amount) {
    if (amount <= 0.0) {
        throw InvalidAmountException();
    }
    if (balance - (amount + transactionFee) < -overdraftLimit) {
        throw InsufficientFundsException();
    }
    balance -= (amount + transactionFee);
}

string CheckingAccount::getType() {
    return "Checking";
}

void CheckingAccount::display() {
    cout << "\nChecking Account\n"
              << "Account Number: " << getAccountNumber() << '\n'
              << "Balance: " << balance << '\n'
              << "Overdraft Limit: " << overdraftLimit << '\n'
              << "Transaction Fee: " << transactionFee << '\n';
}

double CheckingAccount::getOverdraftLimit() const {
    return overdraftLimit;
}

double CheckingAccount::getTransactionFee() const {
    return transactionFee;
}
