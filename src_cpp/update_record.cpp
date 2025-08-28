#include "../include_cpp/my.h"

bool WalletManager::update_record(const std::string &to_update)
{
    sqlite3 *db = WalletManager::init_db();
    transaction toUpdate{};
    sqlite3_int64 previous_amount = 0;
    const unsigned char* previous_type_from_SQL;
    std::string previous_type = "";
    const unsigned char* previous_wallet_name_from_SQL;
    std::string previous_wallet_name = "";
    sqlite3_stmt *get_previous_amount_of_transaction_to_be_updated_Stmt = nullptr;
    sqlite3_stmt *get_previous_type_of_transaction_to_be_updated_Stmt = nullptr;
    sqlite3_stmt *get_previous_wallet_name_of_transaction_to_be_updated_Stmt = nullptr;
    const char *get_previous_amount_of_transaction_to_be_updated_SQL = "SELECT amount FROM transactions WHERE id = ?;";
    const char *get_previous_type_of_transaction_to_be_updated_SQL = "SELECT type FROM transactions where id = ?;";
    const char *get_previous_wallet_name_of_transaction_to_be_updated_SQL = "SELECT wallet_name FROM transactions WHERE id = ?;";
    std::string result_msg = "Failed to update record!";
    bool result = false;
    

    glz::read_json(toUpdate, to_update);
    if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, get_previous_amount_of_transaction_to_be_updated_SQL, -1, &get_previous_amount_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, get_previous_type_of_transaction_to_be_updated_SQL, -1, &get_previous_type_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    if (sqlite3_prepare_v2(db, get_previous_wallet_name_of_transaction_to_be_updated_SQL, -1, &get_previous_wallet_name_of_transaction_to_be_updated_Stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL prepare error: " << sqlite3_errmsg(db) << endl;
        goto cleanup;
    }
    sqlite3_bind_int(get_previous_amount_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    sqlite3_bind_int(get_previous_type_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    sqlite3_bind_int(get_previous_wallet_name_of_transaction_to_be_updated_Stmt, 1, toUpdate.id);
    if (sqlite3_step(get_previous_amount_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        previous_amount = sqlite3_column_int64(get_previous_amount_of_transaction_to_be_updated_Stmt, 0);
    } else {
        std::cerr << "No transaction found with that ID.\n";
        goto cleanup;
    }
    if (sqlite3_step(get_previous_type_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        previous_type_from_SQL = sqlite3_column_text(get_previous_type_of_transaction_to_be_updated_Stmt, 0);
        previous_type = reinterpret_cast<const char *>(previous_type_from_SQL);
    } else {
        std::cerr << "No type found with that ID.\n";
        goto cleanup;
    }
    if (sqlite3_step(get_previous_wallet_name_of_transaction_to_be_updated_Stmt) == SQLITE_ROW) {
        previous_wallet_name_from_SQL = sqlite3_column_text(get_previous_wallet_name_of_transaction_to_be_updated_Stmt, 0);
        previous_wallet_name = reinterpret_cast<const char *>(previous_wallet_name_from_SQL);
    } else {
        std::cerr << "Couldn't retrieve previous wallet name" << endl;
        goto cleanup;
    }
    if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        goto cleanup;
    }
    
    //THE NEGATIVE OF A STATEMENT WILL BE PRECEEDED BY '_' 
    // toUpdate.wallet_name == previous_wallet_name ====> A && _A <=== toUpdate.wallet_name != previous_wallet_name
    //           toUpdate.amount == previous_amount ====> B && _B <=== toUpdate.amount != previous_amount
    //               toUpdate.type == previous_type ====> C && _C <=== toUpdate.type != previous_type

    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type == previous_type)) result_msg = WalletManager::ABC(db, toUpdate);                                          //CASE 1 ABC
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type == previous_type)) result_msg = WalletManager::A_BC(db, toUpdate, previous_amount);                        //CASE 2 A_BC
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type != previous_type)) result_msg = WalletManager::AB_C(db, toUpdate);                                         //CASE 3 AB_C
    if ((toUpdate.wallet_name == previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type != previous_type)) result_msg = WalletManager::A_B_C(db, toUpdate, previous_amount);                       //CASE 4 A_sult =
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type == previous_type)) result_msg = WalletManager::_ABC(db, toUpdate, previous_wallet_name);                    //CASE 5 _ABC
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type == previous_type)) result_msg = WalletManager::_A_BC(db, toUpdate, previous_wallet_name, previous_amount);  //CASE 6 _A_BC
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount == previous_amount) && (toUpdate.type != previous_type)) result_msg = WalletManager::_AB_C(db, toUpdate, previous_wallet_name);                   //CASE 7 _AB_C
    if ((toUpdate.wallet_name != previous_wallet_name) && (toUpdate.amount != previous_amount) && (toUpdate.type != previous_type)) result_msg = WalletManager::_A_B_C(db, toUpdate, previous_wallet_name, previous_amount); //CASE 8 _A_B_C
    cleanup:
        if (get_previous_amount_of_transaction_to_be_updated_Stmt) sqlite3_finalize(get_previous_amount_of_transaction_to_be_updated_Stmt);
        if (get_previous_type_of_transaction_to_be_updated_Stmt) sqlite3_finalize(get_previous_type_of_transaction_to_be_updated_Stmt);
        if (get_previous_wallet_name_of_transaction_to_be_updated_Stmt) sqlite3_finalize(get_previous_wallet_name_of_transaction_to_be_updated_Stmt);
        WalletManager::closedb(db);
        cout << result_msg << endl;
        return result;
}