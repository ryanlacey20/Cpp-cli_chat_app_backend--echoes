// database.cpp
#include <iostream>
#include "database.h"

bool database(SQLHANDLE &hEnv, SQLHANDLE &hDbc)
{
    SQLRETURN ret;
    SQLCHAR outConnStr[1024];
    SQLSMALLINT outConnStrLen;

    // Allocate the ODBC environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::cerr << "Error allocating environment handle." << std::endl;
        return false;
    }

    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        std::cerr << "Error allocating connection handle." << std::endl;
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }

    // Connect to DB
    ret = SQLDriverConnect(hDbc, NULL,
                           (SQLCHAR *)"DRIVER={ODBC Driver 17 for SQL Server};"
                                      "SERVER=ryantadhg-server.database.windows.net;"
                                      "DATABASE=echoes;"
                                      "UID=ryantadhg;"
                                      "PWD=Meetmeafterthetogaparty!;",
                           SQL_NTS, outConnStr, sizeof(outConnStr),
                           &outConnStrLen, SQL_DRIVER_NOPROMPT);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "Connected to the database." << std::endl;
        return true;
    }
    else
    {
        std::cerr << "Failed to connect to the database." << std::endl;
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
        return false;
    }
}

void executeQuery(SQLHANDLE &hStmt)
{
    SQLRETURN ret;
    ret = SQLExecDirect(hStmt, (SQLCHAR *)"SELECT * FROM your_table", SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR columnValue[256];
        SQLLEN indicator;

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            SQLGetData(hStmt, 1, SQL_C_CHAR, columnValue, sizeof(columnValue), &indicator);
            std::cout << "Column Value: " << columnValue << std::endl;
        }
    }
    else
    {
        std::cerr << "Error executing query." << std::endl;
    }
}
