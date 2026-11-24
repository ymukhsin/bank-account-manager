#include "FileStorage.h"
#include "BankSystem.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "Exceptions.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

using namespace std;

// File parsing helpers used by FileStorage.
vector<string> splitLine(const string& line) {
    vector<string> parts;
    size_t start = 0;
    size_t end;

    // BANK|B001|deu|01000000000|deu@example.com|Busan, South Korea|0.05|2|APPROVED
    while ((end = line.find('|', start)) != string::npos) {
        parts.push_back(line.substr(start, end - start));
        start = end + 1;
    }

    parts.push_back(line.substr(start));
    return parts;
}

double readNumber(string value) {
    try {
        size_t used = 0;
        double result = stod(value, &used);
        if (used != value.size() || !isfinite(result)) {
            throw BankingException("Invalid number in saved data.");
        }
        return result;
    } catch (const exception&) {
        throw BankingException("Invalid number in saved data.");
    }
}

int readIdNumber(string value, char prefix) {
    // value = "B001", prefix = "B"
    try {
        if (value.size() < 2 || value[0] != prefix) {
            throw BankingException("Invalid ID in saved data.");
        }
        size_t used = 0;
        int number = stoi(value.substr(1), &used);
        if (used != value.size() - 1 || number < 1) {
            throw BankingException("Invalid ID in saved data.");
        }
        return number;
    } catch (const exception&) {
        throw BankingException("Invalid ID in saved data.");
    }
}

void checkAccountValues(double balance, double value1, double value2,
                        bool savings) {
    if (!isfinite(balance) || !isfinite(value1) || !isfinite(value2) ||
        value1 < 0 || value2 < 0) {
        throw BankingException("Invalid saved account values.");
    }
    if (savings && (balance < 0 || balance < value2)) {
        throw BankingException("Invalid saved savings balance.");
    }
    if (!savings && balance < -value1) {
        throw BankingException("Invalid saved checking balance.");
    }
}

void FileStorage::save(const BankSystem& system) {
#ifdef _WIN32
    (void)_mkdir("data");
#else
    (void)mkdir("data", 0755);
#endif

    ofstream banks("data/banks.txt");
    ofstream customers("data/customers.txt");
    ofstream accounts("data/accounts.txt");
    ofstream admins("data/admins.txt");
    ofstream staffFile("data/bank_staff.txt");

    if (!banks || !customers || !accounts || !admins || !staffFile)
        throw BankingException("Cannot open data files for saving.");

    banks.precision(12);
    accounts.precision(12);

    for (Bank* bank : system.banks) {
        banks << "BANK|" << bank->bankId << '|' << bank->bankName << '|'
              << bank->phoneNumber << '|' << bank->email << '|'
              << bank->address << '|' << bank->defaultInterestRate << '|'
              << bank->defaultTransactionFee << '|' << bank->status << '\n';
    }
    admins << "ADMIN|" << system.admin.adminName << '|'
           << system.admin.password << '\n';
    for (BankStaff* staff : system.staffMembers) {
        staffFile << "STAFF|" << staff->getStaffId() << '|'
                  << staff->getBankId() << '|' << staff->getFullName() << '|'
                  << staff->getPhoneNumber() << '|' << staff->getEmail() << '|'
                  << staff->getPassword() << '\n';
    }
    for (Customer* customer : system.customers) {
        customers << "CUSTOMER|" << customer->getCustomerId() << '|'
                  << customer->getFullName() << '|' << customer->getPhoneNumber() << '|'
                  << customer->getEmail() << '|' << customer->getPassword() << '\n';
        for (Account* account : customer->getAccounts()) {
            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
            CheckingAccount* checking = dynamic_cast<CheckingAccount*>(account);
            if (savings) {
                accounts << "SAVINGS|" << account->getBankId() << '|'
                         << customer->getCustomerId() << '|'
                         << account->getAccountNumber() << '|'
                         << savings->getBalance() << '|'
                         << savings->getInterestRate() << '|'
                         << savings->getMinimumBalance() << '\n';
            } else if (checking) {
                accounts << "CHECKING|" << account->getBankId() << '|'
                         << customer->getCustomerId() << '|'
                         << account->getAccountNumber() << '|'
                         << checking->getBalance() << '|'
                         << checking->getOverdraftLimit() << '|'
                         << checking->getTransactionFee() << '\n';
            }
        }
    }
    if (!banks || !customers || !accounts || !admins || !staffFile)
        throw BankingException("Could not save all data files.");
}

