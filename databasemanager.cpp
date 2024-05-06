#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

#include <sqlite3.h>
#include <mysql/mysql.h>
#include <libpq-fe.h>

class DatabaseManager {
private:
    void *dbConnection;  // Generic pointer to hold any DB connection
    std::string dbType;

public:
    DatabaseManager(const std::string& dbType): dbType(dbType), dbConnection(nullptr) {
        if (dbType == "SQLite") {
            sqlite3 *conn;
            sqlite3_open("example.db", &conn);
            dbConnection = conn;
        } else if (dbType == "MySQL") {
            MYSQL *conn = mysql_init(nullptr);
            mysql_real_connect(conn, "host", "user", "password", "database", 0, nullptr, 0);
            dbConnection = conn;
        } else if (dbType == "PostgreSQL") {
            PGconn *conn = PQconnectdb("hostaddr='host' user='user' password='password' dbname='database'");
            dbConnection = conn;
        }
    }

    ~DatabaseManager() {
        if (dbType == "SQLite") {
            sqlite3_close(static_cast<sqlite3*>(dbConnection));
        } else if (dbType == "MySQL") {
            mysql_close(static_cast<MYSQL*>(dbConnection));
        } else if (dbType == "PostgreSQL") {
            PQfinish(static_cast<PGconn*>(dbConnection));
        }
    }

    std::string executeSQL(const std::string& sql) {
        if (dbType == "SQLite") {
            char *errMsg = nullptr;
            sqlite3_exec(static_cast<sqlite3*>(dbConnection), sql.c_str(), nullptr, nullptr, &errMsg);
            if (errMsg != nullptr) {
                std::string error = errMsg;
                sqlite3_free(errMsg);
                return error;
            }
        } else if (dbType == "MySQL") {
            if (mysql_query(static_cast<MYSQL*>(dbConnection), sql.c_str())) {
                return mysql_error(static_cast<MYSQL*>(dbConnection));
            }
        } else if (dbType == "PostgreSQL") {
            PGresult *res = PQexec(static_cast<PGconn*>(dbConnection), sql.c_str());
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                std::string error = PQerrorMessage(static_cast<PGconn*>(dbConnection));
                PQclear(res);
                return error;
            }
            PQclear(res);
        }
        return "Query executed
