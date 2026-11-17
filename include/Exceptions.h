#ifndef BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
#define BANK_ACCOUNT_MANAGER_EXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
 * BankingException
 * ----------------
 * Base class for all domain errors raised by the bank.
 */
class BankingException : public std::runtime_error {
public:
    explicit BankingException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidAmountException : public BankingException {
public:
    explicit InvalidAmountException(const std::string& message = "Invalid amount.")
        : BankingException(message) {}
};

class InsufficientFundsException : public BankingException {
public:
    explicit InsufficientFundsException(const std::string& message = "Insufficient funds.")
        : BankingException(message) {}
};

class AccountNotFoundException : public BankingException {
public:
    explicit AccountNotFoundException(const std::string& message = "Account not found.")
        : BankingException(message) {}
};

#endif // BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
