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

void printMainMenu(const string& bankName) {
    cout << "\n========================================\n"
         << "   " << bankName << "\n"
         << "========================================\n"
         << " 1) Login as Bank Manager\n"
         << " 2) Register / Login as Customer\n"
         << " 0) Save & Exit\n"
         << "----------------------------------------\n";
}

void printManagerMenu() {
    cout << "\n--- Bank Manager Menu ---\n"
         << " 1) Add New Customer\n"
         << " 2) Open Savings Account for Customer\n"
         << " 3) Open Checking Account for Customer\n"
         << " 4) View Customer Accounts & Total Balance\n"
         << " 5) Apply Interest to Savings Account\n"
         << " 6) Close Customer Account\n"
         << " 7) View All Customers & Accounts\n"
         << " 8) Update Checking Transaction Fee\n"
         << " 0) Logout to Main Menu\n"
         << "----------------------------------------\n";
}

void printCustomerPortalMenu() {
    cout << "\n--- Customer Portal ---\n"
         << " 1) Register as New Customer\n"
         << " 2) Login as Existing Customer\n"
         << " 0) Back to Main Menu\n"
         << "----------------------------------------\n";
}

void printCustomerDashboardMenu(const string& customerName) {
    cout << "\n--- Customer Dashboard: " << customerName << " ---\n"
         << " 1) View My Accounts & Total Balance\n"
         << " 2) Deposit\n"
         << " 3) Withdraw\n"
         << " 4) Open Savings Account\n"
         << " 5) Open Checking Account\n"
         << " 0) Logout to Main Menu\n"
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

Customer* readAndAddCustomer(Bank& bank) {
    string name;
    string phone;
    string email;

    if (!readRequiredLine("Full name: ", name) ||
        !readRequiredLine("Phone number: ", phone) ||
        !readRequiredLine("Email: ", email)) {
        return nullptr;
    }

    string id = bank.addCustomer(name, phone, email);
    cout << "Assigned Customer ID: " << id << '\n';

    Customer* customer = bank.findCustomer(id);
    if (customer == nullptr) {
        throw BankingException("Customer registration failed.");
    }
    return customer;
}

void viewCustomer(Customer* customer) {
    cout << "\nCustomer: " << customer->getFullName()
         << " (" << customer->getCustomerId() << ")\n";
    customer->listAccounts();
    cout << "Total balance: " << customer->getTotalBalance() << '\n';
}

bool openSavingsFor(Bank& bank, const string& customerId) {
    double initialBalance = 0.0;
    double rate = 0.0;
    double minimum = 0.0;

    if (!readDouble("Initial balance: ", initialBalance) ||
        !readDouble("Interest rate (e.g. 0.05): ", rate) ||
        !readDouble("Minimum balance: ", minimum)) {
        return false;
    }

    if (initialBalance < 0.0) {
        throw InvalidAmountException("Initial balance cannot be negative.");
    }
    if (rate < 0.0) {
        throw InvalidAmountException("Interest rate cannot be negative.");
    }
    if (minimum < 0.0) {
        throw InvalidAmountException("Minimum balance cannot be negative.");
    }

    string accountNumber = bank.openSavingsAccount(
        customerId, initialBalance, rate, minimum);
    cout << "Savings account opened. Account Number: "
         << accountNumber << '\n';
    return true;
}

bool openCheckingFor(Bank& bank, const string& customerId) {
    double initialBalance = 0.0;
    double limit = 0.0;

    if (!readDouble("Initial balance: ", initialBalance) ||
        !readDouble("Overdraft limit: ", limit)) {
        return false;
    }

    if (initialBalance < 0.0) {
        throw InvalidAmountException("Initial balance cannot be negative.");
    }
    if (limit < 0.0) {
        throw InvalidAmountException("Overdraft limit cannot be negative.");
    }

    string accountNumber = bank.openCheckingAccount(
        customerId, initialBalance, limit);
    cout << "Checking account opened. Account Number: "
         << accountNumber << '\n';
    return true;
}

void runManagerMenu(Bank& bank, double& defaultTransactionFee) {
    while (true) {
        printManagerMenu();

        int choice = -1;
        if (!readInt("Select an option: ", choice)) {
            cout << "\nInput closed.\n";
            return;
        }
        if (choice == 0) {
            return;
        }

        try {
            switch (choice) {
                case 1: {
                    Customer* customer = readAndAddCustomer(bank);
                    if (customer == nullptr) {
                        return;
                    }
                    cout << "Customer added.\n";
                    break;
                }
                case 2: {
                    string customerId;
                    if (!readRequiredLine("Customer ID: ", customerId)) {
                        return;
                    }
                    if (!openSavingsFor(bank, customerId)) {
                        return;
                    }
                    break;
                }
                case 3: {
                    string customerId;
                    if (!readRequiredLine("Customer ID: ", customerId)) {
                        return;
                    }
                    if (!openCheckingFor(bank, customerId)) {
                        return;
                    }
                    break;
                }
                case 4: {
                    string customerId;
                    if (!readRequiredLine("Customer ID: ", customerId)) {
                        return;
                    }
                    Customer* customer = bank.findCustomer(customerId);
                    if (customer == nullptr) {
                        throw BankingException("Customer not found.");
                    }
                    viewCustomer(customer);
                    break;
                }
                case 5: {
                    string accountNumber;
                    if (!readRequiredLine("Savings account number: ",
                                          accountNumber)) {
                        return;
                    }

                    Account* account = bank.findAccount(accountNumber);
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
                case 6: {
                    string accountNumber;
                    if (!readRequiredLine("Account number: ", accountNumber)) {
                        return;
                    }
                    bank.closeAccount(accountNumber);
                    cout << "Account closed.\n";
                    break;
                }
                case 7:
                    bank.viewAllCustomers();
                    break;
                case 8: {
                    double fee = 0.0;
                    if (!readDouble("New default transaction fee: ", fee)) {
                        return;
                    }
                    if (fee < 0.0) {
                        throw InvalidAmountException(
                            "Transaction fee cannot be negative.");
                    }

                    defaultTransactionFee = fee;
                    cout << "Default transaction fee updated to "
                         << defaultTransactionFee << ".\n";

                    string answer;
                    if (!readRequiredLine(
                            "Update an existing checking account? (y/n): ",
                            answer)) {
                        return;
                    }

                    if (answer == "y" || answer == "Y") {
                        string accountNumber;
                        if (!readRequiredLine(
                                "Checking account number: ", accountNumber)) {
                            return;
                        }

                        Account* account = bank.findAccount(accountNumber);
                        CheckingAccount* checking =
                            dynamic_cast<CheckingAccount*>(account);
                        if (checking == nullptr) {
                            throw BankingException(
                                "Selected account is not a checking account.");
                        }

                        checking->setTransactionFee(fee);
                        cout << "Existing checking account fee updated.\n";
                    } else {
                        cout << "Only future checking accounts will use the new fee.\n";
                    }
                    break;
                }
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

void runCustomerDashboard(Bank& bank, Customer* customer) {
    while (true) {
        printCustomerDashboardMenu(customer->getFullName());

        int choice = -1;
        if (!readInt("Select an option: ", choice)) {
            cout << "\nInput closed.\n";
            return;
        }
        if (choice == 0) {
            return;
        }

        try {
            switch (choice) {
                case 1:
                    viewCustomer(customer);
                    break;
                case 2: {
                    string accountNumber;
                    double amount = 0.0;
                    if (!readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Deposit amount: ", amount)) {
                        return;
                    }

                    Account* account = customer->findAccount(accountNumber);
                    if (account == nullptr) {
                        throw BankingException(
                            "Account does not belong to you.");
                    }

                    account->deposit(amount);
                    cout << "Deposit successful.\n";
                    break;
                }
                case 3: {
                    string accountNumber;
                    double amount = 0.0;
                    if (!readRequiredLine("Account number: ", accountNumber) ||
                        !readDouble("Withdrawal amount: ", amount)) {
                        return;
                    }

                    Account* account = customer->findAccount(accountNumber);
                    if (account == nullptr) {
                        throw BankingException(
                            "Account does not belong to you.");
                    }

                    account->withdraw(amount);
                    cout << "Withdrawal successful.\n";
                    break;
                }
                case 4:
                    if (!openSavingsFor(bank, customer->getCustomerId())) {
                        return;
                    }
                    break;
                case 5:
                    if (!openCheckingFor(bank, customer->getCustomerId())) {
                        return;
                    }
                    break;
                default:
                    cout << "Invalid menu option. Choose 0-5.\n";
                    break;
            }
        } catch (const BankingException& e) {
            cout << "Error: " << e.what() << '\n';
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << '\n';
        }
    }
}

void runCustomerPortal(Bank& bank) {
    while (true) {
        printCustomerPortalMenu();

        int choice = -1;
        if (!readInt("Select an option: ", choice)) {
            cout << "\nInput closed.\n";
            return;
        }
        if (choice == 0) {
            return;
        }

        try {
            if (choice == 1) {
                Customer* customer = readAndAddCustomer(bank);
                if (customer == nullptr) {
                    return;
                }

                cout << "Registration successful.\n";
                runCustomerDashboard(bank, customer);
                return;
            }

            if (choice == 2) {
                string customerId;
                string emailOrPhone;
                if (!readRequiredLine("Customer ID: ", customerId) ||
                    !readRequiredLine("Email or phone number: ",
                                      emailOrPhone)) {
                    return;
                }

                Customer* customer = bank.findCustomer(customerId);
                if (customer == nullptr) {
                    throw BankingException("Customer not found.");
                }
                if (customer->getEmail() != emailOrPhone &&
                    customer->getPhoneNumber() != emailOrPhone) {
                    throw BankingException(
                        "Email or phone number does not match.");
                }

                runCustomerDashboard(bank, customer);
                return;
            }

            cout << "Invalid menu option. Choose 0-2.\n";
        } catch (const BankingException& e) {
            cout << "Error: " << e.what() << '\n';
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << '\n';
        }
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

string Bank::generateCustomerId() {
    return to_string(nextCustomerId++);
}

string Bank::generateAccountNumber() {
    return to_string(nextAccountNumber++);
}

string Bank::addCustomer(string name, string phone, string email) {
    string id = generateCustomerId();
    if (findCustomer(id) != nullptr) {
        throw BankingException("Generated customer ID collision.");
    }

    customers.push_back(
        new Customer(id, move(name), move(phone), move(email)));
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
                                double rate, double minimum) {
    if (findCustomer(customerId) == nullptr) {
        throw BankingException("Customer not found.");
    }

    string accountNumber = generateAccountNumber();
    Account* account = new SavingsAccount(
        accountNumber, initialBalance, rate, minimum);
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

void Bank::viewAllCustomers() {
    if (customers.empty()) {
        cout << "No customers found.\n";
        return;
    }

    for (Customer* customer : customers) {
        cout << "\nCustomer ID: " << customer->getCustomerId() << '\n'
             << "Name: " << customer->getFullName() << '\n'
             << "Phone: " << customer->getPhoneNumber() << '\n'
             << "Email: " << customer->getEmail() << '\n';
        customer->listAccounts();
        cout << "Total balance: " << customer->getTotalBalance() << '\n';
    }
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

    nextCustomerId = 1001;
    nextAccountNumber = 10000001;

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

            try {
                int loadedId = stoi(fields[1]);
                if (loadedId >= nextCustomerId) {
                    nextCustomerId = loadedId + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric customer IDs.
            }

            customers.push_back(customer);
            currentCustomer = customer;
        } else if (fields[0] == "SAVINGS") {
            if (fields.size() != 5 || currentCustomer == nullptr) {
                throw BankingException("Invalid savings account record.");
            }

            try {
                long loadedNumber = stol(fields[1]);
                if (loadedNumber >= nextAccountNumber) {
                    nextAccountNumber = loadedNumber + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric account numbers.
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

            try {
                long loadedNumber = stol(fields[1]);
                if (loadedNumber >= nextAccountNumber) {
                    nextAccountNumber = loadedNumber + 1;
                }
            } catch (const exception&) {
                // Ignore legacy non-numeric account numbers.
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
    const string MANAGER_PASS = "admin123";

    while (true) {
        printMainMenu(bankName);

        int choice = -1;
        if (!readInt("Select an option: ", choice)) {
            cout << "\nInput closed.\n";
            return;
        }
        if (choice == 0) {
            cout << "Saving and exiting...\n";
            return;
        }

        try {
            if (choice == 1) {
                cout << "\n=== Bank Manager Login ===\n"
                     << "Hint: Bank Name = " << bankName
                     << " | Manager Password = " << MANAGER_PASS << '\n';

                string enteredBankName;
                string enteredPassword;
                if (!readRequiredLine("Bank Name: ", enteredBankName) ||
                    !readRequiredLine("Manager Password: ",
                                      enteredPassword)) {
                    return;
                }

                if (enteredBankName != bankName ||
                    enteredPassword != MANAGER_PASS) {
                    cout << "Invalid manager credentials.\n";
                    continue;
                }

                runManagerMenu(*this, defaultTransactionFee);
            } else if (choice == 2) {
                runCustomerPortal(*this);
            } else {
                cout << "Invalid menu option. Choose 0-2.\n";
            }
        } catch (const BankingException& e) {
            cout << "Error: " << e.what() << '\n';
        } catch (const exception& e) {
            cout << "Unexpected error: " << e.what() << '\n';
        }
    }
}
