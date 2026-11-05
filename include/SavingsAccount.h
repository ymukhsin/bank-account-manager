#ifndef BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
#define BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H

#include "Account.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

/**
 * SavingsAccount
 * --------------
 * Concrete Account that earns interest on its current balance.
 *
 * interestRate is stored as a decimal fraction (0.05 means 5%).
 */
class SavingsAccount : public Account {
private:
    double interestRate;

    // Simple course-project ID generator: SAV-1001, SAV-1002, ...
    static std::string generateAccountNumber() {
        static int nextNumber = 1001;
        return "SAV-" + std::to_string(nextNumber++);
    }

public:
    // The owner and opening balance belong to the Account base; the
    // interest rate is specific to a savings account.
    SavingsAccount(std::string ownerName, double initialBalance, double rate)
        : Account(generateAccountNumber(), std::move(ownerName), initialBalance),
          interestRate(rate) {}

    // Compound this period's interest into the balance.
    void addInterest() {
        balance += balance * interestRate;
    }

    void deposit(double amount) override {
        if (amount > 0.0) {
            balance += amount;
        }
    }

    void withdraw(double amount) override {
        if (amount > 0.0 && amount <= balance) {
            balance -= amount;
        }
    }

    void displayInfo() override {
        std::ios_base::fmtflags previousFlags = std::cout.flags();
        std::streamsize previousPrecision = std::cout.precision();

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n Savings Account Details\n";
        std::cout << " -----------------------\n";
        std::cout << " Account No    : " << accountNumber << '\n';
        std::cout << " Holder        : " << holderName << '\n';
        std::cout << " Balance       : " << balance << '\n';
        std::cout << " Interest Rate : " << (interestRate * 100.0) << "%\n";

        std::cout.flags(previousFlags);
        std::cout.precision(previousPrecision);
    }
};

#endif // BANK_ACCOUNT_MANAGER_SAVINGS_ACCOUNT_H
