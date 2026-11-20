#include "BankSystem.h"
#include "FileStorage.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "Exceptions.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>

using namespace std;

// helper functions for the banking menus.

// checking the basic email format.
bool isValidEmail(const string& email) {
    size_t at = email.find('@');
    size_t dot = email.rfind('.');
    return at != string::npos && at > 0 &&
           email.find('@', at + 1) == string::npos &&
           dot != string::npos && dot > at + 1 && dot + 1 < email.size() &&
           email.find_first_of(" \t\r\n") == string::npos;
}

void showCustomer(BankSystem& system, Customer* customer) {
    cout << "\n========== CUSTOMER PROFILE ==========\n"
         << "Customer ID: " << customer->getCustomerId() << '\n'
         << "Name: " << customer->getFullName() << '\n'
         << "Phone: " << customer->getPhoneNumber() << '\n'
         << "Email: " << customer->getEmail() << '\n';
    int number = 0;
    cout << "\n============= MY ACCOUNTS ============\n";
    for (Account* account : customer->getAccounts()) {
        Bank* bank = system.findBank(account->getBankId());
        cout << "\nACCOUNT #" << ++number << " - " << account->getType() << '\n';
        cout << "Bank: " << (bank ? bank->getBankName() : "Unknown bank")
             << " (" << account->getBankId() << ")\n";
        account->display();
    }
    if (number == 0) cout << "No bank accounts yet.\n";
    cout << "\n============= SUMMARY ===============\n"
         << "Total accounts: " << number << '\n'
         << "Total balance across all banks: " << customer->getTotalBalance() << '\n';
}

void showBankCustomer(Customer* customer, Bank& bank) {
    cout << "\nCustomer: " << customer->getFullName()
         << " (" << customer->getCustomerId() << ")\n";
    customer->listAccounts(bank.getBankId());
    cout << "Balance at " << bank.getBankName() << ": "
         << customer->getTotalBalance(bank.getBankId()) << '\n';
}

Bank* chooseBank(BankSystem& system, bool requireActive = true);

