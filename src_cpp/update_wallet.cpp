#include "../include_cpp/my.h"

bool WalletManager::update_wallet(const std::string &to_update)
{
    sqlite3 *db = WalletManager::init_db();
    wallet to_edit{};
    sqlite3_stmt *stmt = nullptr;
    const char *updateSQL = "UPDATE wallets SET "
                    "name = ?, currency = ?, source = ?, initial_amount = ?, "
                    "balance = ? , color = ? WHERE rowid = ?;"
    ;
    std::string result_msg = "Failed to update wallet!";
    bool result = false;

    glz::read_json(to_edit, to_update);
    if (sqlite3_prepare_v2(db, updateSQL,-1 , &stmt, nullptr)!=SQLITE_OK){
        std::cerr << "SQL prepare error:" << sqlite3_errmsg(db) <<endl;
        goto cleanup;
    }
    sqlite3_bind_text(stmt,   1, to_edit.name.c_str() ,    -1 ,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   2, to_edit.currency.c_str() ,-1 ,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt,   3, to_edit.source.c_str() ,  -1 ,SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt,  4, to_edit.initial_amount);
    sqlite3_bind_int64(stmt,  5, to_edit.balance);
    sqlite3_bind_text(stmt,   6, to_edit.color.c_str() ,   -1 ,SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt,  7, to_edit.id);

    if (!(sqlite3_step(stmt) == SQLITE_DONE)){
        std::cerr << "SQL step error : " << sqlite3_errmsg(db) <<endl;
        goto cleanup;
    }

    result_msg = "Wallet updated successfully";
    result = true;
    cleanup:
        if (stmt) sqlite3_finalize(stmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}