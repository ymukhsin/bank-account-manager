#ifndef BANK_ACCOUNT_MANAGER_CUSTOMER_H
#define BANK_ACCOUNT_MANAGER_CUSTOMER_H

#include "Account.h"
#include "Exceptions.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

/**
 * Customer
 * --------
 * Represents one bank customer, verifies the customer's PIN, and owns
 * that customer's accounts through Account pointers.
 */
class Customer {
private:
    std::string customerId;
    std::string name;
    std::string pin;
    std::vector<Account*> accounts;

public:
    Customer(std::string id, std::string customerName, std::string customerPin)
        : customerId(std::move(id)),
          name(std::move(customerName)),
          pin(std::move(customerPin)) {}

    // Customer owns its accounts, so copying raw pointers would cause
    // double deletion. Copying is therefore disabled.
    Customer(const Customer&) = delete;
    Customer& operator=(const Customer&) = delete;

    // Delete every account transferred to this customer.
    ~Customer() {
        for (Account* account : accounts) {
            delete account;
        }
    }

    std::string getCustomerId() const {
        return customerId;
    }

    std::string getName() const {
        return name;
    }

    bool verifyPin(const std::string& enteredPin) const {
        return pin == enteredPin;
    }

    // The customer takes ownership of a and deletes it in ~Customer().
    void addAccount(Account* a) {
        if (a != nullptr) {
            accounts.push_back(a);
        }
    }

    // Remove and delete an account; throw if the number is unknown.
    void removeAccount(const std::string& accountNumber) {
        for (std::vector<Account*>::iterator it = accounts.begin();
             it != accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accountNumber) {
                delete *it;
                accounts.erase(it);
                return;
            }
        }
        throw AccountNotFoundException();
    }

    // Return the account with the matching account number, or nullptr.
    Account* findAccount(const std::string& accountNumber) {
        for (Account* account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account;
            }
        }
        return nullptr;
    }

    // Virtual dispatch calls the correct displayInfo() for each account.
    void listAccounts() const {
        if (accounts.empty()) {
            std::cout << " No accounts found for this customer.\n";
            return;
        }

        for (Account* account : accounts) {
            account->displayInfo();
        }
    }

    double getTotalBalance() const {
        double total = 0.0;
        for (Account* account : accounts) {
            total += account->getBalance();
        }
        return total;
    }
};

#endif // BANK_ACCOUNT_MANAGER_CUSTOMER_H
