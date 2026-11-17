#include "Customer.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

using namespace std;

Customer::Customer(string id, string name,
                   string phone, string mail, string pwd)
    : customerId(move(id)),
      fullName(move(name)),
      phoneNumber(move(phone)),
      email(move(mail)),
      password(move(pwd)) {}

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

void Customer::removeAccount(string accountNumber) {
    for (vector<Account*>::iterator it = accounts.begin();
         it != accounts.end(); ++it) {
        if ((*it)->getAccountNumber() == accountNumber) {
            delete *it;
            accounts.erase(it);
            return;
        }
    }
    throw AccountNotFoundException();
}

Account* Customer::findAccount(string accountNumber) {
    for (Account* account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

void Customer::listAccounts() const {
    if (accounts.empty()) {
        cout << "No accounts found.\n";
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

string Customer::getCustomerId() const {
    return customerId;
}

string Customer::getFullName() const {
    return fullName;
}

string Customer::getPhoneNumber() const {
    return phoneNumber;
}

string Customer::getEmail() const {
    return email;
}

string Customer::getPassword() const {
    return password;
}

bool Customer::verifyPassword(string pwd) const {
    return password == pwd;
}

const vector<Account*>& Customer::getAccounts() const {
    return accounts;
}
