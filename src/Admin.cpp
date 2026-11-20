#include "Admin.h"
#include "Bank.h"
#include <iostream>

using namespace std;

Admin::Admin(string name, string pwd)
    : adminName(name), password(pwd) {}

bool Admin::login(string name, string pwd) const {
    return adminName == name && password == pwd;
}

void Admin::showBanks(const vector<Bank*>& banks) const {
    cout << "\n========== REGISTERED BANKS ==========\n";
    if (banks.empty()) {
        cout << "No banks registered.\n";
    }
    for (Bank* bank : banks) {
        cout << bank->getBankId() << " - " << bank->getBankName()
             << " | " << bank->getAddress()
             << " | Status: " << bank->getStatus() << '\n';
    }
}
