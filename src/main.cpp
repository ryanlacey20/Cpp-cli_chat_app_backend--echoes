#include <iostream>
#include <sql.h>
#include <sqlext.h>

void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type)
{
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR message[256];
        SQLGetDiagRec(type, handle, 1, NULL, NULL, message, sizeof(message), NULL);
        std::cerr << "ODBC Error: " << message << std::endl;
    }
}

int main()
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;

    // Initialize the ENV
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    // Connect to DB
    ret = SQLDriverConnect(dbc, NULL,
                           (SQLCHAR *)"Driver={ODBC Driver 18 for SQL Server};Server=tcp:ryantadhg-server.database.windows.net,1433;"
                                      "Database=echoes;Uid=ryantadhg;Pwd=Meetmeafterthetogaparty!;Encrypt=yes;TrustServerCertificate=no;",
                           SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    checkError(ret, dbc, SQL_HANDLE_DBC);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "Connected to the database.\n";

        // Allocate statement handle
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        ret = SQLExecDirect(stmt, (SQLCHAR *)"SELECT * FROM messages", SQL_NTS);
        checkError(ret, stmt, SQL_HANDLE_STMT);

        // Get and print column headers (column names)
        SQLSMALLINT numCols;
        SQLNumResultCols(stmt, &numCols); // Get the number of columns in the result set
        for (SQLUSMALLINT i = 1; i <= numCols; i++)
        {
            SQLCHAR colName[128];
            SQLSMALLINT nameLen;
            SQLDescribeCol(stmt, i, colName, sizeof(colName), &nameLen, NULL, NULL, NULL, NULL);
            std::cout << colName << "\t"; // Print column name
        }
        std::cout << std::endl;

        // Fetch and print all rows
        SQLCHAR colData[256];
        while (SQLFetch(stmt) == SQL_SUCCESS)
        {
            for (SQLUSMALLINT i = 1; i <= numCols; i++)
            {
                SQLGetData(stmt, i, SQL_C_CHAR, colData, sizeof(colData), NULL);
                std::cout << colData << "\t"; // Print column data
            }
            std::cout << std::endl;
        }

        // Clean up
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    }

    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

    return 0;
}
