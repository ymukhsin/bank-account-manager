# DEU Bank System — OOP II C++ Project

A beginner-friendly C++17 command-line banking program with several banks and **one customer profile per person**.

## Build and run

From the project directory:

**macOS / Linux**

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bank_app
./bank_app
```

**Windows (MinGW / Git Bash)**

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o bank_app.exe
./bank_app.exe
```

Only standard C++ headers are used. There is no database, external framework, or extra package.

## Main menu

```text
========== DEU BANK SYSTEM ==========
1) Admin login
2) Bank staff registration
3) Bank staff login
4) Customer portal
0) Exit
```

### Customer: register once, use multiple banks

1. Choose **Customer portal → Register**. Enter your name, phone, email and password **only once**.
2. Log in with the same email/phone and password, no matter which banks you use.
3. Choose **Open savings account** or **Open checking account**.
4. The program shows only **approved** banks. Enter a **bank name** (for example `deu` or `Campus Bank`); capitalization does not matter.
5. Enter the account details and open it. Repeat with another bank if you want.
6. **View my accounts** shows accounts from every bank and your combined balance.

Example: one customer, `1001`, can have a DEU Bank savings account `B001-10000001` and a Campus Bank checking account `B002-10000001`. These accounts are stored under **one** customer profile.

The application automatically adds `010` to eight-digit Korean mobile input and checks for invalid names, malformed emails, blank required fields, and invalid amounts.

### Bank staff

Staff members provide their contact details and create a bank record with bank name, contact phone, email and address. Each staff member is linked to their own bank. Newly registered banks remain **PENDING** until the admin approves them. Staff cannot log in until approval. A staff member can view/search the customers **with accounts at that bank** and close accounts with zero balance; they cannot see the customer's accounts held at another bank.

This self-service bank creation is only a teaching simplification. Real bank registration and staff permissions require verification.

### Admin

Classroom demo credentials: `admin` / `admin`. The administrator can list banks and customers, view staff, approve/reject pending banks, see system statistics, and deactivate an approved bank. Only bank staff can change settings for their own bank. Deactivation keeps saved account data but blocks new accounts and transactions until a future reactivation feature is added.

### Savings and checking

- **SavingsAccount:** has `interestRate` and `minimumBalance`. Withdrawal cannot leave less than the minimum. Interest is applied automatically when that bank’s staff changes the interest rate.
- **CheckingAccount:** has `overdraftLimit` and `transactionFee`. Withdrawal subtracts the amount plus the fee, subject to the overdraft limit.

Both inherit from abstract `Account` and implement virtual `withdraw()` and `display()` (runtime polymorphism).

## File saving (FileStorage)

`BankSystem::save()` calls `FileStorage::save()`. Data saves automatically after successful changes and on normal exit. `FileStorage::load()` restores objects when the program starts, with validation so a damaged file does not overwrite the existing information.

The program writes five files (`banks.txt`, `customers.txt`, `accounts.txt`, `admins.txt`, `bank_staff.txt`) inside the **`data/` folder** (relative to the working directory). This folder is created automatically. Files are ignored by Git. Only these five files inside `data/` are loaded; older formats are not supported.

The new file format stores each customer **once** and tags each account with a bank ID:

```text
BANK|B001|deu|01000000000|deu@example.com|Busan|0.05|2|APPROVED
BANK|B002|Campus Bank|01022223333|campus@example.com|Seoul|0.04|3|APPROVED
CUSTOMER|1001|Kim Minji|01012345678|minji@example.com|test123
SAVINGS|B001|1001|B001-10000001|500000|0.05|10000
CHECKING|B002|1001|B002-10000001|200000|50000|3
```

The loader reads only the current bank, admin, staff, customer, and account records from `data/`. Keep all five files together.

## Main classes and OOP concepts

- `BankSystem` — owns all banks, customer profiles and staff; operates main menus.
- `Bank` — bank details and account operations. Keeps **non-owning** references to customers who have accounts at this bank.
- `Customer` — one profile, owns accounts across several banks.
- `Account` — abstract base class, stores `bankId`, account number and balance.
- `SavingsAccount`, `CheckingAccount` — hierarchical inheritance and method overriding.
- `Admin` — administrator login and access to bank management.
- `BankStaff` — staff profile linked to one bank.
- `FileStorage` — separate class for saving/loading all data.
- `BankingException` plus derived exceptions — input and banking error handling.

**Key relationships:** BankSystem composes customers and banks; Customer composes accounts; Account references the bank that issued it; bank staff is associated with one bank. See `uml.mmd` and `docs/Updated_Bank_System_UML.png`.

## Folder structure

```text
include/                  Class declarations (.h)
src/                      Implementations and main.cpp (.cpp)
docs/                     UML diagram and design explanations
uml.mmd                   Editable Mermaid UML
data/                    Five current-format saved .txt files
README.md                 This guide
.gitignore                Files not to commit
```

**Classroom project only:** Passwords are saved in plain text, the admin credential is fixed, new banks require administrator approval, and balances use `double`. This design is not secure enough for real financial services.


## Updated roles and storage

- Admin login: username `admin`, password `admin`. One built-in administrator. Admin views bank and customer records but cannot change bank rates or fees.
- Bank staff: register a bank, log in, and update **only their own bank's** savings interest rate or checking transaction fee. Interest is automatically applied to that bank's existing savings accounts whenever its rate changes; there is no separate Apply Interest menu.
- Customers: register once, then type a bank **name** when opening each account. Bank-name matching ignores capitalization and leading/trailing spaces.
- `FileStorage` saves five separate text files in `data/`: `banks.txt`, `customers.txt`, `accounts.txt`, `admins.txt`, and `bank_staff.txt`. Keep all five together. The program creates this folder when saving. No older-format import is included.
- For demonstration only: passwords are stored in plain text; a production banking app requires secure credential storage.

## Admin menu (approval workflow)

```text
--- ADMIN: ALL BANKS ---
1) View registered banks
2) View all customers
3) View customers for a specific bank
4) View all bank staff
5) Approve or reject bank registration
6) View system statistics
7) Deactivate a bank
0) Logout
```

New banks remain PENDING. The admin approves or rejects them by bank name. Rejected and inactive bank records are preserved. Only APPROVED banks allow staff login or customer transactions. Existing files with eight-field BANK records load as APPROVED.

The editable class diagram is `uml.mmd`. Open it in https://mermaid.live/ to export an updated PNG/SVG.
