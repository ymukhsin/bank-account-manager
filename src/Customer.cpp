#include "Customer.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

Customer::Customer(std::string id, std::string name,
                   std::string phone, std::string mail)
    : customerId(std::move(id)),
      fullName(std::move(name)),
      phoneNumber(std::move(phone)),
      email(std::move(mail)) {}

Customer::~Customer() {
    for (Account* account : accounts) {
        delete account;
    }
}

void Customer::addAccount(Account* account) {
    if (account != nullptr) {
        accounts.push_back(account);
    }
}

void Customer::removeAccount(std::string accountNumber) {
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

Account* Customer::findAccount(std::string accountNumber) {
    for (Account* account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

void Customer::listAccounts() const {
    if (accounts.empty()) {
        std::cout << "No accounts found.\n";
        return;
    }

    for (Account* account : accounts) {
        account->display();
    }
}

double Customer::getTotalBalance() const {
    double total = 0.0;
    for (Account* account : accounts) {
        total += account->getBalance();
    }
    return total;
}

std::string Customer::getCustomerId() const {
    return customerId;
}

std::string Customer::getFullName() const {
    return fullName;
}

std::string Customer::getPhoneNumber() const {
    return phoneNumber;
}

std::string Customer::getEmail() const {
    return email;
}

const std::vector<Account*>& Customer::getAccounts() const {
    return accounts;
}
