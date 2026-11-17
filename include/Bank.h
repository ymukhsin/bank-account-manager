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
    int nextCustomerId = 1001;
    long nextAccountNumber = 10000001;
    double defaultInterestRate = 0.05;
    double defaultTransactionFee = 2.0;
    vector<Customer*> customers;

    string generateCustomerId();
    string generateAccountNumber();

public:
    Bank(string name, string phone,
         string mail, string location);
    ~Bank();

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;

    string addCustomer(string name, string phone,
                       string email, string password);
    Customer* findCustomer(string customerId);
    Customer* loginCustomer(string emailOrPhone, string password);

    string openSavingsAccount(string customerId, double initialBalance,
                              double minimum);
    string openCheckingAccount(string customerId, double initialBalance,
                               double limit);

    void openAccount(string customerId, Account* account);
    void closeAccount(string accountNumber);
    Account* findAccount(string accountNumber);

    void deposit(string accountNumber, double amount);
    void withdraw(string accountNumber, double amount);

    void viewAllCustomers();
    void updateAllInterestRates(double newRate);
    void updateAllTransactionFees(double newFee);

    void saveData(string filePath);
    void loadData(string filePath);

    void run();
};

#endif // BANK_ACCOUNT_MANAGER_BANK_H
