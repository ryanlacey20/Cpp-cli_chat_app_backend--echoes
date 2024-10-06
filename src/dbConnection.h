#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>

class Database
{
public:
    SQLHENV env; // Environment handle
    SQLHDBC dbc; // Connection handle

    Database();  // Constructor to establish connection
    ~Database(); // Destructor to close connection

    void executeQuery(const std::string &query); // Function to execute query and return results
    void printResults(SQLHSTMT stmt);

private:
    void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type); // Error handling function
    void establishConnection();                                         // Establish connection to the database
    void closeConnection();                                             // Close the connection to the database
};

#endif // DBCONNECTION_H
