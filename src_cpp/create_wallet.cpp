#include "../include_cpp/my.h"

bool WalletManager::create_wallet(const std::string &to_create)
{
    sqlite3 *db = WalletManager::init_db();
    wallet to_insert{};
    sqlite3_stmt *stmt = nullptr;
    const char *createSQL = "INSERT INTO wallets "
                    "(name, currency, source, "
                    "initial_amount, balance, color) "
                    "VALUES (?, ?, ?, ?, ?, ?);"
    ;
    std::string result_msg = "Failed to create wallet!!";
    bool result = false;

    glz::read_json(to_insert, to_create);
    if (sqlite3_prepare_v2(db, createSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    sqlite3_bind_text(stmt, 1, to_insert.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, to_insert.currency.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, to_insert.source.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, to_insert.initial_amount);
    sqlite3_bind_int(stmt, 5, to_insert.balance);
    sqlite3_bind_text(stmt, 6, to_insert.color.c_str(), -1, SQLITE_STATIC);
    if (!(sqlite3_step(stmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    result_msg = "Wallet created successfully!";
    result = true;

    cleanup:
        if (stmt) sqlite3_finalize(stmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}