#include <iostream>
#include "crow_all.h"
#include "dbConnection.h"
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int startServer()
{
    crow::SimpleApp app;
    Database dbHandler;

    CROW_ROUTE(app, "/getMessages")
    ([&dbHandler](const crow::request &req)
     {
         json results = dbHandler.executeQuery("SELECT * FROM messages ORDER BY created_at");

    // return crow::response(results.dump()); // .dump is converting to a string, not sure if we need or would be better/possible to just return a json..
        return "hello"; });

    app.port(18080).multithreaded().run();

    return 0;
}
