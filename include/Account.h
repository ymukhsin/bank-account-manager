#ifndef BANK_ACCOUNT_MANAGER_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_ACCOUNT_H

#include <string>

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

public:
    // Virtual destructor: deleting a derived account through an Account*
    // runs the complete derived-class destructor chain.
    virtual ~Account() = default;

    // Add amount to the balance. Implementations must reject amounts
    // that are zero or negative.
    virtual void deposit(double amount) = 0;

    // Remove amount from the balance. Implementations must reject
    // amounts that are zero or negative and must not let the balance
    // fall below the account type's permitted minimum.
    virtual void withdraw(double amount) = 0;

    // Print a formatted summary of this account to std::cout.
    virtual void displayInfo() = 0;
};

#endif // BANK_ACCOUNT_MANAGER_ACCOUNT_H
