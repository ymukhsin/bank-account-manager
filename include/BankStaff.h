#ifndef BANK_ACCOUNT_MANAGER_BANK_STAFF_H
#define BANK_ACCOUNT_MANAGER_BANK_STAFF_H

#include <string>
using namespace std;

class BankStaff {
private:
    string staffId;
    string bankId;
    string fullName;
    string phoneNumber;
    string email;
    string password;

public:
    BankStaff(string id, string bank, string name,
              string phone, string mail, string pwd);

    bool verifyPassword(string pwd) const;
    string getStaffId() const;
    string getBankId() const;
    string getFullName() const;
    string getPhoneNumber() const;
    string getEmail() const;
    string getPassword() const;
};

#endif
