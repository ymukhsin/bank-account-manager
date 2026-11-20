#include "Bank.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "Exceptions.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Bank::Bank(string id, string name, string phone,
           string mail, string location)
    : bankId(id), bankName(name), phoneNumber(phone),
      email(mail), address(location) {}

// The Bank does not delete customers: BankSystem owns the shared customer profiles.
Bank::~Bank() {}

string Bank::generateAccountNumber() {
    // Bank ID makes account numbers different even between different banks.
    return bankId + "-" + to_string(nextAccountNumber++);
}

Customer* Bank::findCustomer(string customerId) {
    for (Customer* customer : customers) {
        if (customer->getCustomerId() == customerId) return customer;
    }
    return nullptr;
}

void Bank::openAccount(Customer* customer, Account* account) {
    if (customer == nullptr || account == nullptr) {
        throw BankingException("Customer or account is missing.");
    }
    if (account->getBankId() != bankId) {
        throw BankingException("This account belongs to another bank.");
    }
    if (customer->findAccount(account->getAccountNumber()) != nullptr) {
        throw BankingException("Duplicate account number.");
    }

    customer->addAccount(account);
    if (findCustomer(customer->getCustomerId()) == nullptr) {
        customers.push_back(customer); // Only a reference; the customer is not registered twice.
    }
}

string Bank::openSavingsAccount(Customer* customer, double initialBalance,
                                double minimum) {
    if (!isActive()) throw BankingException("Bank is not approved or active.");
    if (customer == nullptr) throw BankingException("Customer not found.");
    if (!isfinite(initialBalance) || !isfinite(minimum) ||
        initialBalance < 0 || minimum < 0) {
        throw InvalidAmountException("Initial and minimum balances must be non-negative.");
    }
    if (initialBalance < minimum) {
        throw InvalidAmountException("Initial balance cannot be less than minimum balance.");
    }

    string number = generateAccountNumber();
    Account* account = new SavingsAccount(
        number, initialBalance, defaultInterestRate, minimum, bankId);
    try { openAccount(customer, account); }
    catch (...) { delete account; throw; }
    return number;
}

string Bank::openCheckingAccount(Customer* customer, double initialBalance,
                                 double limit) {
    if (!isActive()) throw BankingException("Bank is not approved or active.");
    if (customer == nullptr) throw BankingException("Customer not found.");
    if (!isfinite(initialBalance) || !isfinite(limit) ||
        initialBalance < 0 || limit < 0) {
        throw InvalidAmountException("Invalid initial balance or overdraft limit.");
    }

    string number = generateAccountNumber();
    Account* account = new CheckingAccount(
        number, initialBalance, limit, defaultTransactionFee, bankId);
    try { openAccount(customer, account); }
    catch (...) { delete account; throw; }
    return number;
}

void Bank::closeAccount(string accountNumber) {
    for (vector<Customer*>::iterator it = customers.begin(); it != customers.end(); ++it) {
        Customer* customer = *it;
        Account* account = customer->findAccount(accountNumber);
        if (account != nullptr && account->getBankId() == bankId) {
            if (account->getBalance() != 0) {
                throw BankingException("Account balance must be zero before closing.");
            }
            customer->removeAccount(accountNumber);

            // Remove the bank's reference if this person has no other account here.
            bool stillHere = false;
            for (Account* remaining : customer->getAccounts()) {
                if (remaining->getBankId() == bankId) stillHere = true;
            }
            if (!stillHere) customers.erase(it);
            return;
        }
    }
    throw AccountNotFoundException();
}

Account* Bank::findAccount(string accountNumber) {
    for (Customer* customer : customers) {
        Account* account = customer->findAccount(accountNumber);
        if (account != nullptr && account->getBankId() == bankId) return account;
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
        cout << "No customers with accounts at this bank.\n";
        return;
    }
    for (Customer* customer : customers) {
        cout << "\033[1;36m\n---- Customer ID: " << customer->getCustomerId()
             << " ----\033[0m\n"
             << "Name: " << customer->getFullName() << '\n'
             << "Phone: " << customer->getPhoneNumber() << '\n'
             << "Email: " << customer->getEmail() << '\n';
        customer->listAccounts(bankId); // Staff may only view their own bank's accounts.
        cout << "Balance at this bank: " << customer->getTotalBalance(bankId) << '\n';
    }
}

void Bank::updateAllInterestRates(double newRate) {
    if (!isfinite(newRate) || newRate < 0.0) {
        throw InvalidAmountException("Interest rate cannot be negative.");
    }
    defaultInterestRate = newRate;
    for (Customer* customer : customers) {
        for (Account* account : customer->getAccounts()) {
            if (account->getBankId() != bankId) continue;
            SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
            if (savings != nullptr) {
                savings->setInterestRate(newRate);
                savings->applyInterest();
            }
        }
    }
}

void Bank::updateAllTransactionFees(double newFee) {
    if (!isfinite(newFee) || newFee < 0.0) {
        throw InvalidAmountException("Transaction fee cannot be negative.");
    }
    defaultTransactionFee = newFee;
    for (Customer* customer : customers) {
        for (Account* account : customer->getAccounts()) {
            if (account->getBankId() != bankId) continue;
            CheckingAccount* checking = dynamic_cast<CheckingAccount*>(account);
            if (checking != nullptr) checking->setTransactionFee(newFee);
        }
    }
}

string Bank::getBankId() const { return bankId; }
string Bank::getStatus() const { return status; }
void Bank::setStatus(string newStatus) { status = newStatus; }
bool Bank::isActive() const { return status == "APPROVED"; }
string Bank::getBankName() const { return bankName; }
string Bank::getPhoneNumber() const { return phoneNumber; }
string Bank::getEmail() const { return email; }
string Bank::getAddress() const { return address; }
double Bank::getDefaultInterestRate() const { return defaultInterestRate; }
double Bank::getDefaultTransactionFee() const { return defaultTransactionFee; }
const vector<Customer*>& Bank::getCustomers() const { return customers; }
