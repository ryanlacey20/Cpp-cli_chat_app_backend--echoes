#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <cstdlib>

class Database
{
public:
    Database();  // Obj Constructor
    ~Database(); // Obj Destructor

    std::vector<std::vector<std::string>> executeQuery(const std::string &query); // Function to execute an SQL query
    void printResults();

private:
    SQLHENV env;                // Environment handle
    SQLHDBC dbc;                // Database connection handle
    void establishConnection(); // Function to establish a connection
    void closeConnection();     // Function to close the connection
};

#endif
