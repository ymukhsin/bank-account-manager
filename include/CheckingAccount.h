#ifndef BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H

#include "Account.h"

#include <string>

class CheckingAccount : public Account {
private:
    double overdraftLimit;
    double transactionFee;

public:
    CheckingAccount(std::string number, double initialBalance,
                    double limit, double fee);

    void withdraw(double amount) override;

    std::string getType() override;
    void display() override;

    double getOverdraftLimit() const;
    double getTransactionFee() const;
};

#endif // BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H
