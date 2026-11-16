#ifndef BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
#define BANK_ACCOUNT_MANAGER_EXCEPTIONS_H

#include <exception>
#include <string>

/**
 * BankingException
 * ----------------
 * Base class for every domain-specific error thrown by the bank.
 */
class BankingException : public std::exception {
private:
    std::string message;

public:
    explicit BankingException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidAmountException : public BankingException {
public:
    InvalidAmountException()
        : BankingException("Error: Invalid amount. Amount must be greater than zero.") {}
};

class InsufficientFundsException : public BankingException {
public:
    InsufficientFundsException()
        : BankingException("Error: Insufficient funds for withdrawal!") {}
};

class AccountNotFoundException : public BankingException {
public:
    AccountNotFoundException()
        : BankingException("Error: Account not found.") {}
};

class AuthenticationException : public BankingException {
public:
    AuthenticationException()
        : BankingException("Error: Authentication failed.") {}
};

#endif // BANK_ACCOUNT_MANAGER_EXCEPTIONS_H