void bankStaffMenu(BankSystem& system, Bank& bank) {
    while (true) {
        if (!bank.isActive()) {
            cout << "This bank is not approved or active.\n";
            return;
        }
        cout << "\n--- " << bank.getBankName() << " - "
             << "BANK STAFF ---\n"
             << "1) View all customers\n"
             << "2) Search customer\n"
             << "3) Change savings interest rate\n"
             << "4) Change checking transaction fee\n"
             << "5) Close account\n";
        
        cout << "0) Logout\nChoice: ";
        string input;
        if (!getline(cin, input)) return;
        int choice = -1;
        try {
            size_t used = 0;
            choice = stoi(input, &used);
            if (used != input.size()) choice = -1;
        } catch (const exception&) { choice = -1; }
        if (choice == 0) return;
        try {
            if (choice == 1) {
                bank.viewAllCustomers();
            } else if (choice == 2) {
                string id;
                cout << "Customer ID: ";
                if (!getline(cin, id)) return;
                Customer* customer = bank.findCustomer(id);
                if (!customer) throw BankingException("Customer not found at this bank.");
                showBankCustomer(customer, bank);
            } else if (choice == 3 || choice == 4) {
                string value;
                cout << (choice == 3 ? "New interest rate (e.g. 0.05): " : "New transaction fee: ");
                if (!getline(cin, value)) return;
                double number = 0;
                try {
                    size_t used = 0;
                    number = stod(value, &used);
                    if (used != value.size() || !isfinite(number) || number < 0)
                        throw BankingException("Enter a valid non-negative number.");
                } catch (const exception&) {
                    cout << "Enter a valid non-negative number.\n";
                    continue;
                }
                if (choice == 3) {
                    bank.updateAllInterestRates(number);
                    system.save();
                    cout << "Interest rate updated, interest applied, and data saved successfully.\n";
                } else {
                    bank.updateAllTransactionFees(number);
                    system.save();
                    cout << "Transaction fee updated and saved successfully.\n";
                }
            } else if (choice == 5) {
                string accountNumber;
                cout << "Account number: ";
                if (!getline(cin, accountNumber)) return;
                bank.closeAccount(accountNumber);
                system.save();
                cout << "Account closed and saved successfully.\n";
            } else {
                cout << "Choose a number from 0 to 5.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}

void customerMenu(BankSystem& system, Customer* customer) {
    while (true) {
        cout << "\n--- Welcome, " << customer->getFullName() << " ---\n"
             << "1) View my accounts\n"
             << "2) Deposit\n"
             << "3) Withdraw\n"
             << "4) Open savings account\n"
             << "5) Open checking account\n"
             << "0) Logout\n";
        string input;
        cout << "Choice: ";
        if (!getline(cin, input)) return;
        int choice = -1;
        try {
            size_t used = 0;
            choice = stoi(input, &used);
            if (used != input.size()) choice = -1;
        } catch (const exception&) { choice = -1; }
        if (choice == 0) return;
        try {
            switch (choice) {
                case 1: showCustomer(system, customer); break;
                case 2:
                case 3: {
                string accountNumber;
                while (true) {
                    cout << "Account number: ";
                    if (!getline(cin, accountNumber)) return;
                    if (accountNumber.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                double amount = 0;
                while (true) {
                    string input;
                    cout << "Amount: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        amount = stod(input, &used);
                        if (used != input.size() || !isfinite(amount) || amount < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    Account* account = customer->findAccount(accountNumber);
                    if (!account) throw BankingException("This account does not belong to you.");
                    Bank* accountBank = system.findBank(account->getBankId());
                    if (!accountBank || !accountBank->isActive())
                        throw BankingException("Transactions are unavailable for this bank.");
                    if (choice == 2) account->deposit(amount);
                    else account->withdraw(amount);
                    system.save();
                    cout << "Transaction successful and saved.\n";
                    break;
                }
                case 4: {
                Bank* bank = chooseBank(system);
                if (!bank) break;
                double initialBalance = 0;
                while (true) {
                    string input;
                    cout << "Initial balance: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        initialBalance = stod(input, &used);
                        if (used != input.size() || !isfinite(initialBalance) || initialBalance < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                double minimum = 0;
                while (true) {
                    string input;
                    cout << "Minimum balance: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        minimum = stod(input, &used);
                        if (used != input.size() || !isfinite(minimum) || minimum < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    string id = bank->openSavingsAccount(customer, initialBalance, minimum);
                    system.save();
                    cout << "Savings account opened: " << id << "\n";
                    break;
                }
                case 5: {
                Bank* bank = chooseBank(system);
                if (!bank) break;
                double initialBalance = 0;
                while (true) {
                    string input;
                    cout << "Initial balance: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        initialBalance = stod(input, &used);
                        if (used != input.size() || !isfinite(initialBalance) || initialBalance < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                double limit = 0;
                while (true) {
                    string input;
                    cout << "Overdraft limit: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        limit = stod(input, &used);
                        if (used != input.size() || !isfinite(limit) || limit < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    string id = bank->openCheckingAccount(customer, initialBalance, limit);
                    system.save();
                    cout << "Checking account opened: " << id << "\n";
                    break;
                }
                default: cout << "Choose a number from 0 to 5.\n";
            }
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

void customerPortal(BankSystem& system) {
    while (true) {
        cout << "\n--- Customer Portal ---\n"
             << "1) Register\n2) Login\n0) Back\n";
        string input;
        cout << "Choice: ";
        if (!getline(cin, input)) return;
        int choice = -1;
        try {
            size_t used = 0;
            choice = stoi(input, &used);
            if (used != input.size()) choice = -1;
        } catch (const exception&) { choice = -1; }
        if (choice == 0) return;
        try {
            if (choice == 1) {
                string name;
                while (true) {
                    cout << "Full name: ";
                    if (!getline(cin, name)) return;
                    if (name.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                // A name must contain at least one letter, not only punctuation or digits.
                bool validName = false;
                for (char c : name) if (isalpha(static_cast<unsigned char>(c))) validName = true;
                if (!validName) { cout << "Please enter a valid name.\n"; continue; }

                string phone;
                while (true) {
                    string lastEight;
                    cout << "Phone: 010";
                    if (!getline(cin, lastEight)) return;
                    bool valid = lastEight.size() == 8;
                    for (char c : lastEight) if (!isdigit(static_cast<unsigned char>(c))) valid = false;
                    if (valid) { phone = "010" + lastEight; break; }
                    cout << "Enter exactly 8 digits after 010 (no spaces or dashes).\n";
                }

                string email;
                while (true) {
                    cout << "Email: ";
                    if (!getline(cin, email)) return;
                    if (isValidEmail(email)) break;
                    cout << "Invalid email. Example: name@example.com\n";
                }
                string password;
                while (true) {
                    cout << "Create password: ";
                    if (!getline(cin, password)) return;
                    if (password.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                Customer* customer = system.registerCustomer(name, phone, email, password);
                system.save();
                cout << "Registered! Your customer ID: " << customer->getCustomerId() << "\n";
                customerMenu(system, customer);
                return;
            } else if (choice == 2) {
                string emailOrPhone;
                while (true) {
                    cout << "Email or phone (010 + 8 digits): ";
                    if (!getline(cin, emailOrPhone)) return;
                    if (emailOrPhone.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                string password;
                while (true) {
                    cout << "Password: ";
                    if (!getline(cin, password)) return;
                    if (password.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                // letting users type either the full number or just its last 8 digits.
                if (emailOrPhone.size() == 8 &&
                    emailOrPhone.find_first_not_of("0123456789") == string::npos) {
                    emailOrPhone = "010" + emailOrPhone;
                }
                Customer* customer = system.loginCustomer(emailOrPhone, password);
                customerMenu(system, customer);
                return;
            } else cout << "Choose 0, 1 or 2.\n";
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

// Customers select a bank only when opening an account.
Bank* chooseBank(BankSystem& system, bool requireActive) {
    if (requireActive) {
        cout << "\n========== APPROVED BANKS ==========\n";
        bool foundApproved = false;
        for (Bank* bank : system.getBanks()) {
            if (bank->isActive()) {
                cout << bank->getBankName() << " (" << bank->getBankId() << ")\n";
                foundApproved = true;
            }
        }
        if (!foundApproved) {
            cout << "No approved banks available.\n";
            return nullptr;
        }
    } else {
        system.showBanks();
    }
    string name;
    cout << "Enter bank name: ";
    if (!getline(cin, name)) return nullptr;
    Bank* bank = system.findBankByName(name);
    if (!bank) {
        cout << "Bank not found. Enter a registered bank name.\n";
        return nullptr;
    }
    if (requireActive && !bank->isActive()) {
        cout << "This bank is not approved or active.\n";
        return nullptr;
    }
    return bank;
}

void registerStaff(BankSystem& system) {
    // Staff first provide their information, and then register a bank.
    string staffName, staffPhone, staffEmail, staffPassword;
    string bankName, bankPhone, bankEmail, bankAddress;

    while (true) {
        cout << "Staff full name: ";
        if (!getline(cin, staffName)) return;
        bool hasLetter = false;
        for (char c : staffName) {
            if (isalpha(static_cast<unsigned char>(c))) hasLetter = true;
        }
        if (hasLetter) break;
        cout << "Enter a valid name (not empty or spaces only).\n";
    }

    while (true) {
        string eight;
        cout << "Staff phone: 010";
        if (!getline(cin, eight)) return;
        bool valid = eight.size() == 8;
        for (char c : eight) if (!isdigit(static_cast<unsigned char>(c))) valid = false;
        if (valid) { staffPhone = "010" + eight; break; }
        cout << "Enter exactly 8 digits after 010.\n";
    }

    while (true) {
        cout << "Staff email: ";
        if (!getline(cin, staffEmail)) return;
        if (isValidEmail(staffEmail)) break;
        cout << "Invalid email. Example: staff@example.com\n";
    }
    while (true) {
        cout << "Create staff password: ";
        if (!getline(cin, staffPassword)) return;
        if (staffPassword.find_first_not_of(" \t\r\n") != string::npos) break;
        cout << "Password cannot be empty or spaces only.\n";
    }

    cout << "\n--- Register your bank ---\n";
    while (true) {
        cout << "Bank name: ";
        if (!getline(cin, bankName)) return;
        if (bankName.find_first_not_of(" \t\r\n") != string::npos) break;
        cout << "Bank name cannot be empty.\n";
    }
    while (true) {
        string eight;
        cout << "Bank contact phone: 010";
        if (!getline(cin, eight)) return;
        bool valid = eight.size() == 8;
        for (char c : eight) if (!isdigit(static_cast<unsigned char>(c))) valid = false;
        if (valid) { bankPhone = "010" + eight; break; }
        cout << "Enter exactly 8 digits after 010.\n";
    }
    while (true) {
        cout << "Bank email: ";
        if (!getline(cin, bankEmail)) return;
        if (isValidEmail(bankEmail)) break;
        cout << "Invalid email. Example: bank@example.com\n";
    }
    while (true) {
        cout << "Bank address: ";
        if (!getline(cin, bankAddress)) return;
        if (bankAddress.find_first_not_of(" \t\r\n") != string::npos) break;
        cout << "Address cannot be empty or spaces only.\n";
    }

    BankStaff* staff = system.registerBankAndStaff(
        bankName, bankPhone, bankEmail, bankAddress,
        staffName, staffPhone, staffEmail, staffPassword);
    system.save();
    cout << "Registered! Staff ID: " << staff->getStaffId()
         << " | Bank ID: " << staff->getBankId() << '\n';
    cout << "Your bank registration is PENDING admin approval.\n";
    cout << "You can log in after approval.\n";
}

void staffLogin(BankSystem& system) {
    string contact, password;
    cout << "Staff email or phone: ";
    if (!getline(cin, contact)) return;
    if (contact.size() == 8 &&
        contact.find_first_not_of("0123456789") == string::npos) {
        contact = "010" + contact;
    }
    cout << "Staff password: ";
    if (!getline(cin, password)) return;
    BankStaff* staff = system.loginStaff(contact, password);
    Bank* bank = system.findBank(staff->getBankId());
    if (!bank) throw BankingException("The staff member's bank is missing.");
    if (!bank->isActive()) throw BankingException("Your bank is not approved or active.");
    cout << "Welcome, " << staff->getFullName() << " (" << bank->getBankName() << ")!\n";
    bankStaffMenu(system, *bank);
}

void adminLogin(BankSystem& system) {
    string name, password;
    cout << "Admin username: ";
    if (!getline(cin, name)) return;
    cout << "Admin password: ";
    if (!getline(cin, password)) return;
    if (!system.getAdmin().login(name, password)) {
        cout << "Incorrect admin username or password.\n";
        return;
    }
    while (true) {
        cout << "\n--- ADMIN: ALL BANKS ---\n"
             << "1) View registered banks\n"
             << "2) View all customers\n"
             << "3) View customers for a specific bank\n"
             << "4) View all bank staff\n"
             << "5) Approve or reject bank registration\n"
             << "6) View system statistics\n"
             << "7) Deactivate a bank\n"
             << "0) Logout\nChoice: ";
        string input;
        if (!getline(cin, input)) return;
        if (input == "0") return;
        if (input == "1") {
            system.showBanks();
        } else if (input == "2") {
            cout << "\n--- ALL REGISTERED CUSTOMERS ---\n";
            if (system.getCustomers().empty()) {
                cout << "No customers registered yet.\n";
            } else {
                for (Customer* customer : system.getCustomers()) {
                    showCustomer(system, customer);
                }   
            }
        } else if (input == "3") {
            Bank* bank = chooseBank(system, false);
            if (bank) {
                cout << "\n--- CUSTOMERS AT " << bank->getBankName() << " ---\n";
                bank->viewAllCustomers();
            }
        } else if (input == "4") {
            cout << "\n--- ALL BANK STAFF ---\n";
            if (system.getStaffMembers().empty()) cout << "No bank staff registered.\n";
            for (BankStaff* staff : system.getStaffMembers()) {
                Bank* bank = system.findBank(staff->getBankId());
                cout << "Staff ID: " << staff->getStaffId()
                     << " | Name: " << staff->getFullName()
                     << " | Phone: " << staff->getPhoneNumber()
                     << " | Email: " << staff->getEmail()
                     << " | Bank: " << (bank ? bank->getBankName() : "Unknown")
                     << " | Status: " << (bank ? bank->getStatus() : "Unknown") << '\n';
            }
        } else if (input == "5") {
            cout << "\n--- PENDING BANK REGISTRATIONS ---\n";
            bool anyPending = false;
            for (Bank* bank : system.getBanks()) {
                if (bank->getStatus() == "PENDING") {
                    anyPending = true;
                    cout << bank->getBankId() << " - " << bank->getBankName()
                         << " | " << bank->getEmail() << '\n';
                }
            }
            if (!anyPending) {
                cout << "No pending registrations.\n";
                continue;
            }
            string bankName, decision;
            cout << "Bank name to review (or 0 to cancel): ";
            if (!getline(cin, bankName)) return;
            if (bankName == "0") continue;
            Bank* bank = system.findBankByName(bankName);
            if (!bank || bank->getStatus() != "PENDING") {
                cout << "No pending bank with that name.\n";
                continue;
            }
            cout << "1) Approve\n2) Reject\n0) Cancel\nChoice: ";
            if (!getline(cin, decision)) return;
            if (decision == "1") {
                bank->setStatus("APPROVED");
                system.save();
                cout << "Bank approved successfully.\n";
            } else if (decision == "2") {
                bank->setStatus("REJECTED");
                system.save();
                cout << "Bank registration rejected.\n";
            } else if (decision != "0") {
                cout << "Invalid choice.\n";
            }
        } else if (input == "6") {
            int approved = 0, pending = 0, rejected = 0, inactive = 0;
            int accountCount = 0, savingsCount = 0, checkingCount = 0;
            for (Bank* bank : system.getBanks()) {
                if (bank->getStatus() == "APPROVED") approved++;
                else if (bank->getStatus() == "PENDING") pending++;
                else if (bank->getStatus() == "REJECTED") rejected++;
                else if (bank->getStatus() == "INACTIVE") inactive++;
            }
            for (Customer* customer : system.getCustomers()) {
                for (Account* account : customer->getAccounts()) {
                    accountCount++;
                    if (account->getType() == "Savings") savingsCount++;
                    else if (account->getType() == "Checking") checkingCount++;
                }
            }
            cout << "\n--- SYSTEM STATISTICS ---\n"
                 << "Total banks: " << system.getBanks().size() << '\n'
                 << "Approved banks: " << approved << '\n'
                 << "Pending banks: " << pending << '\n'
                 << "Rejected banks: " << rejected << '\n'
                 << "Inactive banks: " << inactive << '\n'
                 << "Total customers: " << system.getCustomers().size() << '\n'
                 << "Total bank staff: " << system.getStaffMembers().size() << '\n'
                 << "Total accounts: " << accountCount << '\n'
                 << "Savings accounts: " << savingsCount << '\n'
                 << "Checking accounts: " << checkingCount << '\n';
        } else if (input == "7") {
            system.showBanks();
            string bankName, confirm;
            cout << "Bank name to deactivate (or 0 to cancel): ";
            if (!getline(cin, bankName)) return;
            if (bankName == "0") continue;
            Bank* bank = system.findBankByName(bankName);
            if (!bank || !bank->isActive()) {
                cout << "No approved, active bank with that name.\n";
                continue;
            }
            cout << "Deactivate " << bank->getBankName()
                 << "? Customers will not be able to transact at this bank. (yes/no): ";
            if (!getline(cin, confirm)) return;
            if (confirm == "yes") {
                bank->setStatus("INACTIVE");
                system.save();
                cout << "Bank deactivated successfully. Existing records are preserved.\n";
            } else cout << "Cancelled.\n";
        } else {
            cout << "Choose a number from 0 to 7.\n";
        }
    }
}

void BankSystem::run() {
    while (true) {
        cout << "\n========== DEU BANK SYSTEM ==========\n"
             << "1) Admin login\n"
             << "2) Bank staff registration\n"
             << "3) Bank staff login\n"
             << "4) Customer portal\n"
             << "0) Exit\nChoice: ";
        string input;
        if (!getline(cin, input)) return;
        try {
            if (input == "0") return;
            if (input == "1") adminLogin(*this);
            else if (input == "2") registerStaff(*this);
            else if (input == "3") staffLogin(*this);
            else if (input == "4") customerPortal(*this);
            else cout << "Choose a number from 0 to 4.\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    }
}

BankSystem::BankSystem() : admin("admin", "admin") {
    // Start with DEU as the sample bank.
    banks.push_back(new Bank("B001", "deu", "01000000000",
                             "deu@example.com", "Busan, South Korea"));
}

BankSystem::~BankSystem() {
    for (Bank* bank : banks) delete bank;
    for (BankStaff* staff : staffMembers) delete staff;
    for (Customer* customer : customers) delete customer;
}

Bank* BankSystem::findBank(string id) const {
    for (Bank* bank : banks) {
        if (bank->getBankId() == id) return bank;
    }
    return nullptr;
}

Bank* BankSystem::findBankByName(string name) const {
    // ignoring upper/lower case in bank names.
    for (char& c : name) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));

    for (Bank* bank : banks) {
        string registered = bank->getBankName();
        for (char& c : registered)
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        if (registered == name) return bank;
    }
    return nullptr;
}

BankStaff* BankSystem::loginStaff(string emailOrPhone, string pwd) const {
    for (BankStaff* staff : staffMembers) {
        if ((staff->getEmail() == emailOrPhone ||
             staff->getPhoneNumber() == emailOrPhone) &&
            staff->verifyPassword(pwd)) return staff;
    }
    throw BankingException("Incorrect staff email/phone or password.");
}

BankStaff* BankSystem::registerBankAndStaff(string bankName, string bankPhone,
                                           string bankEmail, string bankAddress,
                                           string staffName, string staffPhone,
                                           string staffEmail, string staffPassword) {
    if (findBankByName(bankName) != nullptr) {
        throw BankingException("Bank name is already registered.");
    }
    for (BankStaff* staff : staffMembers) {
        if (staff->getEmail() == staffEmail || staff->getPhoneNumber() == staffPhone) {
            throw BankingException("This staff email or phone is already registered.");
        }
    }

    // nextBankId = 11
    // B011 
    string bankId = string("B") + (nextBankId < 10 ? "00" : (nextBankId < 100 ? "0" : ""))
                    + to_string(nextBankId);
    string staffId = string("S") + (nextStaffId < 10 ? "00" : (nextStaffId < 100 ? "0" : ""))
                     + to_string(nextStaffId);
    Bank* newBank = new Bank(bankId, bankName, bankPhone, bankEmail, bankAddress);
    BankStaff* newStaff = new BankStaff(staffId, bankId, staffName,
                                       staffPhone, staffEmail, staffPassword);
    newBank->setStatus("PENDING");
    banks.push_back(newBank);
    staffMembers.push_back(newStaff);
    ++nextBankId;
    ++nextStaffId;
    return newStaff;
}

const vector<Bank*>& BankSystem::getBanks() const { return banks; }
const vector<BankStaff*>& BankSystem::getStaffMembers() const { return staffMembers; }
const Admin& BankSystem::getAdmin() const { return admin; }
void BankSystem::showBanks() const { admin.showBanks(banks); }
void BankSystem::save() const { FileStorage::save(*this); }

Customer* BankSystem::findCustomer(string id) const {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == id) return customer;
    }
    return nullptr;
}

Customer* BankSystem::registerCustomer(string name, string phone,
                                      string email, string password) {
    for (Customer* customer : customers) {
        if (customer->getEmail() == email || customer->getPhoneNumber() == phone) {
            throw BankingException("Email or phone number is already registered.");
        }
    }
    string id = to_string(nextCustomerId++);
    Customer* customer = new Customer(id, name, phone, email, password);
    customers.push_back(customer);
    return customer;
}

Customer* BankSystem::loginCustomer(string emailOrPhone, string password) const {
    for (Customer* customer : customers) {
        if ((customer->getEmail() == emailOrPhone ||
             customer->getPhoneNumber() == emailOrPhone) &&
            customer->verifyPassword(password)) return customer;
    }
    throw BankingException("Incorrect customer email/phone or password.");
}

const vector<Customer*>& BankSystem::getCustomers() const { return customers; }
