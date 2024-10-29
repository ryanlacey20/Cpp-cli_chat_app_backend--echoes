#ifdef _WIN32
#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "dbConnection.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Database::Database()
{
    env = SQL_NULL_HENV;
    dbc = SQL_NULL_HDBC;
    establishConnection();
    std::cout << "DB INSTANCE CREATED \n" << std::endl;
}

Database::~Database()
{
    std::cout << "DB INSTANCE DESTROYED \n" << std::endl;
    closeConnection();
}

json Database::executeQuery(const std::string &query) // Change return type to json
{
    SQLHSTMT stmt;
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLExecDirect(stmt, (SQLCHAR *)query.c_str(), SQL_NTS);
    checkError(ret, stmt, SQL_HANDLE_STMT);

    json jsonResponse = json::array(); // Create a JSON array to hold the results

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLSMALLINT numCols;
        SQLNumResultCols(stmt, &numCols); // Get number of cols

        // Fetch each row
        SQLCHAR colData[256];
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            json jsonRow = json::object(); // Create a new JSON object for the row

            for (SQLUSMALLINT i = 1; i <= numCols; i++)
            {
                SQLCHAR colName[128];
                SQLSMALLINT nameLen;
                SQLDescribeCol(stmt, i, colName, sizeof(colName), &nameLen, NULL, NULL, NULL, NULL);

                // Get the column data and add it to the JSON object
                SQLGetData(stmt, i, SQL_C_CHAR, colData, sizeof(colData), NULL);
                jsonRow[std::string(reinterpret_cast<char *>(colName), nameLen)] = std::string(reinterpret_cast<char *>(colData)); // Use column name as key
            }

            jsonResponse.push_back(jsonRow); // Append the JSON row to the JSON array
        }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return jsonResponse; // Return the JSON array
}

void Database::printResults(SQLHSTMT stmt)
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

void Database::checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type)
{
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR message[256];
        SQLGetDiagRec(type, handle, 1, NULL, NULL, message, sizeof(message), NULL);
        std::cerr << "ODBC Error: " << message << std::endl;
    }
}

void Database::establishConnection()
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

void Database::closeConnection()
{
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}
