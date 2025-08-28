#include "../include_cpp/my.h"

bool WalletManager::restore_wallet(const std::string &to_restore)
{
    sqlite3 *db = WalletManager::init_db();
    sqlite3_stmt *restoreStmt = nullptr;
    sqlite3_stmt *restoreTransactionStmt = nullptr;
    std::string result_msg = "Failed to restore wallet!";
    bool result = false;
    

    const char* restoreWalletSQL = "UPDATE wallets SET is_active = TRUE WHERE name = ?;";
    const char* restoreRelatedTransactionsSQL = "UPDATE transactions SET is_archived = FALSE WHERE name = ?;";
    
    if (sqlite3_prepare_v2(db, restoreWalletSQL, -1, &restoreStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, restoreRelatedTransactionsSQL, -1, &restoreTransactionStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    
    sqlite3_bind_text(restoreStmt, 1, to_restore.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(restoreTransactionStmt, 1, to_restore.c_str(), -1, SQLITE_TRANSIENT);
    
    if (sqlite3_step(restoreStmt) != SQLITE_DONE) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    if (sqlite3_step(restoreTransactionStmt) != SQLITE_DONE) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    
    result_msg = "Wallet restored successfully";
    result = true;
    
cleanup:
    if (restoreStmt) sqlite3_finalize(restoreStmt);
    if (restoreTransactionStmt) sqlite3_finalize(restoreTransactionStmt);
    WalletManager::closedb(db);
    cout << result_msg << endl;
    return result;
}
