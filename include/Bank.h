#ifndef BANK_ACCOUNT_MANAGER_BANK_H
#define BANK_ACCOUNT_MANAGER_BANK_H

#include "Account.h"
#include "Customer.h"

#include <string>
#include <vector>

using namespace std;

class FileStorage;

class Bank {
    friend class FileStorage;
private:
    string bankId;
    string bankName;
    string phoneNumber;
    string email;
    string address;
    string status = "APPROVED"; // APPROVED, PENDING, REJECTED, INACTIVE
    long nextAccountNumber = 10000001;
    double defaultInterestRate = 0.05;
    double defaultTransactionFee = 2.0;
    vector<Customer*> customers; // References to customers with accounts here; BankSystem owns them.

    string generateAccountNumber();

public:
    Bank(string id, string name, string phone,
         string mail, string location);
    ~Bank();

    Customer* findCustomer(string customerId);

    string openSavingsAccount(Customer* customer, double initialBalance,
                              double minimum);
    string openCheckingAccount(Customer* customer, double initialBalance,
                               double limit);

    void openAccount(Customer* customer, Account* account);
    void closeAccount(string accountNumber);
    Account* findAccount(string accountNumber);

    void deposit(string accountNumber, double amount);
    void withdraw(string accountNumber, double amount);

    void viewAllCustomers();
    void updateAllInterestRates(double newRate);
    void updateAllTransactionFees(double newFee);

    string getBankId() const;
    string getBankName() const;
    string getPhoneNumber() const;
    string getEmail() const;
    string getAddress() const;
    string getStatus() const;
    void setStatus(string newStatus);
    bool isActive() const;
    double getDefaultInterestRate() const;
    double getDefaultTransactionFee() const;
    const vector<Customer*>& getCustomers() const;
};

#endif // BANK_ACCOUNT_MANAGER_BANK_H
