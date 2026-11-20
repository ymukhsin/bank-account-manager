#ifndef BANK_ACCOUNT_MANAGER_ADMIN_H
#define BANK_ACCOUNT_MANAGER_ADMIN_H

#include <string>
#include <vector>
using namespace std;

class Bank;

class Admin {
    friend class FileStorage;
private:
    string adminName;
    string password;

public:
    Admin(string name, string pwd);
    bool login(string name, string pwd) const;
    void showBanks(const vector<Bank*>& banks) const;
};

#endif
