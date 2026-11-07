#ifndef BANK_ACCOUNT_MANAGER_INSUFFICIENT_FUNDS_EXCEPTION_H
#define BANK_ACCOUNT_MANAGER_INSUFFICIENT_FUNDS_EXCEPTION_H

#include <exception>

/**
 * InsufficientFundsException
 * --------------------------
 * Thrown when a withdrawal would exceed the funds available to an
 * account, including any permitted overdraft.
 */
class InsufficientFundsException : public std::exception {
private:
    double requestedAmount;
    double currentBalance;

public:
    InsufficientFundsException(double req, double bal)
        : requestedAmount(req), currentBalance(bal) {}

    const char* what() const noexcept override {
        return "Error: Insufficient funds for withdrawal!";
    }

    double getRequestedAmount() const {
        return requestedAmount;
    }

    double getCurrentBalance() const {
        return currentBalance;
    }
};

#endif // BANK_ACCOUNT_MANAGER_INSUFFICIENT_FUNDS_EXCEPTION_H
