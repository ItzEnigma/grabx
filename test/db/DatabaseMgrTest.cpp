#include "db/DatabaseMgr.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "util/Logger.h"

namespace test
{
class DatabaseMgrTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        // Set up logging for tests
        util::Logger::setLogDirection(util::Logger::LogDirection::CONSOLE);

        // Create test directory
        testDbPath = "./test_data/test.db";
        std::filesystem::create_directories("./test_data");

        // Clean up any existing test database
        if (std::filesystem::exists(testDbPath))
        {
            std::filesystem::remove(testDbPath);
        }
    }

    static void TearDownTestSuite()
    {
        // Close the database connection before cleanup
        try
        {
            auto& dbMgr = db::DatabaseMgr::getInstance();
            dbMgr.closeConnection();
        }
        catch (...)
        {
            // Ignore if getInstance fails (e.g., not yet initialized)
        }

        // Clean up test database
        if (std::filesystem::exists(testDbPath))
        {
            std::filesystem::remove(testDbPath);
        }
        std::filesystem::remove_all("./test_data");
    }

    static std::string testDbPath;
};

// Define static member
std::string DatabaseMgrTest::testDbPath;

TEST_F(DatabaseMgrTest, InvalidDatabasePath)
{
    // Test that providing an empty path throws an exception
    EXPECT_THROW(db::DatabaseMgr::getInstance("/invalid/path/to/db"), std::runtime_error);
    EXPECT_THROW(db::DatabaseMgr::getInstance(""), std::invalid_argument);
}

TEST_F(DatabaseMgrTest, ConnectionNotInitialized)
{
    // Manually create a DatabaseMgr instance with an invalid path
    EXPECT_THROW(db::DatabaseMgr::getInstance(""), std::invalid_argument);
}

TEST_F(DatabaseMgrTest, SingletonPattern)
{
    // Test that getInstance returns the same instance
    auto& instance1 = db::DatabaseMgr::getInstance(testDbPath);
    auto& instance2 = db::DatabaseMgr::getInstance();

    EXPECT_EQ(&instance1, &instance2);
}

TEST_F(DatabaseMgrTest, GetConnection)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);
    sqlite3* conn = dbMgr.getConnection();
    EXPECT_NE(conn, nullptr);
}

TEST_F(DatabaseMgrTest, TransactionCommands)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);
    EXPECT_NO_THROW(dbMgr.beginTransaction());
    EXPECT_NO_THROW(dbMgr.commit());
    EXPECT_NO_THROW(dbMgr.beginTransaction());
    EXPECT_NO_THROW(dbMgr.rollback());
}

TEST_F(DatabaseMgrTest, InvalidSQLExecution)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);
    EXPECT_THROW(dbMgr.executeTransaction("INVALID SQL;"), std::runtime_error);
}

TEST_F(DatabaseMgrTest, ExecuteTransactionWithValidSQL)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // Create a test table
    EXPECT_NO_THROW(dbMgr.executeTransaction("CREATE TABLE IF NOT EXISTS test_table (id INTEGER);"));

    // Insert data within transaction
    EXPECT_NO_THROW(dbMgr.beginTransaction());
    EXPECT_NO_THROW(dbMgr.executeTransaction("INSERT INTO test_table VALUES (1);"));
    EXPECT_NO_THROW(dbMgr.commit());

    // Verify data was inserted
    sqlite3* conn = dbMgr.getConnection();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(conn, "SELECT COUNT(*) FROM test_table;", -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    EXPECT_EQ(count, 1);

    // Cleanup
    dbMgr.executeTransaction("DROP TABLE test_table;");
}

TEST_F(DatabaseMgrTest, TransactionRollbackRestoresState)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // Create test table and insert initial data
    dbMgr.executeTransaction("CREATE TABLE IF NOT EXISTS rollback_test (id INTEGER);");
    dbMgr.executeTransaction("INSERT INTO rollback_test VALUES (1);");

    // Start transaction and insert data
    dbMgr.beginTransaction();
    dbMgr.executeTransaction("INSERT INTO rollback_test VALUES (2);");
    dbMgr.rollback();

    // Verify only initial data exists
    sqlite3* conn = dbMgr.getConnection();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(conn, "SELECT COUNT(*) FROM rollback_test;", -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    EXPECT_EQ(count, 1);

    // Cleanup
    dbMgr.executeTransaction("DROP TABLE rollback_test;");
}

TEST_F(DatabaseMgrTest, MultipleTransactions)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // Create test table
    dbMgr.executeTransaction("CREATE TABLE IF NOT EXISTS multi_test (id INTEGER);");

    // First transaction - commit
    dbMgr.beginTransaction();
    dbMgr.executeTransaction("INSERT INTO multi_test VALUES (1);");
    dbMgr.commit();

    // Second transaction - rollback
    dbMgr.beginTransaction();
    dbMgr.executeTransaction("INSERT INTO multi_test VALUES (2);");
    dbMgr.rollback();

    // Third transaction - commit
    dbMgr.beginTransaction();
    dbMgr.executeTransaction("INSERT INTO multi_test VALUES (3);");
    dbMgr.commit();

    // Verify only committed data exists
    sqlite3* conn = dbMgr.getConnection();
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(conn, "SELECT COUNT(*) FROM multi_test;", -1, &stmt, nullptr);
    sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    EXPECT_EQ(count, 2); // Only values 1 and 3 should exist

    // Cleanup
    dbMgr.executeTransaction("DROP TABLE multi_test;");
}

TEST_F(DatabaseMgrTest, ThreadSafetyGetConnection)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // Verify multiple calls to getConnection are safe
    sqlite3* conn1 = nullptr;
    sqlite3* conn2 = nullptr;

    EXPECT_NO_THROW(conn1 = dbMgr.getConnection());
    EXPECT_NO_THROW(conn2 = dbMgr.getConnection());

    // Both should return the same connection
    EXPECT_EQ(conn1, conn2);
    EXPECT_NE(conn1, nullptr);
}

TEST_F(DatabaseMgrTest, TransactionAfterConnectionClosed)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // First, verify connection works
    EXPECT_NE(dbMgr.getConnection(), nullptr);

    // Close the connection
    dbMgr.closeConnection();

    // Attempting transaction operations should throw
    EXPECT_THROW(dbMgr.beginTransaction(), std::runtime_error);
    EXPECT_THROW(dbMgr.commit(), std::runtime_error);
    EXPECT_THROW(dbMgr.rollback(), std::runtime_error);
    EXPECT_THROW(dbMgr.executeTransaction("SELECT 1;"), std::runtime_error);
    // After closing, getConnection should throw
    EXPECT_THROW(dbMgr.getConnection(), std::runtime_error);
}

TEST_F(DatabaseMgrTest, CloseAlreadyClosedConnection)
{
    auto& dbMgr = db::DatabaseMgr::getInstance(testDbPath);

    // Close once
    dbMgr.closeConnection();

    // Close again - should not throw
    EXPECT_NO_THROW(dbMgr.closeConnection());
}

} // namespace test
