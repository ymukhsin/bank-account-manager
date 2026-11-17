#include "CheckingAccount.h"

#include <iostream>
#include <utility>

CheckingAccount::CheckingAccount(std::string number, double initialBalance,
                                 double limit, double fee)
    : Account(std::move(number), initialBalance),
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

std::string CheckingAccount::getType() {
    return "Checking";
}

void CheckingAccount::display() {
    std::cout << "\nChecking Account\n"
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
