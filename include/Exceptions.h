#ifndef BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
#define BANK_ACCOUNT_MANAGER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

/**
 * BankingException
 * ----------------
 * Base class for all errors raised by the bank.
 */
class BankingException : public runtime_error {
public:
    BankingException(const string& message)
        : runtime_error(message) {}
};

class InvalidAmountException : public BankingException {
public:
    InvalidAmountException(const string& message = "Invalid amount.")
        : BankingException(message) {}
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException(const string& message = "Insufficient funds.")
        : BankingException(message) {}
};

class AccountNotFoundException : public BankingException {
public:
    AccountNotFoundException(const string& message = "Account not found.")
        : BankingException(message) {}
};

#endif // BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
