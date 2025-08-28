#pragma once

#include "my.h"
#include "wallet.h"
#include "transaction.h"

class WalletManager {
    WalletManager();

    public:
        static bool record_transaction       (const std::string &to_record);
        static bool record_transfer          (const std::string &to_record);
        static bool create_wallet            (const std::string &to_create);
        static bool update_record            (const std::string &to_update);
        static bool update_transfer          (const std::string &to_update);
        static bool update_wallet            (const std::string &to_update);
        static std::string get_wallets       (void);
        static std::string get_records       (void);
        static bool delete_record            (const std::string &to_delete);
        static bool restore_wallet           (const std::string &tor_restore);
        static bool delete_wallet            (const std::string &to_delete);
        static bool delete_wallet_permanently(const std::string &to_delete);
    private:
        /*|||||_______________________________________________________[___UPDATE RECORD___]__________________________________________________|||||*/
        
                //THE NEGATIVE OF A STATEMENT WILL BE PRECEEDED BY '_' 
                    // toUpdate.wallet_name == previous_wallet_name ====> A && _A <=== toUpdate.wallet_name != previous_wallet_name
                    //           toUpdate.amount == previous_amount ====> B && _B <=== toUpdate.amount != previous_amount
                    //               toUpdate.type == previous_type ====> C && _C <=== toUpdate.type != previous_type
                //_____SAME WALLET
            static bool ABC(sqlite3 *db, transaction &toUpdate);
            static bool AB_C(sqlite3 *db, transaction &toUpdate);
            static bool A_BC(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount);
            static bool A_B_C(sqlite3 *db, transaction &toUpdate, sqlite3_int64 previous_amount);
                //_____DIFFERENT WALLET
            static bool _ABC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name);
            static bool _AB_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name);
            static bool _A_BC(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount);
            static bool _A_B_C(sqlite3 *db, transaction &toUpdate, std::string &previous_wallet_name, sqlite3_int64 previous_amount);

        /*|||||________________________________________________________[_____THE END_____]___________________________________________________|||||*/
        static sqlite3 *init_db(void);
        static std::string safe_column_text(sqlite3_stmt* stmt, int col);
        static void closedb(sqlite3 *db);
        static void init_tables(sqlite3 *db);
        static bool caseInsensitiveCMP(const std::string& str1, const std::string& str2);
};
