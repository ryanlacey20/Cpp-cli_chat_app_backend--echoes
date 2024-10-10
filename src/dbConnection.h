#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Database
{
public:
    SQLHENV env; // Environment handle
    SQLHDBC dbc; // Connection handle

    Database();  // Constructor to establish connection
    ~Database(); // Destructor to close connection

    json executeQuery(const std::string &query); // Function to execute query and return results (as a jsion)
    void printResults(SQLHSTMT stmt);

private:
    void checkError(SQLRETURN ret, SQLHANDLE handle, SQLSMALLINT type); // Error handling function
    void establishConnection();                                         // Establish connection to the database
    void closeConnection();                                             // Close connection to the database
};

#endif // DBCONNECTION_H
