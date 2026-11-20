#ifndef BANK_ACCOUNT_MANAGER_FILE_STORAGE_H
#define BANK_ACCOUNT_MANAGER_FILE_STORAGE_H

class BankSystem;

class FileStorage {
public:
    static void save(const BankSystem& system);
    static void load(BankSystem& system);
};

#endif
