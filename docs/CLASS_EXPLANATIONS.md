# How the updated bank classes work — team notes

## Why the design changed

**Earlier design:** A customer had to register separately in a chosen bank. The same person could have different customer profiles at different banks.

**Current design:** The customer registers only once with `BankSystem`. After logging in, the customer can open a savings or checking account at any bank. One `Customer` object owns all their accounts.

## What each class does

| Class | Simple explanation |
|---|---|
| `BankSystem` | Owns all banks, staff, and customer profiles. Registration and login happen here. |
| `Customer` | Saves customer contact details/password and owns their accounts. |
| `Bank` | Has bank name/contact/rates and issues savings/checking accounts. It keeps references only to customers with accounts here. |
| `Account` | Abstract parent class. Stores an account number, bank ID, and balance. |
| `SavingsAccount` | Adds interest rate and minimum balance. Its withdrawal checks the minimum. |
| `CheckingAccount` | Adds overdraft limit and transaction fee. Its withdrawal checks both. |
| `Admin` | Checks `admin` / `admin` login and allows viewing all banks, all customers, or customers at one bank. |
| `BankStaff` | Stores staff profile and which bank they work for. |
| `FileStorage` | Saves and loads customer, bank, staff and account records. |
| Exception classes | Show meaningful errors rather than accepting invalid operations. |

## Important UML relationships

- `BankSystem` **composition** `Customer`: BankSystem owns each customer, only once.
- `BankSystem` **composition** `Bank`: manages several banks.
- `Customer` **composition** `Account`: one person can own many accounts, potentially in different banks.
- `Account` **association** `Bank`: the `bankId` says which bank issued that account.
- `Bank` **association** `Customer`: the bank lists only people who have accounts there; the bank does not own their customer profiles.
- `SavingsAccount` and `CheckingAccount` **hierarchically inherit** `Account`.
- `BankStaff` **association** `Bank`: staff works at one bank.
- `FileStorage` **dependency** `BankSystem`: it reads and writes bank system data.

## Q&A

**How does a customer open accounts at two banks?** The customer logs in once, chooses a bank for each new account. `Bank::openSavingsAccount()` or `Bank::openCheckingAccount()` creates an `Account` with that bank ID and adds it to the same `Customer`.

**What if two banks both generate account number 10000001?** New account IDs include bank IDs such as `B001-10000001` and `B002-10000001`, so they are unique.

**How can bank staff avoid seeing other banks' accounts?** `Bank::viewAllCustomers()` and the customer search view filter accounts by `account->getBankId()`.

**Why is runtime polymorphism used?** The program holds `Account*` pointers and calls `withdraw()` and `display()`. Each child class implements the correct logic.

**What is different about file storage?** Customers are written only once as `CUSTOMER|...`; each `SAVINGS` and `CHECKING` record includes its bank ID and customer ID. Data is saved automatically.

## Latest updates

- Customers type the bank **name** (not ID) to open an account. Bank names are matched without case sensitivity.
- The one built-in Admin uses username `admin` and password `admin`. Admin can see banks and customers, but cannot change rates or transaction fees.
- BankStaff can change savings interest rates and checking fees only for the bank associated with that staff member. Changing the interest rate also calls `applyInterest()` for existing savings accounts at that bank immediately. 
- `FileStorage` stores banks, customers, accounts, admins, and bank staff separately in `banks.txt`, `customers.txt`, `accounts.txt`, `admins.txt`, and `bank_staff.txt`.

## Bank approval and deactivation

`Bank` has a `status` string: `PENDING`, `APPROVED`, `REJECTED`, or `INACTIVE`. Newly registered banks start as `PENDING`. The one built-in DEU example bank starts `APPROVED`. The `Admin` menu in `BankSystem` reviews pending banks, views staff, counts system records, and deactivates banks. `FileStorage` saves this status in `banks.txt`. Only APPROVED banks allow staff access, opening accounts, deposits, and withdrawals. Existing customer and account information stays saved when a bank becomes INACTIVE.
