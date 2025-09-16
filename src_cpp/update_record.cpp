#include "../include_cpp/my.h"

bool WalletManager::update_record(const std::string &to_update)
{
    sqlite3 *db = WalletManager::init_db();
    transaction toUpdate{};
    const char *UpdateSQL1 = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *UpdateSQL2 = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *UpdateSQL3 = "UPDATE transactions SET wallet_name = ?, type = ?, category = ?, description = ?, amount = ?, related_wallet_name = ? WHERE id = ?;";
    sqlite3_stmt *revertExpenseStmt = nullptr;
    sqlite3_stmt *revertIncomeStmt = nullptr;
    sqlite3_stmt *revertSrcTransferStmt = nullptr;
    sqlite3_stmt *revertDestTransferStmt = nullptr;
    sqlite3_stmt *applyExpenseStmt = nullptr;
    sqlite3_stmt *applyIncomeStmt = nullptr;
    sqlite3_stmt *applySrcTransferStmt = nullptr;
    sqlite3_stmt *applyDestTransferStmt = nullptr;
    sqlite3_stmt *updateTransactionStmt = nullptr;
    std::string initialWallet = "";
    std::string initialType = "";
    std::string initialRWallet = "";
    std::string currentRWallet = "";
    std::string result_msg = "Failed to update record!";
    bool result = false;
    
    glz::read_json(toUpdate, to_update);
    initialWallet = toUpdate.recorded_at.substr(0, toUpdate.recorded_at.rfind('T'));
    initialType = toUpdate.recorded_at.substr(toUpdate.recorded_at.rfind('T') + 1, toUpdate.recorded_at.length());
    initialRWallet = toUpdate.updated_at.substr(0, toUpdate.updated_at.rfind('T'));
    currentRWallet = toUpdate.updated_at.substr(toUpdate.updated_at.rfind('T') + 1, toUpdate.updated_at.length());
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(initialType, "income")) {
        if (sqlite3_prepare_v2(db, UpdateSQL2, -1, &revertIncomeStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(revertIncomeStmt, 1, toUpdate.is_archived);
        sqlite3_bind_text  (revertIncomeStmt, 2, initialWallet.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(revertIncomeStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(revertIncomeStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else if (WalletManager::caseInsensitiveCMP(initialType, "expense")) {
        if (sqlite3_prepare_v2(db, UpdateSQL1, -1, &revertExpenseStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(revertExpenseStmt, 1, toUpdate.is_archived);
        sqlite3_bind_text  (revertExpenseStmt, 2, initialWallet.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(revertExpenseStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(revertExpenseStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else if (WalletManager::caseInsensitiveCMP(initialType, "transfer")) {
        if (sqlite3_prepare_v2(db, UpdateSQL1, -1, &revertSrcTransferStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, UpdateSQL2, -1, &revertDestTransferStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(revertSrcTransferStmt, 1, toUpdate.is_archived);
        sqlite3_bind_text  (revertDestTransferStmt, 2, initialWallet.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(revertSrcTransferStmt, 1, toUpdate.is_archived);
        sqlite3_bind_text  (revertDestTransferStmt, 2, initialRWallet.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(revertSrcTransferStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(revertSrcTransferStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_step(revertDestTransferStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(revertDestTransferStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }

    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "income")) {
        if (sqlite3_prepare_v2(db, UpdateSQL1, -1, &applyIncomeStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(applyIncomeStmt, 1, toUpdate.amount);
        sqlite3_bind_text  (applyIncomeStmt, 2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(applyIncomeStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(applyIncomeStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else if (WalletManager::caseInsensitiveCMP(toUpdate.type, "expense")) {
        if (sqlite3_prepare_v2(db, UpdateSQL2, -1, &applyExpenseStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(applyExpenseStmt, 1, toUpdate.amount);
        sqlite3_bind_text  (applyExpenseStmt, 2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(applyExpenseStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(applyExpenseStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else if (WalletManager::caseInsensitiveCMP(toUpdate.type, "transfer")) {
        if (sqlite3_prepare_v2(db, UpdateSQL2, -1, &applySrcTransferStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, UpdateSQL1, -1, &applyDestTransferStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
            goto cleanup;
        }
        sqlite3_bind_double(applySrcTransferStmt, 1, toUpdate.amount);
        sqlite3_bind_text  (applySrcTransferStmt, 2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(applyDestTransferStmt, 1, toUpdate.amount);
        sqlite3_bind_text  (applyDestTransferStmt, 2, currentRWallet.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(applySrcTransferStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(applySrcTransferStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_step(applyDestTransferStmt) != SQLITE_DONE) {
            std::cerr << "SQL step(applyDestTransferStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }

    if (sqlite3_prepare_v2(db, UpdateSQL3, -1, &updateTransactionStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateTransactionSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    sqlite3_bind_text(updateTransactionStmt, 1, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateTransactionStmt, 2, toUpdate.type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateTransactionStmt, 3, toUpdate.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateTransactionStmt, 4, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(updateTransactionStmt, 5, toUpdate.amount);
    cout << currentRWallet << endl;
    sqlite3_bind_text(updateTransactionStmt, 6, currentRWallet.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(updateTransactionStmt, 7, toUpdate.id);
    if (sqlite3_step(updateTransactionStmt) != SQLITE_DONE) {
        std::cerr << "SQL step(updateTransactionStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }

    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result = true;
    result_msg = "Transaction updated successfully";
    
    cleanup:
        if (revertExpenseStmt) sqlite3_finalize(revertExpenseStmt);
        if (revertIncomeStmt) sqlite3_finalize(revertIncomeStmt);
        if (revertSrcTransferStmt) sqlite3_finalize(revertSrcTransferStmt);
        if (revertDestTransferStmt) sqlite3_finalize(revertDestTransferStmt);
        if (applyExpenseStmt) sqlite3_finalize(applyExpenseStmt);
        if (applyIncomeStmt) sqlite3_finalize(applyIncomeStmt);
        if (applySrcTransferStmt) sqlite3_finalize(applySrcTransferStmt);
        if (applyDestTransferStmt) sqlite3_finalize(applyDestTransferStmt);
        if (updateTransactionStmt) sqlite3_finalize(updateTransactionStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}