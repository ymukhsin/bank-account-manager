# Bank Account Manager

A console-based **Bank Account Manager** built in C++17 for the **OOP II** final project. The application separates Bank Manager and Customer roles, authenticates customers with passwords, auto-generates customer IDs and account numbers, controls interest rates and transaction fees at the bank level, and persists all data between runs.

## Project Information

| Item | Detail |
| --- | --- |
| Project | Bank Account Manager (CLI) |
| Course | OOP II |
| Instructor | Professor Asrat Kaleab Taye |
| Language | C++17 |
| Toolchain | g++ |
| Version | 1.2.0 |

## Team Credits

| Team member | Role | Responsibilities |
| --- | --- | --- |
| [Team Member 1] | Team lead / architect | Class design, integration, build |
| [Team Member 2] | Developer | Account types, transactions |
| [Team Member 3] | Developer / QA | CLI flows, testing, documentation |

> Replace the placeholder names and roles with the actual team members before submission.

## Project Overview

`Bank` owns customers through `Customer*` pointers, and each customer owns accounts through `Account*` pointers. New customers receive an automatic 4-digit ID and choose a password during registration. New bank accounts receive automatic 8-digit account numbers.

Customers open their own accounts. Savings accounts receive the Bank's current default interest rate, and checking accounts receive its current default transaction fee. The Bank Manager can update those defaults and apply the new values to every existing account.

## OOP Design Summary

- **Encapsulation:** Customer passwords, account balances, ID counters, default rates, and collections are private or protected and accessed through public operations.
- **Inheritance:** `SavingsAccount` and `CheckingAccount` inherit publicly from `Account`.
- **Abstraction:** `Account` is abstract and declares `withdraw()`, `getType()`, and `display()` as pure virtual functions.
- **Runtime polymorphism:** Accounts are managed through `Account*`, and `dynamic_cast` is used for type-specific rate and fee updates.
- **Authentication:** `Customer::verifyPassword()` and `Bank::loginCustomer()` validate customer credentials by email or phone number.
- **Custom exceptions:** `BankingException` derives from `std::runtime_error`; `InvalidAmountException`, `InsufficientFundsException`, and `AccountNotFoundException` derive from it.
- **File I/O:** `saveData()` and `loadData()` persist bank, customer (including password), and account records using `<fstream>`, `getline`, `find`, and `substr`.
- **Memory ownership:** Raw pointers only. `~Bank()` deletes customers and `~Customer()` deletes accounts.

## CLI Features

### Main Menu

```text
1) Login as Bank Manager
2) Register / Login as Customer
0) Save & Exit
```

### Bank Manager Menu

```text
1) View All Customers & Accounts
2) Search Customer Accounts & Total Balance
3) Update Savings Interest Rate (All Accounts)
4) Update Checking Transaction Fee (All Accounts)
5) Apply Interest to Savings Accounts
6) Close Customer Account
0) Logout to Main Menu
```

Manager login credentials are displayed on the login screen:

```text
Bank Name: DEU Bank
Password:  admin123
```

### Customer Portal

```text
1) Register as New Customer
2) Login as Existing Customer
0) Back to Main Menu
```

Registration asks for full name, phone number, email, and password, then generates and displays the customer ID. Existing-customer login uses email or phone number plus password.

### Customer Dashboard

```text
1) View My Accounts & Total Balance
2) Deposit
3) Withdraw
4) Open Savings Account
5) Open Checking Account
0) Logout to Main Menu
```

Opening a savings account asks only for the initial deposit and minimum balance; the interest rate comes from the Bank. Opening a checking account asks only for the initial deposit and overdraft limit; the transaction fee comes from the Bank. Deposits and withdrawals verify ownership through `Customer::findAccount()`.

## Project Structure

