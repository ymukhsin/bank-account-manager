#ifndef BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H

#include "Account.h"

#include <string>

using namespace std;

class SavingsAccount : public Account {
private:
    double interestRate;
    double minimumBalance;

public:
    SavingsAccount(string number, double initialBalance,
                   double rate, double minimum);

    void withdraw(double amount) override;
    void applyInterest();

    string getType() override;
    void display() override;

    double getInterestRate() const;
    double getMinimumBalance() const;
    void setInterestRate(double rate);
};

#endif // BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
