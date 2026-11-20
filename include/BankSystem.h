#ifndef BANK_ACCOUNT_MANAGER_BANK_SYSTEM_H
#define BANK_ACCOUNT_MANAGER_BANK_SYSTEM_H

#include "Admin.h"
#include "Bank.h"
#include "BankStaff.h"
#include <string>
#include <vector>

using namespace std;

class FileStorage;

// BankSystem owns customer profiles. Each customer may open accounts at different banks.
class BankSystem {
    friend class FileStorage;
private:
    vector<Bank*> banks;
    vector<Customer*> customers;
    vector<BankStaff*> staffMembers;
    Admin admin;
    int nextCustomerId = 1001;
    int nextBankId = 2;
    int nextStaffId = 1;

public:
    BankSystem();
    ~BankSystem();

    Bank* findBank(string id) const;
    Customer* findCustomer(string id) const;
    Customer* loginCustomer(string emailOrPhone, string password) const;
    Customer* registerCustomer(string name, string phone, string email, string password);
    Bank* findBankByName(string name) const;
    BankStaff* loginStaff(string emailOrPhone, string pwd) const;
    BankStaff* registerBankAndStaff(string bankName, string bankPhone,
                                   string bankEmail, string bankAddress,
                                   string staffName, string staffPhone,
                                   string staffEmail, string staffPassword);
    const vector<Bank*>& getBanks() const;
    const vector<Customer*>& getCustomers() const;
    const vector<BankStaff*>& getStaffMembers() const;
    const Admin& getAdmin() const;
    void showBanks() const;
    void save() const;
    void run();
};

#endif
