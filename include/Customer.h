#ifndef BANK_ACCOUNT_MANAGER_CUSTOMER_H
#define BANK_ACCOUNT_MANAGER_CUSTOMER_H

#include "Account.h"

#include <string>
#include <vector>

using namespace std;

class Customer {
private:
    string customerId;
    string fullName;
    string phoneNumber;
    string email;
    string password;
    vector<Account*> accounts;

public:
    Customer(string id, string name, string phone,
             string mail, string pwd);
    ~Customer();

    Customer(const Customer&) = delete;
    Customer& operator=(const Customer&) = delete;

    void addAccount(Account* account);
    void removeAccount(string accountNumber);
    Account* findAccount(string accountNumber);
    void listAccounts() const;
    double getTotalBalance() const;

    string getCustomerId() const;
    string getFullName() const;
    string getPhoneNumber() const;
    string getEmail() const;
    string getPassword() const;
    bool verifyPassword(string pwd) const;
    const vector<Account*>& getAccounts() const;
};

#endif // BANK_ACCOUNT_MANAGER_CUSTOMER_H
