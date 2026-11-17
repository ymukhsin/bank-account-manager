#ifndef BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
#define BANK_ACCOUNT_MANAGER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

/**
 * BankingException
 * ----------------
 * Base class for all domain errors raised by the bank.
 */
class BankingException : public runtime_error {
public:
    explicit BankingException(const string& message)
        : runtime_error(message) {}
};

class InvalidAmountException : public BankingException {
public:
    explicit InvalidAmountException(const string& message = "Invalid amount.")
        : BankingException(message) {}
};

class InsufficientFundsException : public BankingException {
public:
    explicit InsufficientFundsException(const string& message = "Insufficient funds.")
        : BankingException(message) {}
};

class AccountNotFoundException : public BankingException {
public:
    explicit AccountNotFoundException(const string& message = "Account not found.")
        : BankingException(message) {}
};

#endif // BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
