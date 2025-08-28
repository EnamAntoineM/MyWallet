#include "../include_cpp/my.h"

bool WalletManager::delete_wallet_permanently(const std::string &to_delete)
{
    sqlite3 *db = WalletManager::init_db();
    wallet toDelete{};
    sqlite3_stmt *deleteWalletStmt = nullptr;
    const char *deleteWalletSQL = "DELETE FROM wallets WHERE name = ?;";
    std::string result_msg = "Failed to delete record!!";
    bool result = false;

    glz::read_json(toDelete, to_delete);
    if (sqlite3_prepare_v2(db, deleteWalletSQL, -1, &deleteWalletStmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    sqlite3_bind_text(deleteWalletStmt, 1, toDelete.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(deleteWalletStmt) != SQLITE_DONE ) {
        std::cerr << "Failed to complete the query: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    result_msg = "Wallet deleted successfully";
    result = true;
    cleanup:
        if (deleteWalletStmt) sqlite3_finalize(deleteWalletStmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}