#include "../include_cpp/my.h"

bool WalletManager::record_transaction(const std::string &to_record)
{
    sqlite3 *db = WalletManager::init_db();
    transaction to_insert{};
    sqlite3_stmt *insertTransactionStmt = nullptr;
    sqlite3_stmt *updateWalletStmt = nullptr;
    const char *createSQL = "INSERT INTO transactions "
                    "(wallet_name, type, "
                    "category, description, "
                    "amount, related_wallet_name) "
                    "VALUES (?, ?, ?, ?, ?, ?);"
    ;
    const char *updateWalletSQL_income = "UPDATE wallets SET balance = balance + ?  WHERE name = ?;";
    const char *updateWalletSQL_expense = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to record transaction!!";
    bool result = false;

    glz::read_json(to_insert, to_record);
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) { // <=== START TRANSACTION
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, createSQL, -1, &insertTransactionStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(to_insert.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, updateWalletSQL_income, -1, &updateWalletStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, updateWalletSQL_expense, -1, &updateWalletStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    
    sqlite3_bind_text(insertTransactionStmt,   1, to_insert.wallet_name.c_str(),         -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt,   2, to_insert.type.c_str(),                -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt,   3, to_insert.category.c_str(),            -1, SQLITE_STATIC);
    sqlite3_bind_text(insertTransactionStmt,   4, to_insert.description.c_str(),         -1, SQLITE_STATIC);
    sqlite3_bind_double(insertTransactionStmt,  5, to_insert.amount);
    sqlite3_bind_text(insertTransactionStmt,   6, to_insert.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    //----------------------------------------------------------------------
    sqlite3_bind_double(updateWalletStmt, 1, to_insert.amount);
    sqlite3_bind_text(updateWalletStmt,  2, to_insert.wallet_name.c_str(), -1, SQLITE_STATIC);

    if (!(sqlite3_step(insertTransactionStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (!(sqlite3_step(updateWalletStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) { //  <--- CLOSING TRANSACTION
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Transaction recorded successfully";
    result = true;
    //
    cleanup:
        if (insertTransactionStmt) sqlite3_finalize(insertTransactionStmt);
        if (updateWalletStmt) sqlite3_finalize(updateWalletStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}