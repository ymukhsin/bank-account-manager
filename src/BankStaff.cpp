#include "BankStaff.h"
using namespace std;

BankStaff::BankStaff(string id, string bank, string name,
                     string phone, string mail, string pwd)
    : staffId(id), bankId(bank), fullName(name),
      phoneNumber(phone), email(mail), password(pwd) {}

bool BankStaff::verifyPassword(string pwd) const { return password == pwd; }
string BankStaff::getStaffId() const { return staffId; }
string BankStaff::getBankId() const { return bankId; }
string BankStaff::getFullName() const { return fullName; }
string BankStaff::getPhoneNumber() const { return phoneNumber; }
string BankStaff::getEmail() const { return email; }
string BankStaff::getPassword() const { return password; }
