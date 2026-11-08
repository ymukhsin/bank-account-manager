#ifndef BANK_ACCOUNT_MANAGER_BANK_H
#define BANK_ACCOUNT_MANAGER_BANK_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Account.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"

/**
 * Bank
 * ----
 * Owns a collection of accounts through Account pointers so that calls
 * such as displayInfo() are resolved polymorphically at runtime.
 */
class Bank {
private:
    std::vector<Account*> accounts;

public:
    Bank() = default;

    // Bank owns its accounts. Copying would duplicate the raw pointers
    // and cause double deletion, so copying is disabled.
    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;

    // Free every account that was transferred to the bank.
    ~Bank() {
        for (Account* account : accounts) {
            delete account;
        }
    }

    // The bank takes ownership of acc and deletes it in ~Bank().
    void addAccount(Account* acc) {
        if (acc != nullptr) {
            accounts.push_back(acc);
        }
    }

    // Virtual dispatch selects the correct displayInfo() for each type.
    void showAllAccounts() const {
        if (accounts.empty()) {
            std::cout << " No accounts registered.\n";
            return;
        }

        for (Account* account : accounts) {
            account->displayInfo();
        }
    }

    // Return the first account held by ownerName, or nullptr if absent.
    Account* findAccount(const std::string& ownerName) {
        for (Account* account : accounts) {
            if (account->getHolderName() == ownerName) {
                return account;
            }
        }
        return nullptr;
    }
};

#endif // BANK_ACCOUNT_MANAGER_BANK_H
