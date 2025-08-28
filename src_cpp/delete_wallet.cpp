#include "../include_cpp/my.h"

bool WalletManager::delete_wallet(const std::string &to_delete)
{
    sqlite3 *db = WalletManager::init_db();
    wallet toDelete{};
    sqlite3_stmt *archiveWalletStmt = nullptr;
    sqlite3_stmt *archiveTransactionsStmt = nullptr;
    const char *archiveWalletSQL = "UPDATE wallets SET is_active = FALSE WHERE name = ?;";
    const char *archiveTransactionsSQL = "UPDATE transactions SET is_archived = TRUE WHERE wallet_name = ?;";
    std::string result_msg = "Failed to delete record!!";
    bool result = false;

    glz::read_json(toDelete, to_delete);
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) { // <=== START TRANSACTION
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, archiveWalletSQL, -1, &archiveWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_text(archiveWalletStmt, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(archiveWalletStmt) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }

    if (sqlite3_prepare_v2(db, archiveTransactionsSQL, -1, &archiveTransactionsStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_text(archiveTransactionsStmt, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(archiveTransactionsStmt) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) { //  <--- CLOSING TRANSACTION
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Wallet deleted successfully";
    result = true;
    cleanup:
        if (archiveWalletStmt) sqlite3_finalize(archiveWalletStmt);
        if (archiveTransactionsStmt) sqlite3_finalize(archiveTransactionsStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}