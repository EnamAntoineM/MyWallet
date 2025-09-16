#include "../include_cpp/my.h"

bool WalletManager::record_transfer(const std::string &to_record)
{   
    sqlite3 *db = WalletManager::init_db();
    transaction trans_insert{};
    sqlite3_stmt *insertTransferStmt = nullptr;
    sqlite3_stmt *withdraw_WalletStmt = nullptr;
    sqlite3_stmt *incomeWalletStmt = nullptr;
    const char *createSQL = "INSERT INTO transactions "
                    "(wallet_name, type, "
                    "category, description, "
                    "amount, related_wallet_name) "
                    "VALUES (?, ?, ?, ?, ?, ?);"
    ;
    const char *incomeWalletSQL = "UPDATE wallets SET balance = balance + ?  WHERE name = ?;";
    const char *withdraw_WalletSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to record transfer!!";
    bool result = false;

    glz::read_json(trans_insert, to_record);
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) { // <=== START TRANSACTION
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, createSQL, -1, &insertTransferStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, incomeWalletSQL, -1, &incomeWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, withdraw_WalletSQL, -1, &withdraw_WalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }

    sqlite3_bind_text(insertTransferStmt,   1,trans_insert.wallet_name.c_str(),         -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(insertTransferStmt,   2,trans_insert.type.c_str(),                -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(insertTransferStmt,   3,trans_insert.category.c_str(),            -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(insertTransferStmt,   4,trans_insert.description.c_str(),         -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(insertTransferStmt,  5,trans_insert.amount);
    sqlite3_bind_text (insertTransferStmt,   6,trans_insert.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    //
    sqlite3_bind_double(incomeWalletStmt, 1, trans_insert.amount);
    sqlite3_bind_text(incomeWalletStmt, 2, trans_insert.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    //
    sqlite3_bind_double(withdraw_WalletStmt, 1, trans_insert.amount);
    sqlite3_bind_text(withdraw_WalletStmt, 2, trans_insert.wallet_name.c_str(), -1, SQLITE_TRANSIENT);

    if (!(sqlite3_step(insertTransferStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (!(sqlite3_step(incomeWalletStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (!(sqlite3_step(withdraw_WalletStmt) == SQLITE_DONE)) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) { //  <--- CLOSING TRANSACTION
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Transfer recorded successfully";
    result = true;
    //
    cleanup:
        if (insertTransferStmt) sqlite3_finalize(insertTransferStmt);
        if (incomeWalletStmt) sqlite3_finalize(incomeWalletStmt);
        if (withdraw_WalletStmt) sqlite3_finalize(withdraw_WalletStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}