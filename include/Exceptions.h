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
    InvalidAmountException() : BankingException("Invalid amount.") {}
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException() : BankingException("Insufficient funds.") {}
};

class AccountNotFoundException : public BankingException {
public:
    AccountNotFoundException() : BankingException("Account not found.") {}
};

#endif // BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
