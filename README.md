# Bank Account Manager

A console-based **Bank Account Manager** written in C++ for the **OOP II** final project. The system models bank accounts through an abstract base class and a polymorphic command-line interface.

## Project Information

| Item | Detail |
| --- | --- |
| Project | Bank Account Manager (CLI) |
| Course | OOP II |
| Language | C++ (C++11 or later) |
| Toolchain | g++ |
| Current version | 0.1.0 - foundation |

## Team Credits

| Team member | Role | Responsibilities |
| --- | --- | --- |
| [Team Member 1] | Team lead / architect | Class design, integration, build |
| [Team Member 2] | Developer | Account types, transactions |
| [Team Member 3] | Developer / QA | CLI flows, testing, documentation |

> Replace the placeholder names and roles with the actual team members before submission.

## Requirements Checklist

### Foundation (this milestone)

- [x] Abstract `Account` base class in `include/Account.h`
- [x] Protected state: `accountNumber`, `holderName`, `balance`
- [x] Pure virtual `deposit(double)`, `withdraw(double)`, `displayInfo()`
- [x] Virtual destructor for safe polymorphic deletion
- [x] Interactive "Bank Account Manager" CLI welcome banner
- [x] Header inclusion verified at compile time
- [x] Clean build with `g++ -Iinclude src/main.cpp -o bank_app`

### Planned milestones

- [ ] Concrete account types (`SavingsAccount`, `CheckingAccount`)
- [ ] Account creation, lookup, listing, and closing
- [ ] Deposit/withdraw flows with input and balance validation
- [ ] Transaction history and reporting
- [ ] Persistent storage (load/save accounts)
- [ ] Exception handling and edge-case tests
- [ ] Final UML diagram, report, and demo

## Project Structure

```text
bank-account-manager/
|-- include/
|   `-- Account.h        # Abstract base class
|-- src/
|   `-- main.cpp         # CLI entry point and inclusion smoke test
|-- .gitignore
`-- README.md
```

## Build and Run

### Prerequisites

- g++ with C++11 support or newer
- A terminal (PowerShell, Bash, etc.)

### Compile

From the project root:

```bash
g++ -Iinclude src/main.cpp -o bank_app
```

`-Iinclude` adds the `include/` directory to the header search path, so `#include "Account.h"` resolves no matter which directory the compiler is invoked from.

### Run

```bash
./bank_app        # Linux / macOS / Git Bash
bank_app.exe      # Windows PowerShell or CMD
```

### Recommended strict build (optional)

```bash
g++ -std=c++14 -Wall -Wextra -pedantic -Iinclude src/main.cpp -o bank_app
```

## Design Notes

- `Account` is abstract: it defines the contract but cannot be instantiated.
- Data members are `protected` so derived account types can manage state while clients use only the public API.
- `balance` defaults to `0.0`; concrete constructors should initialise `accountNumber` and `holderName`.
- The virtual destructor guarantees that deleting a derived account through an `Account*` runs the full destructor chain.
- Derived classes must enforce their own rules (positive amounts, no overdraft below the allowed minimum, etc.).

## Next Steps

1. Add `SavingsAccount` and `CheckingAccount` in `include/` and `src/`.
2. Replace the placeholder banner flow with the main menu.
3. Add transaction records and file persistence.
4. Expand this README with usage examples and test evidence.
