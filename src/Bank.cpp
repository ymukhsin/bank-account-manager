#include "Bank.h"

#include "CheckingAccount.h"
#include "Exceptions.h"
#include "SavingsAccount.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

namespace {

vector<string> splitLine(const string& line, char delimiter) {
    vector<string> fields;
    string::size_type start = 0;

    while (true) {
        string::size_type position = line.find(delimiter, start);
        if (position == string::npos) {
            fields.push_back(line.substr(start));
            break;
        }

        fields.push_back(line.substr(start, position - start));
        start = position + 1;
    }

    return fields;
}

double parseAmount(const string& value) {
    try {
        return stod(value);
    } catch (const exception&) {
        throw BankingException("Invalid numeric value in data file.");
    }
}

void printMenu() {
    cout << "\n========================================\n"
         << "           BANK ACCOUNT MANAGER\n"
         << "========================================\n"
         << " 1) Add Customer\n"
         << " 2) Open Savings Account\n"
         << " 3) Open Checking Account\n"
         << " 4) Deposit\n"
         << " 5) Withdraw\n"
         << " 6) View Customer Accounts & Total Balance\n"
         << " 7) Apply Interest (Savings)\n"
         << " 8) Close Account\n"
         << " 0) Save & Exit\n"
         << "----------------------------------------\n";
}

bool readInt(const string& prompt, int& value) {
    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail()) {
            cin.ignore(10000, '\n');
            return true;
        }
        if (cin.eof()) {
            return false;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a whole number.\n";
    }
}

