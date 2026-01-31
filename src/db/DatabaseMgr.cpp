
#include "db/DatabaseMgr.h"

#include <stdexcept>

#include "util/Logger.h"

namespace db
{
DatabaseMgr::DatabaseMgr(const std::string& dbPath) : _db(nullptr)
{
    if (dbPath.empty())
    {
        throw std::invalid_argument("Database path cannot be empty");
    }
    if (sqlite3_open(dbPath.c_str(), &_db) != SQLITE_OK)
    {
        std::string error = sqlite3_errmsg(_db);
        sqlite3_close(_db); // Clean up partial connection
        _db = nullptr;
        throw std::runtime_error("Failed to open database: " + error);
    }
    util::Logger::info("Database opened successfully at: " + dbPath);
}

// GCOVR_EXCL_START (Cannot easily force destructor failure for testing)
DatabaseMgr::~DatabaseMgr()
{
    if (_db)
    {
        sqlite3_close_v2(_db);
        _db = nullptr;
        util::Logger::info("Database connection closed successfully");
    }
}
// GCOVR_EXCL_STOP

DatabaseMgr& DatabaseMgr::getInstance(const std::string& dbPath)
{
    static DatabaseMgr instance(dbPath); // GCOVR_EXCL_BR_LINE (false positive)
    return instance;
}

sqlite3* DatabaseMgr::getConnection()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_db)
    {
        throw std::runtime_error("Database connection is not initialized or failed to open");
    }
    return _db;
}

void DatabaseMgr::closeConnection()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_db)
    {
        sqlite3_close(_db);
        _db = nullptr;
        util::Logger::info("Database connection closed for testing");
    }
}

void DatabaseMgr::executeTransaction(const char* sql)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (!_db)
    {
        throw std::runtime_error("Cannot execute transaction: database connection is closed");
    }
    char* errMsg = nullptr;
    if (sqlite3_exec(_db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg ? errMsg : "Unknown error"; // GCOVR_EXCL_BR_LINE (Defensive)
        sqlite3_free(errMsg);                                  // Free the error message allocated by SQLite
        throw std::runtime_error("SQL error: " + error);
    }
    util::Logger::debug(std::string("Executed SQL: ") + sql);
}

} // namespace db
