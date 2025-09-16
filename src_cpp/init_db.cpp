#include "../include_cpp/my.h"

static const char *walletsSQL = "CREATE TABLE IF NOT EXISTS wallets ("
                                      "name TEXT PRIMARY KEY COLLATE NOCASE,"   
                                      "currency TEXT NOT NULL COLLATE NOCASE,"
                                      "source TEXT NOT NULL,"
                                      "initial_amount REAL DEFAULT 0,"
                                      "balance REAL DEFAULT 0,"
                                      "color TEXT,"
                                      "created_at TEXT DEFAULT CURRENT_TIMESTAMP,"
                                      "updated_at TEXT DEFAULT CURRENT_TIMESTAMP, "
                                      "is_active INTEGER DEFAULT TRUE);"
                                      ;
static const char *transactionSQL = "CREATE TABLE IF NOT EXISTS transactions ("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                          "wallet_name TEXT NOT NULL COLLATE NOCASE,"
                                          "type TEXT COLLATE NOCASE CHECK (type IN ('INCOME', 'EXPENSE', 'TRANSFER')) NOT NULL,"
                                          "recorded_at TEXT DEFAULT CURRENT_TIMESTAMP,"
                                          "updated_at TEXT DEFAULT CURRENT_TIMESTAMP,"
                                          "category TEXT,"
                                          "description TEXT,"
                                          "amount REAL DEFAULT 0, "
                                          "is_archived INTEGER DEFAULT FALSE, "
                                          "related_wallet_name TEXT COLLATE NOCASE,"
                                          "FOREIGN KEY (wallet_name) REFERENCES wallets(name) ON UPDATE CASCADE ON DELETE CASCADE, "
                                          "FOREIGN KEY (related_wallet_name) REFERENCES wallets(name) ON UPDATE CASCADE ON DELETE CASCADE);"
                                    ;
static const char *triggerUpdateAt_Wallet = "CREATE TRIGGER IF NOT EXISTS trigger_update_at_wallet "
                                            "AFTER UPDATE ON wallets "
                                                "FOR EACH ROW "
                                                    "WHEN NEW.updated_at = OLD.updated_at "
                                                        "BEGIN "
                                                            "UPDATE wallets "
                                                                "SET updated_at = CURRENT_TIMESTAMP "
                                                                "WHERE rowid = OLD.rowid; "
                                                        "END;"
;
static const char *triggerUpdateAt_Transactions = "CREATE TRIGGER IF NOT EXISTS trigger_update_at_transactions "
                                                "AFTER UPDATE ON transactions "
                                                    "FOR EACH ROW "
                                                     "WHEN NEW.updated_at = OLD.updated_at "
                                                        "BEGIN "
                                                            "UPDATE transactions "
                                                                "SET updated_at = CURRENT_TIMESTAMP "
                                                                "WHERE id = OLD.id; "
                                                        "END;"
;

sqlite3 *WalletManager::init_db(void)
{
    namespace fs = std::filesystem;
    sqlite3 *db = nullptr;
    fs::path dbPath1;
    fs::path dbPath2;
    fs::path dbfullPath;
    char *appdata;
    char *home;
    char *errmsg;

    #if _WIN32
        appdata = std::getenv("APPDATA");
        dbPath1 = appdata ? fs::path(appdata) : fs::temp_directory_path();
        
        char *localapp = std::getenv("LOCALAPPDATA");
        fs::path saucerPath = localapp ? fs::path(localapp) / "myWallet" : fs::temp_directory_path();

        saucer::config cfg;
        cfg.data_directory = saucerPath.string();
    #else
        home = std::getenv("HOME");
        dbPath1 = home ? fs::path(home) / ".local" / "share" : fs::temp_directory_path();
    #endif

    dbPath2 = dbPath1 / "myWallet";
    fs::create_directories(dbPath2);
    dbfullPath = dbPath2 / "myWallet.sqlite";

    if (sqlite3_open(dbfullPath.string().c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open the database: " << sqlite3_errmsg(db) << "!!" << endl;
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to open the database");
    }
    sqlite3_busy_timeout(db, 5000);
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    WalletManager::init_tables(db);
    sqlite3_exec(db, triggerUpdateAt_Wallet, nullptr, nullptr, nullptr);
    sqlite3_exec(db, triggerUpdateAt_Transactions, nullptr, nullptr, nullptr);
    return db;
}

void WalletManager::init_tables(sqlite3 *db)
{
    char *errmsg;
    if (sqlite3_exec(db, walletsSQL, 0, 0, &errmsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errmsg << " !!" << endl;
        sqlite3_free(errmsg);
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to create table!");
    }
    if (sqlite3_exec(db, transactionSQL, 0, 0, &errmsg) != SQLITE_OK) {
        std::cerr << "Failed to create table: " << errmsg << " !!" << endl;
        sqlite3_free(errmsg);
        WalletManager::closedb(db);
        throw std::runtime_error("Failed to create table!");
    }
}

void WalletManager::closedb(sqlite3 *db)
{
    if (db != nullptr) {
        sqlite3_close(db);
    }
}