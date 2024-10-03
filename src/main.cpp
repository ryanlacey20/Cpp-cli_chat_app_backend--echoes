#include <iostream>
#include "crow_all.h"
#include "database.h"

int main()
{
    crow::SimpleApp app;
    SQLHANDLE hEnv = SQL_NULL_HENV; 
    SQLHANDLE hDbc = SQL_NULL_HDBC; 
    SQLHANDLE hStmt;                

    // Connect to the database
    if (!connectToDatabase(hEnv, hDbc))
    {
        return -1; // Exit if connection fails
    }

    // Allocate a statement handle
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt); 

   
    CROW_ROUTE(app, "/")
    ([&]()
     {
        // Execute the query  print results
        executeQuery(hStmt);
        return "Query executed. Check console for results."; });

    // Set the port, run on multiple threads, run  app
    app.port(18080).multithreaded().run();

    // Clean up
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}
