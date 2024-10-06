#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

class Database
{
public:
    SQLHENV env;
    SQLHDBC dbc;

    Database() : env(SQL_NULL_HENV), dbc(SQL_NULL_HDBC)
    {
        establishConnection();
    }

    ~Database()
    {
        closeConnection();
    }

    std::vector<std::vector<std::string>> executeQuery(const std::string &query)
    {
        SQLHSTMT stmt;
        SQLRETURN ret;

        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        ret = SQLExecDirect(stmt, (SQLCHAR *)query.c_str(), SQL_NTS);
        checkError(ret, stmt, SQL_HANDLE_STMT);

        std::vector<std::vector<std::string>> results;

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            SQLSMALLINT numCols;
            SQLNumResultCols(stmt, &numCols); // Get number of cols

            // Fetch each row
            SQLCHAR colData[256];
            while (SQLFetch(stmt) == SQL_SUCCESS)
            {
                std::vector<std::string> row;
                for (SQLUSMALLINT i = 1; i <= numCols; i++)
                {
                    SQLGetData(stmt, i, SQL_C_CHAR, colData, sizeof(colData), NULL);
                    row.emplace_back((char *)(colData));
                }
                results.emplace_back(row); // Append to vector
            }
        }

        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        return results; // Return 2d vector
    }
    void printResults(SQLHSTMT stmt)
    {
        // Get and print column headers
        SQLSMALLINT numCols;
        SQLNumResultCols(stmt, &numCols); // Get number of cols
        for (SQLUSMALLINT i = 1; i <= numCols; i++)
        {
            SQLCHAR colName[128];
            SQLSMALLINT nameLen;
            SQLDescribeCol(stmt, i, colName, sizeof(colName), &nameLen, NULL, NULL, NULL, NULL);
            std::cout << colName << "\t"; // Print col name
        }
        std::cout << std::endl;

        // Fetch and print every row
        SQLCHAR colData[256];
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            for (SQLUSMALLINT i = 1; i <= numCols; i++)
            {
                SQLGetData(stmt, i, SQL_C_CHAR, colData, sizeof(colData), NULL);
                std::cout << colData << "\t"; // Print col data
            }
            std::cout << std::endl;
        }
    }

private:
    void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type)
    {
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR message[256];
            SQLGetDiagRec(type, handle, 1, NULL, NULL, message, sizeof(message), NULL);
            std::cerr << "ODBC Error: " << message << std::endl;
        }
    }

    void establishConnection()
    {
        SQLRETURN ret;
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
        SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
        const char *DB_CONNECTION_STRING = std::getenv("ENV_DB_CONNECTION_STRING");
        ret = SQLDriverConnect(dbc, NULL,
                               (SQLCHAR *)DB_CONNECTION_STRING,
                               SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
        checkError(ret, dbc, SQL_HANDLE_DBC);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            std::cout << "Connected to the database.\n";
        }
    }

    void closeConnection()
    {
        SQLDisconnect(dbc);
        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
    }
};
