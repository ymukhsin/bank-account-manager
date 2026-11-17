# DEU Bank — C++ Bank Account Manager

OOP II team project. A command-line banking system with savings and checking accounts.

## Run

On Mac/Linux, open a terminal in the project folder:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bank_app
./bank_app
```

On Windows (MinGW):

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bank_app.exe
.\bank_app.exe
```

## Admin login

- Bank name: `deu`
- Password: `admin`

## Customer registration

Enter a full name, the **last eight digits** of a Korean mobile number (the program adds `010`), an email address, and a password.

The program rejects empty or spaces-only answers, bad phone numbers, malformed emails, and invalid menu/amount entries. It also checks for existing email/phone registrations.

## Saving data

There is no manual Save menu option. The application automatically writes to `bank_data.txt` after registration, account creation/closure, deposits, withdrawals, interest updates and fee updates, and when exiting normally. Saved data loads on the next run.

**If you have an existing `bank_data.txt`, keep it in the project folder** to retain saved customers and accounts. The download intentionally does not include personal data.

## UML Class Diagram

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

## Code files

- `include/` — class definitions
- `src/` — implementation and menus
- `uml.mmd` — final UML diagram

The project keeps OOP inheritance, polymorphism, exceptions and file I/O. Input is handled directly inside the menus rather than via separate `readLine`, `readInt`, or `readDouble` functions.

**Class project only:** Customer passwords are stored as plain text in `bank_data.txt`, and the admin password is fixed. This is not suitable for real financial accounts.
