#ifndef BANK_ACCOUNT_MANAGER_CUSTOMER_H
#define BANK_ACCOUNT_MANAGER_CUSTOMER_H

#include "Account.h"

#include <string>
#include <vector>

class Customer {
private:
    std::string customerId;
    std::string fullName;
    std::string phoneNumber;
    std::string email;
    std::vector<Account*> accounts;

public:
    Customer(std::string id, std::string name,
             std::string phone, std::string mail);
    ~Customer();

    Customer(const Customer&) = delete;
    Customer& operator=(const Customer&) = delete;

    void addAccount(Account* account);
    void removeAccount(std::string accountNumber);
    Account* findAccount(std::string accountNumber);
    void listAccounts() const;
    double getTotalBalance() const;

    std::string getCustomerId() const;
    std::string getFullName() const;
    std::string getPhoneNumber() const;
    std::string getEmail() const;
    const std::vector<Account*>& getAccounts() const;
};

#endif // BANK_ACCOUNT_MANAGER_CUSTOMER_H
