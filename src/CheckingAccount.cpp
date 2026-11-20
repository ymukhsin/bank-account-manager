#include "CheckingAccount.h"

#include <iostream>
#include <cmath>

using namespace std;

CheckingAccount::CheckingAccount(string number, double initialBalance,
                                 double limit, double fee, string bank)
    : Account(number, initialBalance, bank),
      overdraftLimit(limit),
      transactionFee(fee) {}

void CheckingAccount::withdraw(double amount) {
    if (!isfinite(amount) || amount <= 0.0) {
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
    cout << "\nChecking Account (Bank " << getBankId() << ")\n"
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

void CheckingAccount::setTransactionFee(double fee) {
    if (!isfinite(fee) || fee < 0.0) {
        throw InvalidAmountException("Transaction fee cannot be negative.");
    }
    transactionFee = fee;
}
