#include "../include_cpp/my.h"

    /*----------------------------------------------------------------------------------------------------------------------------------*/
/*|||||                                             [___TO DELETE INCOME AND EXPENSE RECORDS___]                                       |||||*/
    /*----------------------------------------------------------------------------------------------------------------------------------*/

bool WalletManager::delete_record(const std::string &to_delete)
{
    /*|||||_______________________________________________________[___VARIABLE DECLARATION___]__________________________________________________|||||*/

        sqlite3 *db = WalletManager::init_db(); // <--- TO CONNECT TO THE DATABASE
        transaction toDelete{};
        sqlite3_stmt *deleteStatementStmt = nullptr;
        sqlite3_stmt *walletBalanceUpdateStmt = nullptr;
        std::string transferJSON = "";
        const char *deleteSQL = "DELETE FROM transactions WHERE id = ?;";
        const char *updateWalletSQL_forExpense = "UPDATE wallets SET balance = balance + ? WHERE name = ?";
        const char *updateWalletSQL_forIncome = "UPDATE wallets SET balance = balance - ? WHERE name = ?";
        std::string result_msg = "Failed to delete record";
        bool result = false;

    /*|||||____________________________________________________________[_____THE END_____]______________________________________________________|||||*/

        glz::read_json(toDelete, to_delete); //<--- TO PARSE THE JSON INFORMATION INTO THE TRANSACTION STRUCT
        
         /*----------------------------------------------------------------------------------------------------------------------------------*/
    /*|||||                                                        [___BEGIN TRANSACTION___]                                                 |||||*/
         /*----------------------------------------------------------------------------------------------------------------------------------*/

        // if (sqlite3_exec(db, "BEGIN IMMEDIATE;", nullptr, nullptr, nullptr) != SQLITE_OK) { // <--- STARTING TRANSACTION      
        //     std::cerr << "BEGIN IMMEDIATE failed: " << sqlite3_errmsg(db) << '\n';
        //     sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        //     goto cleanup;
        // }

    /*|||||_______________________________________________________[___PREPARING STATEMENTS___]__________________________________________________|||||*/

        if (sqlite3_prepare_v2(db, deleteSQL, -1, &deleteStatementStmt, nullptr) != SQLITE_OK) {
            std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            goto cleanup;
        }
        if (WalletManager::caseInsensitiveCMP(toDelete.type, "INCOME")) {
            if (sqlite3_prepare_v2(db, updateWalletSQL_forIncome, -1, &walletBalanceUpdateStmt, nullptr) != SQLITE_OK) {
                std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
                goto cleanup;
            }
        } else if (WalletManager::caseInsensitiveCMP(toDelete.type, "EXPENSE")) {
            if (sqlite3_prepare_v2(db, updateWalletSQL_forExpense, -1, &walletBalanceUpdateStmt, nullptr) != SQLITE_OK) {
                std::cerr << "SQL prepare failed: " << sqlite3_errmsg(db) << "!!" << endl;
                sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
                goto cleanup;
            }
        }

    /*|||||__________________________________________________________[_____THE END_____]________________________________________________________|||||*/    


    /*|||||_________________________________________[___BINDING PARAMETERS TO THE STATEMENT OBJECTS___]_________________________________________|||||*/

        sqlite3_bind_int(deleteStatementStmt, 1, toDelete.id);
        sqlite3_bind_int64(walletBalanceUpdateStmt, 1, toDelete.amount);
        sqlite3_bind_text(walletBalanceUpdateStmt, 2, toDelete.wallet_name.c_str(), -1, SQLITE_STATIC);

    /*|||||__________________________________________________________[_____THE END_____]________________________________________________________|||||*/


    /*|||||______________________________________________________[___EXECUTING STATEMENTS___]__________________________________________________|||||*/

        if (sqlite3_step(deleteStatementStmt) != SQLITE_DONE) {
            std::cerr << "SQL step failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            goto cleanup;
        }
        if (sqlite3_step(walletBalanceUpdateStmt) != SQLITE_DONE) {
            std::cerr << "SQL step failed: " << sqlite3_errmsg(db) << "!!" << endl;
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr); // <--- ROLLING BACK STATEMENT
            goto cleanup;
        }

    /*|||||___________________________________________________________[_____THE END_____]______________________________________________________|||||*/

    
        /*----------------------------------------------------------------------------------------------------------------------------------*/
    /*|||||                                                        [___CLOSING TRANSACTION___]                                                 |||||*/
        /*----------------------------------------------------------------------------------------------------------------------------------*/
            // if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) != SQLITE_OK) { //  <--- CLOSING TRANSACTION
            //     std::cerr << "COMMIT failed: " << sqlite3_errmsg(db) << '\n';
            //     sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            //     goto cleanup;
            // }
            result_msg = "Record deleted successfully";
            result = true;
        


    /*|||||_________________________________________[___CLEANING STATEMENTS___]________________________________________|||||*/
            cleanup:
                if (deleteStatementStmt) sqlite3_finalize(deleteStatementStmt);
                if (walletBalanceUpdateStmt) sqlite3_finalize(walletBalanceUpdateStmt);
                WalletManager::closedb(db);
                cout << result_msg << endl;
                return result;
    /*|||||_____________________________________________[_____THE END_____]_____________________________________________|||||*/
}