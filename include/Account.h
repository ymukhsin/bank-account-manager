#ifndef BANK_ACCOUNT_MANAGER_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_ACCOUNT_H

#include "Exceptions.h"

#include <string>
#include <utility>

/**
 * Account
 * -------
 * Abstract base class for every account type in Bank Account Manager.
 *
 * It owns the state shared by all accounts and defines the transaction
 * and reporting contract that concrete account types must implement.
 * Because the class is abstract, clients interact with accounts through
 * Account pointers or references (polymorphism).
 */
class Account {
protected:
    std::string accountNumber;  // Unique identifier assigned by the bank
    std::string holderName;     // Full name of the account holder
    double balance = 0.0;       // Current balance in the account currency

    // Derived constructors use this to initialise the shared account
    // state through an initializer list.
    Account(std::string number, std::string name, double initialBalance)
        : accountNumber(std::move(number)),
          holderName(std::move(name)),
          balance(initialBalance) {}

public:
    // Virtual destructor: deleting a derived account through an Account*
    // runs the complete derived-class destructor chain.
    virtual ~Account() = default;

    // Read-only access used by Bank and Customer lookups and reporting.
    const std::string& getHolderName() const {
        return holderName;
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    // Return the concrete account type name.
    virtual std::string getType() const = 0;

    // Validate and add a positive amount to the balance.
    void deposit(double amount) {
        if (amount <= 0.0) {
            throw InvalidAmountException();
        }
        balance += amount;
    }

    // Remove amount from the balance. Implementations reject invalid
    // amounts and must not let the balance fall below their limit.
    virtual void withdraw(double amount) = 0;

    // Print a formatted summary of this account to std::cout.
    virtual void displayInfo() = 0;
};

#endif // BANK_ACCOUNT_MANAGER_ACCOUNT_H
