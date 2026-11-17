#ifndef BANK_ACCOUNT_MANAGER_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_ACCOUNT_H

#include "Exceptions.h"

#include <string>

/**
 * Account
 * -------
 * Abstract base class for all bank account types.
 */
class Account {
private:
    std::string accountNumber;

protected:
    double balance;

public:
    Account(std::string number, double initialBalance);
    virtual ~Account() {}

    void deposit(double amount);
    virtual void withdraw(double amount) = 0;

    double getBalance() const;
    std::string getAccountNumber() const;

    virtual std::string getType() = 0;
    virtual void display() = 0;
};

#endif // BANK_ACCOUNT_MANAGER_ACCOUNT_H
