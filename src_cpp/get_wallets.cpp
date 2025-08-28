#include "../include_cpp/my.h"

std::string WalletManager::safe_column_text(sqlite3_stmt* stmt, int col) {
    const unsigned char* text = sqlite3_column_text(stmt, col);
    return text ? reinterpret_cast<const char*>(text) : "";
}

std::string WalletManager::get_wallets(void)
{ 
    sqlite3 *db = WalletManager::init_db();
    std::vector<wallet> walletList;
    wallet pusher;
    sqlite3_stmt *stmt = nullptr;  
    std::string result;
    const char *fetchSQL = "SELECT rowid, name, currency, source, initial_amount, "
                          "balance, color, created_at, updated_at, is_active FROM wallets";

    if (sqlite3_prepare_v2(db, fetchSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << std::endl;
        goto cleanup;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        pusher.id =             sqlite3_column_int  (stmt, 0);
        pusher.name =           safe_column_text    (stmt, 1);
        pusher.currency =       safe_column_text    (stmt, 2);
        pusher.source =         safe_column_text    (stmt, 3);
        pusher.initial_amount = sqlite3_column_int  (stmt, 4);
        pusher.balance =        sqlite3_column_int64(stmt, 5);
        pusher.color =          safe_column_text    (stmt, 6);
        pusher.created_at =     safe_column_text    (stmt, 7);
        pusher.updated_at =     safe_column_text    (stmt, 8);
        pusher.is_active =      sqlite3_column_int  (stmt, 9);
        walletList.push_back(pusher);
    }
    try {
        glz::write_json(walletList, result);
    } catch (const std::exception& error) {
        std::cerr << "JSON serialization error: " << error.what() << std::endl;
        result = "[]"; 
    }

cleanup:
    if (stmt) sqlite3_finalize(stmt);
    WalletManager::closedb(db);
    return result;
}
