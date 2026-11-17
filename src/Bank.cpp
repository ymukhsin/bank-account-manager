#include "Bank.h"
#include "CheckingAccount.h"
#include "Exceptions.h"
#include "SavingsAccount.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {

vector<string> splitLine(const string& line, char delimiter) {
    vector<string> parts;
    size_t start = 0;
    size_t end;
    while ((end = line.find(delimiter, start)) != string::npos) {
        parts.push_back(line.substr(start, end - start));
        start = end + 1;
    }
    parts.push_back(line.substr(start));
    return parts;
}

double parseAmount(const string& value) {
    try {
        size_t used = 0;
        double number = stod(value, &used);
        if (used != value.size()) {
            throw BankingException("Invalid amount in saved data.");
        }
        return number;
    } catch (const exception&) {
        throw BankingException("Invalid numeric value in saved data.");
    }
}

void showCustomer(Customer* customer) {
    cout << "\nCustomer: " << customer->getFullName()
         << " (" << customer->getCustomerId() << ")\n";
    customer->listAccounts();
    cout << "Total balance: " << customer->getTotalBalance() << '\n';
}

void managerMenu(Bank& bank, const vector<Customer*>& customers) {
    while (true) {
        cout << "\n--- Bank Manager ---\n"
             << "1) View all customers\n"
             << "2) Search customer\n"
             << "3) Change savings interest rate\n"
             << "4) Change checking transaction fee\n"
             << "5) Apply savings interest\n"
             << "6) Close account\n"
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
                case 1:
                    bank.viewAllCustomers();
                    break;
                case 2: {
                string id;
                while (true) {
                    cout << "Customer ID: ";
                    if (!getline(cin, id)) return;
                    if (id.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (id.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                    Customer* customer = bank.findCustomer(id);
                    if (!customer) throw BankingException("Customer not found.");
                    showCustomer(customer);
                    break;
                }
                case 3: {
                double rate = 0;
                while (true) {
                    string input;
                    cout << "New interest rate (e.g. 0.05): ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        rate = stod(input, &used);
                        if (used != input.size() || rate < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    bank.updateAllInterestRates(rate);
                    bank.saveData("bank_data.txt");
                    cout << "Interest rate updated and saved.\n";
                    break;
                }
                case 4: {
                double fee = 0;
                while (true) {
                    string input;
                    cout << "New transaction fee: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        fee = stod(input, &used);
                        if (used != input.size() || fee < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    bank.updateAllTransactionFees(fee);
                    bank.saveData("bank_data.txt");
                    cout << "Transaction fee updated and saved.\n";
                    break;
                }
                case 5: {
                    int count = 0;
                    for (Customer* customer : customers) {
                        for (Account* account : customer->getAccounts()) {
                            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
                            if (savings) { savings->applyInterest(); ++count; }
                        }
                    }
                    bank.saveData("bank_data.txt");
                    cout << "Interest applied to " << count << " account(s) and saved.\n";
                    break;
                }
                case 6: {
                string accountNumber;
                while (true) {
                    cout << "Account number: ";
                    if (!getline(cin, accountNumber)) return;
                    if (accountNumber.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (accountNumber.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                    bank.closeAccount(accountNumber);
                    bank.saveData("bank_data.txt");
                    cout << "Account closed and saved.\n";
                    break;
                }
                default: cout << "Choose a number from 0 to 6.\n";
            }
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

void customerMenu(Bank& bank, Customer* customer) {
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
                case 1: showCustomer(customer); break;
                case 2:
                case 3: {
                string accountNumber;
                while (true) {
                    cout << "Account number: ";
                    if (!getline(cin, accountNumber)) return;
                    if (accountNumber.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (accountNumber.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
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
                        if (used != input.size() || amount < 0) {
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
                    if (choice == 2) account->deposit(amount);
                    else account->withdraw(amount);
                    bank.saveData("bank_data.txt");
                    cout << "Transaction successful and saved.\n";
                    break;
                }
                case 4: {
                double initialBalance = 0;
                while (true) {
                    string input;
                    cout << "Initial balance: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        initialBalance = stod(input, &used);
                        if (used != input.size() || initialBalance < 0) {
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
                        if (used != input.size() || minimum < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    string id = bank.openSavingsAccount(customer->getCustomerId(), initialBalance, minimum);
                    bank.saveData("bank_data.txt");
                    cout << "Savings account opened: " << id << "\n";
                    break;
                }
                case 5: {
                double initialBalance = 0;
                while (true) {
                    string input;
                    cout << "Initial balance: ";
                    if (!getline(cin, input)) return;
                    try {
                        size_t used = 0;
                        initialBalance = stod(input, &used);
                        if (used != input.size() || initialBalance < 0) {
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
                        if (used != input.size() || limit < 0) {
                            cout << "Enter a valid non-negative number.\n";
                            continue;
                        }
                        break;
                    } catch (const exception&) {
                        cout << "Enter a valid non-negative number.\n";
                    }
                }
                    string id = bank.openCheckingAccount(customer->getCustomerId(), initialBalance, limit);
                    bank.saveData("bank_data.txt");
                    cout << "Checking account opened: " << id << "\n";
                    break;
                }
                default: cout << "Choose a number from 0 to 5.\n";
            }
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}

void customerPortal(Bank& bank) {
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
                    } else if (name.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
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
                    size_t at = email.find('@');
                    size_t dot = email.rfind('.');
                    bool valid = at != string::npos && at > 0 &&
                                 email.find('@', at + 1) == string::npos &&
                                 dot != string::npos && dot > at + 1 && dot + 1 < email.size() &&
                                 email.find_first_of(" \t\n|") == string::npos;
                    if (valid) break;
                    cout << "Invalid email. Example: name@example.com\n";
                }
                string password;
                while (true) {
                    cout << "Create password: ";
                    if (!getline(cin, password)) return;
                    if (password.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (password.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                string id = bank.addCustomer(name, phone, email, password);
                bank.saveData("bank_data.txt");
                cout << "Registered! Your customer ID: " << id << "\n";
                customerMenu(bank, bank.findCustomer(id));
                return;
            } else if (choice == 2) {
                string emailOrPhone;
                while (true) {
                    cout << "Email or phone (010 + 8 digits): ";
                    if (!getline(cin, emailOrPhone)) return;
                    if (emailOrPhone.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (emailOrPhone.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                string password;
                while (true) {
                    cout << "Password: ";
                    if (!getline(cin, password)) return;
                    if (password.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (password.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                // letting users type either the full number or just its last 8 digits.
                if (emailOrPhone.size() == 8 &&
                    emailOrPhone.find_first_not_of("0123456789") == string::npos) {
                    emailOrPhone = "010" + emailOrPhone;
                }
                Customer* customer = bank.loginCustomer(emailOrPhone, password);
                customerMenu(bank, customer);
                return;
            } else cout << "Choose 0, 1 or 2.\n";
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}
} 

Bank::Bank(string name, string phone,
           string mail, string location)
    : bankName(name),
      phoneNumber(phone),
      email(mail),
      address(location) {}

Bank::~Bank() {
    for (Customer* customer : customers) {
        delete customer;
    }
}

string Bank::generateCustomerId() {
    return to_string(nextCustomerId++);
}

string Bank::generateAccountNumber() {
    return to_string(nextAccountNumber++);
}

string Bank::addCustomer(string name, string phone,
                         string email, string password) {
    for (Customer* customer : customers) {
        if (customer->getEmail() == email || customer->getPhoneNumber() == phone) {
            throw BankingException("Email or phone number is already registered.");
        }
    }
    string id = generateCustomerId();
    if (findCustomer(id) != nullptr) {
        throw BankingException("Generated customer ID collision.");
    }

    customers.push_back(
        new Customer(id, name, phone, email, password));
    return id;
}

Customer* Bank::findCustomer(string customerId) {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == customerId) {
            return customer;
        }
    }
    return nullptr;
}

Customer* Bank::loginCustomer(string emailOrPhone, string password) {
    for (Customer* customer : customers) {
        bool matchesContact =
            customer->getEmail() == emailOrPhone ||
            customer->getPhoneNumber() == emailOrPhone;

        if (matchesContact && customer->verifyPassword(password)) {
            return customer;
        }
    }

    throw BankingException("Invalid customer credentials.");
}

void Bank::openAccount(string customerId, Account* account) {
    Customer* customer = findCustomer(customerId);
    if (customer == nullptr) {
        throw BankingException("Customer not found.");
    }
    if (account == nullptr) {
        throw BankingException("Cannot open a null account.");
    }

    for (Customer* existing : customers) {
        if (existing->findAccount(account->getAccountNumber()) != nullptr) {
            throw BankingException("Duplicate account number.");
        }
    }

    customer->addAccount(account);
}

string Bank::openSavingsAccount(string customerId, double initialBalance,
                                double minimum) {
    if (findCustomer(customerId) == nullptr) {
        throw BankingException("Customer not found.");
    }

    if (initialBalance < 0 || minimum < 0) {
        throw InvalidAmountException(
            "Initial and minimum balances must be non-negative."
        );
    }

    if (initialBalance < minimum) {
        throw InvalidAmountException(
            "Initial balance cannot be less than the minimum balance."
        );
    }

    string accountNumber = generateAccountNumber();

    Account* account = new SavingsAccount(
        accountNumber, initialBalance, defaultInterestRate, minimum);
    try {
        openAccount(customerId, account);
    } catch (...) {
        delete account;
        throw;
    }

    return accountNumber;
}

string Bank::openCheckingAccount(string customerId, double initialBalance,
                                 double limit) {
    if (findCustomer(customerId) == nullptr) {
        throw BankingException("Customer not found.");
    }

    string accountNumber = generateAccountNumber();
    Account* account = new CheckingAccount(
        accountNumber, initialBalance, limit, defaultTransactionFee);
    try {
        openAccount(customerId, account);
    } catch (...) {
        delete account;
        throw;
    }

    return accountNumber;
}

void Bank::closeAccount(string accountNumber) {
    for (Customer* customer : customers) {
        Account* account = customer->findAccount(accountNumber);

        if (account != nullptr) {

            // checking balance before closing an account. It should be 0
            if (account->getBalance() != 0) {
                throw BankingException(
                    "Account balance must be zero before closing."
                );
            }

            customer->removeAccount(accountNumber);
            return;
        }
    }
    throw AccountNotFoundException();
}

Account* Bank::findAccount(string accountNumber) {
    for (Customer* customer : customers) {
        Account* account = customer->findAccount(accountNumber);
        if (account != nullptr) {
            return account;
        }
    }
    throw AccountNotFoundException();
}

void Bank::deposit(string accountNumber, double amount) {
    findAccount(accountNumber)->deposit(amount);
}

void Bank::withdraw(string accountNumber, double amount) {
    findAccount(accountNumber)->withdraw(amount);
}

void Bank::viewAllCustomers() {
    if (customers.empty()) {
        cout << "No customers found.\n";
        return;
    }

    for (Customer* customer : customers) {
        cout << "\033[1;36m"
            << "\n----------------------Customer ID: " << customer->getCustomerId() << "----------------------" 
            << "\033[0m" << '\n' 
             << "Name: " << customer->getFullName() << '\n'
             << "Phone: " << customer->getPhoneNumber() << '\n'
             << "Email: " << customer->getEmail() << '\n';
        customer->listAccounts();
        cout << "Total balance: " << customer->getTotalBalance() << '\n';
    }
}

void Bank::updateAllInterestRates(double newRate) {
    if (newRate < 0.0) {
        throw InvalidAmountException("Interest rate cannot be negative.");
    }

    defaultInterestRate = newRate;
    for (Customer* customer : customers) {
        for (Account* account : customer->getAccounts()) {
            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
            if (savings != nullptr) {
                savings->setInterestRate(newRate);
            }
        }
    }
}

void Bank::updateAllTransactionFees(double newFee) {
    if (newFee < 0.0) {
        throw InvalidAmountException("Transaction fee cannot be negative.");
    }

    defaultTransactionFee = newFee;
    for (Customer* customer : customers) {
        for (Account* account : customer->getAccounts()) {
            CheckingAccount* checking = dynamic_cast<CheckingAccount*>(account);
            if (checking != nullptr) {
                checking->setTransactionFee(newFee);
            }
        }
    }
}

void Bank::saveData(string filePath) {
    ofstream out(filePath);
    if (!out) {
        throw BankingException("Unable to open data file for writing.");
    }

    out.precision(10);
    out << "BANK|" << bankName << '|' << phoneNumber << '|'
    << email << '|' << address << '|'
    << defaultInterestRate << '|'
    << defaultTransactionFee << '\n';

    for (Customer* customer : customers) {
        out << "CUSTOMER|" << customer->getCustomerId() << '|'
            << customer->getFullName() << '|'
            << customer->getPhoneNumber() << '|'
            << customer->getEmail() << '|'
            << customer->getPassword() << '\n';

        for (Account* account : customer->getAccounts()) {
            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
            if (savings != nullptr) {
                out << "SAVINGS|" << savings->getAccountNumber() << '|'
                    << savings->getBalance() << '|'
                    << savings->getInterestRate() << '|'
                    << savings->getMinimumBalance() << '\n';
                continue;
            }

            CheckingAccount* checking = dynamic_cast<CheckingAccount*>(account);
            if (checking != nullptr) {
                out << "CHECKING|" << checking->getAccountNumber() << '|'
                    << checking->getBalance() << '|'
                    << checking->getOverdraftLimit() << '|'
                    << checking->getTransactionFee() << '\n';
            }
        }
    }

    if (!out) {
        throw BankingException("Failed to write data file.");
    }
}

void Bank::loadData(string filePath) {
    ifstream in(filePath);
    if (!in.is_open()) {
        return;
    }

    string line;
    if (!getline(in, line)) {
        throw BankingException("Data file is empty.");
    }

    vector<string> header = splitLine(line, '|');
    if (header.size() != 7 || header[0] != "BANK") 
    {
        throw BankingException("Invalid bank record.");
    }

    for (Customer* customer : customers) {
        delete customer;
    }
    customers.clear();

    nextCustomerId = 1001;
    nextAccountNumber = 10000001;

    phoneNumber = header[2];
    email = header[3];
    address = header[4];
    double loadedRate = parseAmount(header[5]);
    double loadedFee = parseAmount(header[6]);

    if (loadedRate < 0 || loadedFee < 0) {
        throw BankingException("Invalid default bank settings.");
    }

    defaultInterestRate = loadedRate;
    defaultTransactionFee = loadedFee;

    Customer* currentCustomer = nullptr;

    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> parts = splitLine(line, '|');
        if (parts.size() < 2) {
            throw BankingException("Invalid data record.");
        }

        if (parts[0] == "CUSTOMER") {
            if (parts.size() != 6) {
                throw BankingException("Invalid customer record.");
            }

            Customer* customer = new Customer(
                parts[1], parts[2], parts[3], parts[4], parts[5]);
            if (findCustomer(parts[1]) != nullptr) {
                delete customer;
                throw BankingException("Duplicate customer ID in data file.");
            }

            try {
                int loadedId = stoi(parts[1]);
                if (loadedId >= nextCustomerId) {
                    nextCustomerId = loadedId + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric customer IDs.
            }

            customers.push_back(customer);
            currentCustomer = customer;
        } else if (parts[0] == "SAVINGS") {
            if (parts.size() != 5 || currentCustomer == nullptr) {
                throw BankingException("Invalid savings account record.");
            }

            try {
                long loadedNumber = stol(parts[1]);
                if (loadedNumber >= nextAccountNumber) {
                    nextAccountNumber = loadedNumber + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric account numbers.
            }

            currentCustomer->addAccount(
                new SavingsAccount(parts[1],
                                   parseAmount(parts[2]),
                                   parseAmount(parts[3]),
                                   parseAmount(parts[4])));
        } else if (parts[0] == "CHECKING") {
            if (parts.size() != 5 || currentCustomer == nullptr) {
                throw BankingException("Invalid checking account record.");
            }

            try {
                long loadedNumber = stol(parts[1]);
                if (loadedNumber >= nextAccountNumber) {
                    nextAccountNumber = loadedNumber + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric account numbers.
            }

            currentCustomer->addAccount(
                new CheckingAccount(parts[1],
                                    parseAmount(parts[2]),
                                    parseAmount(parts[3]),
                                    parseAmount(parts[4])));
        } else {
            throw BankingException("Unknown data record.");
        }
    }
}

void Bank::run() {
    const string managerPassword = "admin";
    while (true) {
        cout << "\n=== DEU Bank ===\n"
             << "1) Bank Manager Login\n"
             << "2) Customer Portal\n"
             << "0) Exit\n";
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
                string enteredBankName;
                while (true) {
                    cout << "Bank name: ";
                    if (!getline(cin, enteredBankName)) return;
                    if (enteredBankName.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else { break; }
                }
                string enteredPassword;
                while (true) {
                    cout << "Admin password: ";
                    if (!getline(cin, enteredPassword)) return;
                    if (enteredPassword.find_first_not_of(" \t\n") == string::npos) {
                        cout << "Cannot be empty or spaces only.\n";
                    } else if (enteredPassword.find('|') != string::npos) {
                        cout << "Please do not use the | character.\n";
                    } else { break; }
                }
                if (enteredBankName != "deu" || enteredPassword != managerPassword) {
                    cout << "Incorrect bank name or password.\n";
                } else managerMenu(*this, customers);
            } else if (choice == 2) {
                customerPortal(*this);
            } else cout << "Choose 0, 1 or 2.\n";
        } catch (const exception& e) { cout << "Error: " << e.what() << '\n'; }
    }
}
