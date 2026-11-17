#ifndef BANK_ACCOUNT_MANAGER_BANK_H
#define BANK_ACCOUNT_MANAGER_BANK_H

#include "Account.h"
#include "Customer.h"

#include <string>
#include <vector>

using namespace std;

class Bank {
private:
    string bankName;
    string phoneNumber;
    string email;
    string address;
    vector<Customer*> customers;

public:
    Bank(string name, string phone,
         string mail, string location);
    ~Bank();

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;

    void addCustomer(string id, string name,
                     string phone, string email);
    Customer* findCustomer(string customerId);

    void openAccount(string customerId, Account* account);
    void closeAccount(string accountNumber);
    Account* findAccount(string accountNumber);

    void deposit(string accountNumber, double amount);
    void withdraw(string accountNumber, double amount);

    void saveData(string filePath);
    void loadData(string filePath);

    void run();
};

#endif // BANK_ACCOUNT_MANAGER_BANK_H
