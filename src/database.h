// database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <sql.h>
#include <sqlext.h>

bool connectToDatabase(SQLHANDLE& hEnv, SQLHANDLE& hDbc);
void executeQuery(SQLHANDLE& hStmt);

#endif 