void FileStorage::load(BankSystem& system) {

    ifstream bankFile("data/banks.txt");
    ifstream customerFile("data/customers.txt");
    ifstream accountFile("data/accounts.txt");
    ifstream adminFile("data/admins.txt");
    ifstream staffFile("data/bank_staff.txt");

    bool anyFile = bankFile.is_open() || customerFile.is_open() ||
                   accountFile.is_open() || adminFile.is_open() || staffFile.is_open();
    if (!anyFile) return; 
    if (!bankFile || !customerFile || !accountFile || !adminFile || !staffFile)
        throw BankingException("Some data files are missing. Restore all five files.");

    ifstream* files[] = {&bankFile, &adminFile, &staffFile, &customerFile, &accountFile};
    string allData;
    
    for (ifstream* file : files) {
        string row;
        while (getline(*file, row)) allData += row + "\n";
        if (!file->eof() && file->fail())
            throw BankingException("Could not read saved data file.");
    }
    if (allData.empty()) return;  // all files empty = first run, keep defaults
    istringstream in(allData);

    // Deleting the old data to store the new one from files.
    BankSystem loaded;
    for (Bank* bank : loaded.banks) delete bank;
    loaded.banks.clear();
    loaded.nextCustomerId = 1001;
    loaded.nextBankId = 1;
    loaded.nextStaffId = 1;

    bool hasAdmin = false;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        vector<string> parts = splitLine(line);
        if (parts.size() < 2) throw BankingException("Invalid data line.");

        if (parts[0] == "BANK") {
            if (parts.size() != 9)
                throw BankingException("Invalid BANK record.");
            string id = parts[1], name = parts[2], phone = parts[3];
            string email = parts[4], address = parts[5], status = parts[8];
            double rate = readNumber(parts[6]);
            double fee = readNumber(parts[7]);
            if ((status != "APPROVED" && status != "PENDING" &&
                 status != "REJECTED" && status != "INACTIVE") ||
                rate < 0 || fee < 0 || name.empty() ||
                loaded.findBank(id) || loaded.findBankByName(name))
                throw BankingException("Duplicate or invalid bank record.");
            int idNumber = readIdNumber(id, 'B');
            if (idNumber >= loaded.nextBankId) loaded.nextBankId = idNumber + 1;
            Bank* bank = new Bank(id, name, phone, email, address);
            bank->defaultInterestRate = rate;
            bank->defaultTransactionFee = fee;
            bank->status = status;
            loaded.banks.push_back(bank);

        } else if (parts[0] == "ADMIN") {
            if (parts.size() != 3 || parts[1] != "admin" || parts[2] != "admin" || hasAdmin)
                throw BankingException("Invalid ADMIN record.");
            loaded.admin = Admin(parts[1], parts[2]);
            hasAdmin = true;

        } else if (parts[0] == "STAFF") {
            if (parts.size() != 7 || !loaded.findBank(parts[2]))
                throw BankingException("Invalid STAFF record.");
            for (BankStaff* staff : loaded.staffMembers) {
                if (staff->getStaffId() == parts[1] ||
                    staff->getEmail() == parts[5] || staff->getPhoneNumber() == parts[4])
                    throw BankingException("Duplicate bank staff record.");
            }
            int idNumber = readIdNumber(parts[1], 'S');
            if (idNumber >= loaded.nextStaffId) loaded.nextStaffId = idNumber + 1;
            loaded.staffMembers.push_back(new BankStaff(
                parts[1], parts[2], parts[3], parts[4], parts[5], parts[6]));

        } else if (parts[0] == "CUSTOMER") {
            if (parts.size() != 6 || parts[1].empty() || parts[2].empty() ||
                parts[3].empty() || parts[4].empty() || parts[5].empty())
                throw BankingException("Invalid CUSTOMER record.");
            for (Customer* existing : loaded.customers) {
                if (existing->getCustomerId() == parts[1] ||
                    existing->getPhoneNumber() == parts[3] ||
                    existing->getEmail() == parts[4])
                    throw BankingException("Duplicate customer information.");
            }
            size_t used = 0;
            int customerNumber = 0;
            try {
                customerNumber = stoi(parts[1], &used);
            } catch (const exception&) {
                throw BankingException("Invalid customer ID in saved data.");
            }
            if (used != parts[1].size() || customerNumber < 1)
                throw BankingException("Invalid customer ID in saved data.");
            if (customerNumber >= loaded.nextCustomerId)
                loaded.nextCustomerId = customerNumber + 1;
            loaded.customers.push_back(new Customer(
                parts[1], parts[2], parts[3], parts[4], parts[5]));

        } else if (parts[0] == "SAVINGS" || parts[0] == "CHECKING") {
            if (parts.size() != 7)
                throw BankingException("Invalid account record.");
            Bank* bank = loaded.findBank(parts[1]);
            Customer* customer = loaded.findCustomer(parts[2]);
            string number = parts[3];
            if (!bank || !customer || number.empty())
                throw BankingException("Account bank/customer missing.");
            double balance = readNumber(parts[4]);
            double value1 = readNumber(parts[5]); 
            double value2 = readNumber(parts[6]);
            checkAccountValues(balance, value1, value2, parts[0] == "SAVINGS");
            for (Customer* existing : loaded.customers) {
                if (existing->findAccount(number) != nullptr)
                    throw BankingException("Duplicate account number.");
            }
            Account* account = nullptr;
            if (parts[0] == "SAVINGS")
                account = new SavingsAccount(number, balance, value1, value2,
                                             bank->getBankId());
            else
                account = new CheckingAccount(number, balance, value1, value2,
                                              bank->getBankId());
            try { bank->openAccount(customer, account); }
            catch (...) { delete account; throw; }

            // New account IDs must continue after the largest saved number.
            string numeric = number;
            size_t dash = numeric.find('-');
            if (dash != string::npos) numeric = numeric.substr(dash + 1);
            try {
                size_t used = 0;
                long idNumber = stol(numeric, &used);
                if (used == numeric.size() && idNumber >= bank->nextAccountNumber)
                    bank->nextAccountNumber = idNumber + 1;
            } catch (const exception&) {
                throw BankingException("Invalid account number in saved data.");
            }
        } else {
            throw BankingException("Unknown record type in saved data.");
        }
    }
    if (loaded.banks.empty() || !hasAdmin)
        throw BankingException("Missing bank or administrator in saved data.");
    if (!in.eof() && in.fail())
        throw BankingException("Error reading saved data.");

    // Replace only after all data was loaded successfully.
    system.banks.swap(loaded.banks);
    system.staffMembers.swap(loaded.staffMembers);
    system.customers.swap(loaded.customers);
    system.admin = loaded.admin;
    system.nextCustomerId = loaded.nextCustomerId;
    system.nextBankId = loaded.nextBankId;
    system.nextStaffId = loaded.nextStaffId;
}