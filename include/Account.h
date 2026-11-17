#ifndef BANK_ACCOUNT_MANAGER_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_ACCOUNT_H

#include "Exceptions.h"

#include <string>

using namespace std;

/**
 * Account
 * -------
 * Abstract base class for all bank account types.
 */
class Account {
private:
    string accountNumber;

protected:
    double balance;

public:
    Account(string number, double initialBalance);
    virtual ~Account() {}

    void deposit(double amount);
    virtual void withdraw(double amount) = 0;

    double getBalance() const;
    string getAccountNumber() const;

    virtual string getType() = 0;
    virtual void display() = 0;
};

#endif // BANK_ACCOUNT_MANAGER_ACCOUNT_H
