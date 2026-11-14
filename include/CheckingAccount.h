#ifndef BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H

#include "Account.h"
#include "InsufficientFundsException.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

/**
 * CheckingAccount
 * ---------------
 * Concrete Account that permits withdrawals beyond the available
 * balance, up to a fixed overdraft limit.
 */
class CheckingAccount : public Account {
private:
    double overdraftLimit;

    // Simple course-project ID generator: CHK-2001, CHK-2002, ...
    static std::string generateAccountNumber() {
        static int nextNumber = 2001;
        return "CHK-" + std::to_string(nextNumber++);
    }

public:
    // The owner and opening balance belong to the Account base; the
    // overdraft limit is specific to a checking account.
    CheckingAccount(std::string ownerName, double initialBalance, double limit)
        : Account(generateAccountNumber(), std::move(ownerName), initialBalance),
          overdraftLimit(limit) {}

    void deposit(double amount) override {
        if (amount > 0.0) {
            balance += amount;
        }
    }

    // A withdrawal is allowed while the resulting balance stays at or
    // above the negative overdraft limit; otherwise it is rejected.
    void withdraw(double amount) override {
        if (amount <= 0.0) {
            return;
        }
        if ((balance - amount) < -overdraftLimit) {
            throw InsufficientFundsException(amount, balance);
        }
        balance -= amount;
    }

    std::string getType() const override {
        return "Checking";
    }

    void displayInfo() override {
        std::ios_base::fmtflags previousFlags = std::cout.flags();
        std::streamsize previousPrecision = std::cout.precision();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n Checking Account Details\n";
        std::cout << " ------------------------\n";
        std::cout << " Account No      : " << accountNumber << '\n';
        std::cout << " Holder          : " << holderName << '\n';
        std::cout << " Balance         : " << balance << '\n';
        std::cout << " Overdraft Limit : " << overdraftLimit << '\n';

        std::cout.flags(previousFlags);
        std::cout.precision(previousPrecision);
    }
};

#endif // BANK_ACCOUNT_MANAGER_CHECKING_ACCOUNT_H
