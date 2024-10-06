#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include "server.h"
#include "dbConnection.h"

int main()
{
    establishConnection();
    startServer();

    return 0;
}
