#pragma once

#include "my.h"
#include "wallet.h"
#include "transaction.h"

class WalletManager {
    WalletManager();

    public:
        static bool record_transaction       (const std::string &to_record);
        static bool record_transfer          (const std::string &to_record);
        static bool create_wallet            (const std::string &to_create);
        static bool update_record            (const std::string &to_update);
        static bool update_wallet            (const std::string &to_update);
        static std::string get_wallets       (void);
        static std::string get_records       (void);
        static bool delete_record            (const std::string &to_delete);
        static bool restore_wallet           (const std::string &tor_restore);
        static bool delete_wallet            (const std::string &to_delete);
        static bool delete_wallet_permanently(const std::string &to_delete);
    private:
        static sqlite3 *init_db(void);
        static std::string safe_column_text(sqlite3_stmt* stmt, int col);
        static void closedb(sqlite3 *db);
        static void init_tables(sqlite3 *db);
        static bool caseInsensitiveCMP(const std::string& str1, const std::string& str2);
};
