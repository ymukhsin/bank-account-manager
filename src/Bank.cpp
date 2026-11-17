#include "Bank.h"

#include "CheckingAccount.h"
#include "Exceptions.h"
#include "SavingsAccount.h"

#include <fstream>
#include <string>
#include <utility>
#include <vector>

namespace {

std::vector<std::string> splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::string current;

    for (char ch : line) {
        if (ch == delimiter) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }

    fields.push_back(current);
    return fields;
}

double parseAmount(const std::string& value) {
    try {
        return std::stod(value);
    } catch (const std::exception&) {
        throw BankingException("Invalid numeric value in data file.");
    }
}

}  // namespace

Bank::Bank(std::string name, std::string phone,
           std::string mail, std::string location)
    : bankName(std::move(name)),
      phoneNumber(std::move(phone)),
      email(std::move(mail)),
      address(std::move(location)) {}

Bank::~Bank() {
    for (Customer* customer : customers) {
        delete customer;
    }
}

void Bank::addCustomer(Customer* customer) {
    if (customer == nullptr) {
        throw BankingException("Cannot add a null customer.");
    }
    if (findCustomer(customer->getCustomerId()) != nullptr) {
        throw BankingException("Duplicate customer ID.");
    }
    customers.push_back(customer);
}

Customer* Bank::findCustomer(std::string customerId) {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == customerId) {
            return customer;
        }
    }
    return nullptr;
}

void Bank::openAccount(std::string customerId, Account* account) {
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

void Bank::closeAccount(std::string accountNumber) {
    for (Customer* customer : customers) {
        if (customer->findAccount(accountNumber) != nullptr) {
            customer->removeAccount(accountNumber);
            return;
        }
    }
    throw AccountNotFoundException();
}

Account* Bank::findAccount(std::string accountNumber) {
    for (Customer* customer : customers) {
        Account* account = customer->findAccount(accountNumber);
        if (account != nullptr) {
            return account;
        }
    }
    throw AccountNotFoundException();
}

void Bank::deposit(std::string accountNumber, double amount) {
    findAccount(accountNumber)->deposit(amount);
}

void Bank::withdraw(std::string accountNumber, double amount) {
    findAccount(accountNumber)->withdraw(amount);
}

void Bank::saveData(std::string filePath) {
    std::ofstream out(filePath);
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

void Bank::loadData(std::string filePath) {
    std::ifstream in(filePath);
    if (!in) {
        throw BankingException("Unable to open data file for reading.");
    }

    std::string line;
    if (!std::getline(in, line)) {
        throw BankingException("Data file is empty.");
    }

    std::vector<std::string> header = splitLine(line, '|');
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

    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> fields = splitLine(line, '|');
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
