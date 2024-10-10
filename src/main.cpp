#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>
#include "server.h"

int main()
{

    startServer();

    return 0;
}
