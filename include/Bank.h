#ifndef BANK_ACCOUNT_MANAGER_BANK_H
#define BANK_ACCOUNT_MANAGER_BANK_H

#include "Account.h"
#include "Customer.h"

#include <string>
#include <vector>

class Bank {
private:
    std::string bankName;
    std::string phoneNumber;
    std::string email;
    std::string address;
    std::vector<Customer*> customers;

public:
    Bank(std::string name, std::string phone,
         std::string mail, std::string location);
    ~Bank();

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;

    void addCustomer(Customer* customer);
    Customer* findCustomer(std::string customerId);

    void openAccount(std::string customerId, Account* account);
    void closeAccount(std::string accountNumber);
    Account* findAccount(std::string accountNumber);

    void deposit(std::string accountNumber, double amount);
    void withdraw(std::string accountNumber, double amount);

    void saveData(std::string filePath);
    void loadData(std::string filePath);
};

#endif // BANK_ACCOUNT_MANAGER_BANK_H
