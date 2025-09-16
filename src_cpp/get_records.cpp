#include "../include_cpp/my.h"

std::string WalletManager::get_records(void)
{
    sqlite3 *db = WalletManager::init_db();
    std::vector<transaction> transactionsList{};
    transaction helper{};
    sqlite3_stmt *fetchStmt = nullptr;
    std::string result = "";
    const char *fetchSQL = "SELECT * FROM transactions;";

    if (sqlite3_prepare_v2(db, fetchSQL, -1, &fetchStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare(fetchSQL) error: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    while (sqlite3_step(fetchStmt) == SQLITE_ROW) {
        helper.id                  = sqlite3_column_int(fetchStmt, 0);
        helper.wallet_name         = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 1));
        helper.type                = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 2));
        helper.recorded_at         = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 3));
        helper.updated_at          = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 4));
        helper.category            = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 5));
        helper.description         = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 6));
        helper.amount              =                               sqlite3_column_double(fetchStmt, 7);
        helper.is_archived         =                               sqlite3_column_int(fetchStmt, 8);
        helper.related_wallet_name = reinterpret_cast<const char*>(sqlite3_column_text (fetchStmt, 9));
        transactionsList.push_back(helper);
    }
    glz::write_json(transactionsList, result);
    cleanup:
        if (fetchStmt) sqlite3_finalize(fetchStmt);
        WalletManager::closedb(db);
        return result;
}
