#ifndef BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H

#include "Account.h"

#include <string>

class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount(std::string number, double initialBalance,
                   double rate, double minimum);

    void withdraw(double amount) override;
    void applyInterest();

    std::string getType() override;
    void display() override;

    double getInterestRate() const;
    double getMinimumBalance() const;
};

#endif // BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
