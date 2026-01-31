/**
 * @file DatabaseMgr.h
 * @brief Thread-safe singleton manager for SQLite connection.
 */

#pragma once
#include <sqlite3.h>

#include <mutex>
#include <string>

namespace db
{

/**
 * @class DatabaseMgr
 * @brief Singleton managing SQLite connections with transaction support.
 *
 * Features:
 * - Ensures single shared DB connection
 * - Thread-safe via mutex
 * - Provides transaction handling (begin, commit, rollback)
 * - RAII cleanup of SQLite connection
 */
class DatabaseMgr
{
public:
    /**
     * @brief Get the singleton instance of DatabaseMgr
     *
     * @param dbPath Path to the SQLite database file. Only used on first call.
     *               Subsequent calls ignore this parameter.
     * @return Reference to the singleton DatabaseMgr instance
     *
     * @throws std::invalid_argument if dbPath is empty on first call
     * @throws std::runtime_error if database cannot be opened
     *
     * @note Thread-safe: Multiple threads can safely call this method
     */
    static DatabaseMgr& getInstance(const std::string& dbPath = "");

    /**
     * @brief Get the raw SQLite database connection
     *
     * @return sqlite3* Raw SQLite database connection handle
     *
     * @throws std::runtime_error if database connection is not initialized
     *          or connection has been closed
     *
     * @note Thread-safe: Uses mutex protection for concurrent access
     * @note The returned pointer should not be stored long-term as the
     *       DatabaseMgr manages the connection lifecycle
     *
     * @warning Do not call sqlite3_close() on the returned connection.
     *         Connection cleanup is handled by the DatabaseMgr destructor.
     */
    sqlite3* getConnection();

    /**
     * @brief Close the database connection (primarily for testing)
     *
     * Closes the current database connection if open. This allows
     * test cleanup to remove database files on Windows where open
     * files cannot be deleted.
     *
     * @note Thread-safe: Uses mutex protection
     * @warning Should only be used in testing scenarios
     */
    void closeConnection();

    /**
     * @brief Begin a database transaction
     *
     * Starts a new database transaction by executing "BEGIN TRANSACTION;".
     * Transactions provide ACID properties and should be used when performing
     * multiple related database operations that need to be atomic.
     *
     * @throws std::runtime_error if the BEGIN TRANSACTION command fails
     *
     * @note Transactions should be properly closed with either commit() or rollback()
     *
     * @see commit()
     * @see rollback()
     */
    void beginTransaction() { executeTransaction("BEGIN TRANSACTION;"); }

    /**
     * @brief Commit the current database transaction
     *
     * Commits all changes made within the current transaction to the database.
     * This makes all changes permanent and ends the transaction.
     *
     * @throws std::runtime_error if the COMMIT command fails or no transaction is active
     *
     * @note Should only be called after beginTransaction()
     *
     * @see beginTransaction()
     * @see rollback()
     */
    void commit() { executeTransaction("COMMIT;"); }

    /**
     * @brief Rollback the current database transaction
     *
     * Discards all changes made within the current transaction and restores
     * the database to its state before the transaction began. This is typically
     * used for error recovery.
     *
     * @throws std::runtime_error if the ROLLBACK command fails
     *
     * @note Should only be called after beginTransaction()
     * @note It's safe to call rollback even if no transaction is active (SQLite will ignore it)
     *
     * @see beginTransaction()
     * @see commit()
     */
    void rollback() { executeTransaction("ROLLBACK;"); }

private:
    /**
     * @brief Private constructor for singleton pattern
     *
     * Initializes the DatabaseMgr instance with the specified database path.
     * Opens the SQLite database connection and validates the operation.
     *
     * @param dbPath Path to the SQLite database file
     *
     * @throws std::invalid_argument if dbPath is empty
     * @throws std::runtime_error if database cannot be opened
     */
    explicit DatabaseMgr(const std::string& dbPath);

    /**
     * @brief Destructor - cleanup database connection
     *
     * Properly closes the SQLite database connection if it's still open.
     * Ensures no resource leaks occur when the singleton is destroyed
     * at program termination.
     *
     * @note Automatically called at program exit for singleton cleanup
     */
    ~DatabaseMgr();

    /**
     * @brief Deleted copy constructor and assignment operators
     *
     * These special member functions are explicitly deleted to prevent
     * copying or moving of the DatabaseMgr instance, maintaining the
     * singleton pattern integrity.
     *
     * @note Prevents accidental duplication of database connections
     * @note Ensures only one DatabaseMgr instance exists at any time
     */
    DatabaseMgr(const DatabaseMgr&) = delete;
    DatabaseMgr& operator=(const DatabaseMgr&) = delete;
    DatabaseMgr(DatabaseMgr&&) = delete;
    DatabaseMgr& operator=(DatabaseMgr&&) = delete;

public: /** TODO: SHOULD IT BE MADE PRIVATE AND ADDING VISIBLITY ON UTs? */
    /**
     * @brief Execute a transaction command with error handling
     *
     * Internal helper method to execute SQL transaction commands
     * (BEGIN, COMMIT, ROLLBACK) with proper error handling and
     * mutex protection.
     *
     * @param sql SQL transaction command to execute
     *
     * @throws std::runtime_error if the SQL command fails
     *
     * @note Thread-safe: Uses mutex protection
     */
    void executeTransaction(const char* sql);

private:
    /**
     * @brief SQLite database connection handle
     *
     * Raw SQLite3 database connection pointer managed by this class.
     * Initialized in constructor and cleaned up in destructor.
     */
    sqlite3* _db;

    /**
     * @brief Mutex for thread-safe database operations
     *
     * Protects all database operations from concurrent access issues.
     * Ensures thread safety when multiple threads access the database
     * simultaneously.
     */
    std::mutex _mutex;
};

} // namespace db