```text
bank-account-manager/
|-- include/
|   |-- Account.h
|   |-- Bank.h
|   |-- CheckingAccount.h
|   |-- Customer.h
|   |-- Exceptions.h
|   `-- SavingsAccount.h
|-- src/
|   |-- Account.cpp
|   |-- Bank.cpp
|   |-- CheckingAccount.cpp
|   |-- Customer.cpp
|   |-- SavingsAccount.cpp
|   `-- main.cpp
|-- .gitignore
|-- README.md
`-- uml.mmd
```

`bank_data.txt` is generated at runtime and is excluded from version control.

## System Architecture & UML Diagram

```mermaid
classDiagram
    direction TB

    class Bank {
        -string bankName
        -string phoneNumber
        -string email
        -string address
        -int nextCustomerId
        -long nextAccountNumber
        -double defaultInterestRate
        -double defaultTransactionFee
        -vector~Customer*~ customers
        -string generateCustomerId()
        -string generateAccountNumber()
        +Bank(string name, string phone, string email, string address)
        +addCustomer(string name, string phone, string email, string password) string
        +findCustomer(string customerId) Customer*
        +loginCustomer(string emailOrPhone, string password) Customer*
        +openSavingsAccount(string customerId, double initialBalance, double minimum) string
        +openCheckingAccount(string customerId, double initialBalance, double limit) string
        +openAccount(string customerId, Account* account) void
        +closeAccount(string accountNumber) void
        +findAccount(string accountNumber) Account*
        +deposit(string accountNumber, double amount) void
        +withdraw(string accountNumber, double amount) void
        +viewAllCustomers() void
        +updateAllInterestRates(double newRate) void
        +updateAllTransactionFees(double newFee) void
        +saveData(string filePath) void
        +loadData(string filePath) void
        +run() void
    }

    class Customer {
        -string customerId
        -string fullName
        -string phoneNumber
        -string email
        -string password
        -vector~Account*~ accounts
        +Customer(string id, string name, string phone, string email, string password)
        +addAccount(Account* account) void
        +removeAccount(string accountNumber) void
        +findAccount(string accountNumber) Account*
        +listAccounts() void
        +getTotalBalance() double
        +getPassword() string
        +verifyPassword(string pwd) bool
    }

    class Account {
        <<abstract>>
        -string accountNumber
        #double balance
        +Account(string number, double initialBalance)
        +virtual ~Account()
        +deposit(double amount) void
        +withdraw(double amount)* void
        +getBalance() double
        +getAccountNumber() string
        +getType() string*
        +display() void*
    }

    class SavingsAccount {
        -double interestRate
        -double minimumBalance
        +SavingsAccount(string number, double balance, double rate, double minimumBalance)
        +withdraw(double amount) void
        +applyInterest() void
        +getType() string
        +display() void
        +setInterestRate(double rate) void
    }

    class CheckingAccount {
        -double overdraftLimit
        -double transactionFee
        +CheckingAccount(string number, double balance, double limit, double fee)
        +withdraw(double amount) void
        +getType() string
        +display() void
        +setTransactionFee(double fee) void
    }

    class BankingException {
        +BankingException(string message)
    }
    class InvalidAmountException {
        +InvalidAmountException(string message)
    }
    class InsufficientFundsException {
        +InsufficientFundsException(string message)
    }
    class AccountNotFoundException {
        +AccountNotFoundException(string message)
    }
    class std_runtime_error {
        <<C++ standard library>>
    }

    Bank "1" *-- "0..*" Customer : manages
    Customer "1" *-- "0..*" Account : owns
    Account <|-- SavingsAccount
    Account <|-- CheckingAccount

    std_runtime_error <|-- BankingException
    BankingException <|-- InvalidAmountException
    BankingException <|-- InsufficientFundsException
    BankingException <|-- AccountNotFoundException

    Account ..> InvalidAmountException : may throw
    Account ..> InsufficientFundsException : may throw
    Bank ..> AccountNotFoundException : may throw
```

## Build and Run

### Prerequisites

- g++ with C++17 support
- A terminal (PowerShell, Bash, etc.)

### Compile

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bank_app
```

### Run

```bash
./bank_app
```

On Windows PowerShell, the executable may also be launched with:

```bash
./bank_app.exe
```

The application loads `bank_data.txt` on startup when present and saves on every exit path. A missing data file is handled gracefully.

## Persistence Format

`bank_data.txt` stores one pipe-delimited record per line. Customer passwords and spaces in text fields are preserved.

```text
BANK|DEU Bank|02-123-4567|bank@example.com|123 Main Street, Seoul
CUSTOMER|1001|Alice Smith|010-1111-2222|alice@example.com|pass123
SAVINGS|10000001|1100|0.1|100
CHECKING|10000002|200|300|4
SAVINGS|10000003|100|0.1|10
```

On load, the bank restores passwords and account-specific settings and continues ID generation from the highest saved customer and account numbers.

## Verification Checklist

- [x] Clean C++17 build with `-Wall -Wextra` and zero warnings
- [x] Raw pointer ownership with `new`/`delete`; no smart pointers
- [x] Abstract `Account` with pure virtual functions and virtual destructor
- [x] Customer registration includes a password and displays the auto-generated ID
- [x] Customer login works with email or phone plus password
- [x] Passwords are saved and restored across application restarts
- [x] Bank Manager cannot add customers or open accounts; customers self-register and self-open
- [x] Manager updates interest rates and transaction fees for all existing accounts
- [x] Manager can apply interest to all savings accounts
- [x] New accounts inherit the Bank's current default rate or fee
- [x] Admin view-all and customer search display all accounts and totals
- [x] Customer ownership checks prevent accessing another customer's account
- [x] End-to-end test verified password login after restart, rate `0.1`, fee `4`, and total balance `1400`

## Notes

- Build artifacts and `bank_data.txt` are excluded by `.gitignore`.
- Replace the team-credit placeholders before submitting the project.
