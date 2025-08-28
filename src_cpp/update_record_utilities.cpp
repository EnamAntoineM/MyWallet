#include "../include_cpp/my.h"

bool WalletManager::ABC(sqlite3 *db, transaction &toUpdate)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                " category = ?, description = ? WHERE id = ?;"
    ;
    std::string result_msg = "Failed to update the record";
    bool result = false;

    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 3, toUpdate.id);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE) {
        std::cerr << "SQL step error: " << sqlite3_errmsg(db) << " !!" << endl;
        goto cleanup;
    }
    result_msg = "Record updated successfully!";
    result = true;

    cleanup:
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::AB_C(sqlite3 *db, transaction &toUpdate)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "type = ?,  category = ?, "
                                "description = ? WHERE id = ?;"
    ;
    const char *adjustWalletBalance_toIncomeSQL = "UPDATE wallets SET balance = balance + (2 * ?) WHERE name = ?;";
    const char *adjustWalletBalance_toExpenseSQL = "UPDATE wallets SET balance = balance - (2 * ?) WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustWalletBalance_toIncomeSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustWalletBalance_toExpenseSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.type.c_str(),        -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  3, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.id);
    sqlite3_bind_int64(updateWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateWalletBalanceStmt,  2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateWalletBalanceStmt) != SQLITE_DONE) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateWalletBalanceStmt) sqlite3_finalize(updateWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::A_BC(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "category = ?, "
                                "description = ?, amount = ? WHERE id = ?;"
    ;
    const char *adjustWalletBalanceIncomeSQL =  "UPDATE wallets SET balance = balance - ? + ? WHERE name = ?;";
    const char *adjustWalletBalanceExpenseSQL = "UPDATE wallets SET balance = balance + ? - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustWalletBalanceIncomeSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustWalletBalanceExpenseSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 3, toUpdate.amount);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.id);
    sqlite3_bind_int64(updateWalletBalanceStmt, 1, previous_amount);
    sqlite3_bind_int64(updateWalletBalanceStmt, 2, toUpdate.amount);
    sqlite3_bind_text(updateWalletBalanceStmt,  3, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateWalletBalanceStmt) sqlite3_finalize(updateWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::A_B_C(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "type = ?, category = ?, "
                                "description = ?, amount = ? WHERE id = ?;"
    ;
    const char *adjustWalletBalance_to_IncomeSQL =  "UPDATE wallets SET balance = balance + ? + ? WHERE name = ?;";
    const char *adjustWalletBalance_to_ExpenseSQL = "UPDATE wallets SET balance = balance - ? - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustWalletBalance_to_IncomeSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustWalletBalance_to_ExpenseSQL, -1, &updateWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.type.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  3, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.amount);
    sqlite3_bind_int64(updateRecordStmt, 5, toUpdate.id);
    sqlite3_bind_int64(updateWalletBalanceStmt, 1, previous_amount);
    sqlite3_bind_int64(updateWalletBalanceStmt, 2, toUpdate.amount);
    sqlite3_bind_text(updateWalletBalanceStmt,  3, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateWalletBalanceStmt) sqlite3_finalize(updateWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::_ABC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateOldWalletBalanceStmt = nullptr;
    sqlite3_stmt *updateNewWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "category = ?, "
                                "description = ? WHERE id = ?;"
    ;
    const char *adjustOldWalletBalance_IncomeSQL =  "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *adjustNewWalletBalance_IncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustOldWalletBalance_ExpenseSQL = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustNewWalletBalance_ExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_IncomeSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_IncomeSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_ExpenseSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_ExpenseSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 3, toUpdate.id);
    sqlite3_bind_int64(updateOldWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateOldWalletBalanceStmt,  2, previous_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateNewWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateNewWalletBalanceStmt,  2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateOldWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateNewWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateOldWalletBalanceStmt) sqlite3_finalize(updateOldWalletBalanceStmt);
        if (updateNewWalletBalanceStmt) sqlite3_finalize(updateNewWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::_AB_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateOldWalletBalanceStmt = nullptr;
    sqlite3_stmt *updateNewWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "type = ?, category = ?, "
                                "description = ? WHERE id = ?;"
    ;
    const char *adjustOldWalletBalance_to_IncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustNewWalletBalance_to_IncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustOldWalletBalance_to_ExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *adjustNewWalletBalance_to_ExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_to_IncomeSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_to_IncomeSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_to_ExpenseSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_to_ExpenseSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.type.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  3, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.id);
    sqlite3_bind_int64(updateOldWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateOldWalletBalanceStmt,  2, previous_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateNewWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateNewWalletBalanceStmt,  2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateOldWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateNewWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateOldWalletBalanceStmt) sqlite3_finalize(updateOldWalletBalanceStmt);
        if (updateNewWalletBalanceStmt) sqlite3_finalize(updateNewWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::_A_BC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateOldWalletBalanceStmt = nullptr;
    sqlite3_stmt *updateNewWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "category = ?, "
                                "description = ?, amount = ? WHERE id = ?;"
    ;
    const char *adjustOldWalletBalanceIncomeSQL =  "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *adjustNewWalletBalanceIncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustOldWalletBalanceExpenseSQL = "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustNewWalletBalanceExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalanceIncomeSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalanceIncomeSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalanceExpenseSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalanceExpenseSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 3, toUpdate.amount);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.id);
    sqlite3_bind_int64(updateOldWalletBalanceStmt, 1, previous_amount);
    sqlite3_bind_text(updateOldWalletBalanceStmt,  2, previous_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateNewWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateNewWalletBalanceStmt,  2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateOldWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateNewWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateOldWalletBalanceStmt) sqlite3_finalize(updateOldWalletBalanceStmt);
        if (updateNewWalletBalanceStmt) sqlite3_finalize(updateNewWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}

bool WalletManager::_A_B_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount)
{
    sqlite3_stmt *updateRecordStmt = nullptr;
    sqlite3_stmt *updateOldWalletBalanceStmt = nullptr;
    sqlite3_stmt *updateNewWalletBalanceStmt = nullptr;
    const char *updateRecordSQL = "UPDATE transactions SET "
                                "category = ?, "
                                "description = ?, amount = ? WHERE id = ?;"
    ;
    const char *adjustOldWalletBalance_to_IncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustNewWalletBalance_to_IncomeSQL =  "UPDATE wallets SET balance = balance + ? WHERE name = ?;";
    const char *adjustOldWalletBalance_to_ExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    const char *adjustNewWalletBalance_to_ExpenseSQL = "UPDATE wallets SET balance = balance - ? WHERE name = ?;";
    std::string result_msg = "Failed to update record";
    bool result = false;

    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, updateRecordSQL, -1, &updateRecordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(UpdateRecordSQL) error: " << sqlite3_errmsg(db) << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (WalletManager::caseInsensitiveCMP(toUpdate.type, "INCOME")) {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_to_IncomeSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_to_IncomeSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    } else {
        if (sqlite3_prepare_v2(db, adjustOldWalletBalance_to_ExpenseSQL, -1, &updateOldWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustOldWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
        if (sqlite3_prepare_v2(db, adjustNewWalletBalance_to_ExpenseSQL, -1, &updateNewWalletBalanceStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare(adjustNewWalletBalanceSQL) error: " << sqlite3_errmsg(db) << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            goto cleanup;
        }
    }
    sqlite3_bind_text(updateRecordStmt,  1, toUpdate.category.c_str(),    -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(updateRecordStmt,  2, toUpdate.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateRecordStmt, 3, toUpdate.amount);
    sqlite3_bind_int64(updateRecordStmt, 4, toUpdate.id);
    sqlite3_bind_int64(updateOldWalletBalanceStmt, 1, previous_amount);
    sqlite3_bind_text(updateOldWalletBalanceStmt,  2, previous_wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(updateNewWalletBalanceStmt, 1, toUpdate.amount);
    sqlite3_bind_text(updateNewWalletBalanceStmt,  2, toUpdate.wallet_name.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(updateRecordStmt) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateRecordStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateOldWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_step(updateNewWalletBalanceStmt ) != SQLITE_DONE || sqlite3_changes(db) == 0) {
        std::cerr << "SQL step(updateWalletBalanceStmt) error: " << sqlite3_errmsg(db) << " !!" << endl;
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    result_msg = "Record updated successfully";
    result = true;
    cleanup:
        if (updateOldWalletBalanceStmt) sqlite3_finalize(updateOldWalletBalanceStmt);
        if (updateNewWalletBalanceStmt) sqlite3_finalize(updateNewWalletBalanceStmt);
        if (updateRecordStmt) sqlite3_finalize(updateRecordStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}