bool readDouble(const string& prompt, double& value) {
    while (true) {
        cout << prompt;
        cin >> value;

        if (!cin.fail()) {
            cin.ignore(10000, '\n');
            return true;
        }
        if (cin.eof()) {
            return false;
        }

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

bool readLine(const string& prompt, string& value) {
    cout << prompt;
    if (!getline(cin, value)) {
        return false;
    }
    return true;
}

bool readRequiredLine(const string& prompt, string& value) {
    while (true) {
        if (!readLine(prompt, value)) {
            return false;
        }
        if (!value.empty()) {
            return true;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}

}  // namespace

Bank::Bank(string name, string phone,
           string mail, string location)
    : bankName(move(name)),
      phoneNumber(move(phone)),
      email(move(mail)),
      address(move(location)) {}

Bank::~Bank() {
    for (Customer* customer : customers) {
        delete customer;
    }
}

void Bank::addCustomer(string id, string name,
                       string phone, string email) {
    if (findCustomer(id) != nullptr) {
        throw BankingException("Duplicate customer ID.");
    }
    customers.push_back(
        new Customer(move(id), move(name),
                     move(phone), move(email)));
}

Customer* Bank::findCustomer(string customerId) {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == customerId) {
            return customer;
        }
    }
    return nullptr;
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

void Bank::closeAccount(string accountNumber) {
    for (Customer* customer : customers) {
        if (customer->findAccount(accountNumber) != nullptr) {
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

void Bank::saveData(string filePath) {
    ofstream out(filePath);
    if (!out) {
        throw BankingException("Unable to open data file for writing.");
    }

    out.precision(10);
    out << "BANK|" << bankName << '|' << phoneNumber << '|'
        << email << '|' << address << '\n';

    for (Customer* customer : customers) {
        out << "CUSTOMER|" << customer->getCustomerId() << '|'
            << customer->getFullName() << '|'
            << customer->getPhoneNumber() << '|'
            << customer->getEmail() << '\n';

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
    if (header.size() != 5 || header[0] != "BANK") {
        throw BankingException("Invalid bank record.");
    }

    for (Customer* customer : customers) {
        delete customer;
    }
    customers.clear();

    bankName = header[1];
    phoneNumber = header[2];
    email = header[3];
    address = header[4];

    Customer* currentCustomer = nullptr;

    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> fields = splitLine(line, '|');
        if (fields.size() < 2) {
            throw BankingException("Invalid data record.");
        }

        if (fields[0] == "CUSTOMER") {
            if (fields.size() != 5) {
                throw BankingException("Invalid customer record.");
            }

            Customer* customer =
                new Customer(fields[1], fields[2], fields[3], fields[4]);
            if (findCustomer(fields[1]) != nullptr) {
                delete customer;
                throw BankingException("Duplicate customer ID in data file.");
            }

            customers.push_back(customer);
            currentCustomer = customer;
        } else if (fields[0] == "SAVINGS") {
            if (fields.size() != 5 || currentCustomer == nullptr) {
                throw BankingException("Invalid savings account record.");
            }

            currentCustomer->addAccount(
                new SavingsAccount(fields[1],
                                   parseAmount(fields[2]),
                                   parseAmount(fields[3]),
                                   parseAmount(fields[4])));
        } else if (fields[0] == "CHECKING") {
            if (fields.size() != 5 || currentCustomer == nullptr) {
                throw BankingException("Invalid checking account record.");
            }

            currentCustomer->addAccount(
                new CheckingAccount(fields[1],
                                    parseAmount(fields[2]),
                                    parseAmount(fields[3]),
                                    parseAmount(fields[4])));
        } else {
            throw BankingException("Unknown data record.");
        }
    }
}

void Bank::run() {
    int choice = -1;

    while (choice != 0) {
        printMenu();
        if (!readInt("Select an option: ", choice)) {
            cout << "\nInput closed.\n";
            return;
        }

        try {
            switch (choice) {
                case 1: {
                    string id;
                    string name;
                    string phone;
                    string email;

                    if (!readRequiredLine("Customer ID: ", id) ||
                        !readRequiredLine("Full name: ", name) ||
                        !readRequiredLine("Phone number: ", phone) ||
                        !readRequiredLine("Email: ", email)) {
                        return;
                    }

                    addCustomer(id, name, phone, email);
                    cout << "Customer added.\n";
                    break;
                }
                case 2: {
                    string customerId;
                    string accountNumber;
                    double initialBalance = 0.0;
                    double rate = 0.0;
                    double minimum = 0.0;

                    if (!readRequiredLine("Customer ID: ", customerId) ||
                        !readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Initial balance: ", initialBalance) ||
                        !readDouble("Interest rate (e.g. 0.05): ", rate) ||
                        !readDouble("Minimum balance: ", minimum)) {
                        return;
                    }

                    if (initialBalance < 0.0) {
                        throw InvalidAmountException(
                            "Initial balance cannot be negative.");
                    }
                    if (rate < 0.0) {
                        throw InvalidAmountException(
                            "Interest rate cannot be negative.");
                    }
                    if (minimum < 0.0) {
                        throw InvalidAmountException(
                            "Minimum balance cannot be negative.");
                    }

                    Account* account = new SavingsAccount(
                        accountNumber, initialBalance, rate, minimum);
                    try {
                        openAccount(customerId, account);
                    } catch (...) {
                        delete account;
                        throw;
                    }

                    cout << "Savings account opened.\n";
                    break;
                }
                case 3: {
                    string customerId;
                    string accountNumber;
                    double initialBalance = 0.0;
                    double limit = 0.0;
                    double fee = 0.0;

                    if (!readRequiredLine("Customer ID: ", customerId) ||
                        !readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Initial balance: ", initialBalance) ||
                        !readDouble("Overdraft limit: ", limit) ||
                        !readDouble("Transaction fee: ", fee)) {
                        return;
                    }

                    if (initialBalance < 0.0) {
                        throw InvalidAmountException(
                            "Initial balance cannot be negative.");
                    }
                    if (limit < 0.0) {
                        throw InvalidAmountException(
                            "Overdraft limit cannot be negative.");
                    }
                    if (fee < 0.0) {
                        throw InvalidAmountException(
                            "Transaction fee cannot be negative.");
                    }

                    Account* account = new CheckingAccount(
                        accountNumber, initialBalance, limit, fee);
                    try {
                        openAccount(customerId, account);
                    } catch (...) {
                        delete account;
                        throw;
                    }

                    cout << "Checking account opened.\n";
                    break;
                }
                case 4: {
                    string accountNumber;
                    double amount = 0.0;
                    if (!readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Deposit amount: ", amount)) {
                        return;
                    }

                    deposit(accountNumber, amount);
                    cout << "Deposit successful.\n";
                    break;
                }
                case 5: {
                    string accountNumber;
                    double amount = 0.0;
                    if (!readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Withdrawal amount: ", amount)) {
                        return;
                    }

                    withdraw(accountNumber, amount);
                    cout << "Withdrawal successful.\n";
                    break;
                }
                case 6: {
                    string customerId;
                    if (!readRequiredLine("Customer ID: ", customerId)) {
                        return;
                    }

                    Customer* customer = findCustomer(customerId);
                    if (customer == nullptr) {
                        throw BankingException("Customer not found.");
                    }

                    cout << "\nCustomer: " << customer->getFullName()
                              << " (" << customer->getCustomerId() << ")\n";
                    customer->listAccounts();
                    cout << "Total balance: "
                              << customer->getTotalBalance() << '\n';
                    break;
                }
                case 7: {
                    string accountNumber;
                    if (!readRequiredLine("Savings account number: ",
                                          accountNumber)) {
                        return;
                    }

                    Account* account = findAccount(accountNumber);
                    SavingsAccount* savings =
                        dynamic_cast<SavingsAccount*>(account);
                    if (savings == nullptr) {
                        throw BankingException(
                            "Selected account is not a savings account.");
                    }

                    savings->applyInterest();
                    cout << "Interest applied.\n";
                    break;
                }
                case 8: {
                    string accountNumber;
                    if (!readRequiredLine("Account number: ", accountNumber)) {
                        return;
                    }

                    closeAccount(accountNumber);
                    cout << "Account closed.\n";
                    break;
                }
                case 0:
                    cout << "Saving and exiting...\n";
                    return;
                default:
                    cout << "Invalid menu option. Choose 0-8.\n";
                    break;
            }
        } catch (const BankingException& e) {
            cout << "Error: " << e.what() << '\n';
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << '\n';
        }
    }
}
