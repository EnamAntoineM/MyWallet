#include "../include_cpp/my.h"

bool WalletManager::update_transfer(const std::string &to_update)
{ 
    sqlite3 *db = WalletManager::init_db();
    transaction trans_update{};
    transaction old_transaction{};
    sqlite3_stmt *selectOldStmt = nullptr;
    sqlite3_stmt *updateTransferStmt = nullptr;
    sqlite3_stmt *revertOldWalletStmt = nullptr;
    sqlite3_stmt *revertNewWalletStmt = nullptr;
    sqlite3_stmt *applyOldWalletStmt = nullptr;
    sqlite3_stmt *applyNewWalletStmt = nullptr;
    const char *selectOldSQL = "SELECT wallet_name, amount, related_wallet_name "
                              "FROM transactions WHERE id = ?;";
    const char *updateSQL    = "UPDATE transactions SET "
                              "wallet_name = ?, category = ?, description = ?, "
                              "amount = ?, related_wallet_name = ? "
                              "WHERE id = ?;";
    const char *revertOldWalletSQL = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *revertNewWalletSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *applyOldWalletSQL  = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *applyNewWalletSQL  = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    std::string result_msg = "Failed to update transfer!!";
    bool result = false;
    
    glz::read_json(trans_update, to_update);
    if (trans_update.id <= 0) {
        result_msg = "Invalid transaction ID!!";
        result = false;
        goto cleanup;
    }
    if (trans_update.amount <= 0) {
        result_msg = "Transfer amount must be positive!!";
        result = false;
        goto cleanup;
    }    
    if (trans_update.wallet_name.empty() || trans_update.related_wallet_name.empty()) {
        result_msg = "Wallet names cannot be empty!!";
        result = false;
        goto cleanup;
    }    
    if (trans_update.wallet_name == trans_update.related_wallet_name) {
        result_msg = "Source and destination wallets must be different!!";
        result = false;
        goto cleanup;
    }
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, selectOldSQL, -1, &selectOldStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (select): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }    
    if (sqlite3_prepare_v2(db, updateSQL, -1, &updateTransferStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (update): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }    
    if (sqlite3_prepare_v2(db, revertOldWalletSQL, -1, &revertOldWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (revert old): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }    
    if (sqlite3_prepare_v2(db, revertNewWalletSQL, -1, &revertNewWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (revert new): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }    
    if (sqlite3_prepare_v2(db, applyOldWalletSQL, -1, &applyOldWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (apply old): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }    
    if (sqlite3_prepare_v2(db, applyNewWalletSQL, -1, &applyNewWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error (apply new): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_int64(selectOldStmt, 1, trans_update.id);
    if (sqlite3_step(selectOldStmt) != SQLITE_ROW) {
        std::cerr << "Transaction not found or SQL step error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        result_msg = "Transaction not found!!";
        result = false;
        goto cleanup;
    }    
    old_transaction.wallet_name = reinterpret_cast<const char*>(sqlite3_column_text(selectOldStmt, 0));
    old_transaction.amount = sqlite3_column_int64(selectOldStmt, 1);
    old_transaction.related_wallet_name = reinterpret_cast<const char*>(sqlite3_column_text(selectOldStmt, 2));
    sqlite3_bind_int64(revertOldWalletStmt, 1, old_transaction.amount);
    sqlite3_bind_text(revertOldWalletStmt, 2, old_transaction.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(revertNewWalletStmt, 1, old_transaction.amount);
    sqlite3_bind_text(revertNewWalletStmt, 2, old_transaction.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(revertOldWalletStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error (revert old): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(revertNewWalletStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error (revert new): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_text(updateTransferStmt,  1, trans_update.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateTransferStmt,  2, trans_update.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateTransferStmt,  3, trans_update.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateTransferStmt, 4, trans_update.amount);
    sqlite3_bind_text(updateTransferStmt,  5, trans_update.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateTransferStmt, 6, trans_update.id);
    if (sqlite3_step(updateTransferStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error (update): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_int64(applyOldWalletStmt, 1, trans_update.amount);
    sqlite3_bind_text(applyOldWalletStmt, 2, trans_update.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(applyNewWalletStmt, 1, trans_update.amount);
    sqlite3_bind_text(applyNewWalletStmt, 2, trans_update.related_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(applyOldWalletStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error (apply old): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(applyNewWalletStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error (apply new): " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Transfer updated successfully";
    result = true;

    cleanup:
        if (selectOldStmt) sqlite3_finalize(selectOldStmt);
        if (updateTransferStmt) sqlite3_finalize(updateTransferStmt);
        if (revertOldWalletStmt) sqlite3_finalize(revertOldWalletStmt);
        if (revertNewWalletStmt) sqlite3_finalize(revertNewWalletStmt);
        if (applyOldWalletStmt) sqlite3_finalize(applyOldWalletStmt);
        if (applyNewWalletStmt) sqlite3_finalize(applyNewWalletStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}